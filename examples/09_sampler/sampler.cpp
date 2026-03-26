// Example 09: Sampler — Full end-to-end: build, transpile, sample, process.
// Requires: QRMI and IBM Quantum credentials.

#include "circuit/quantumcircuit.hpp"
#include "transpiler/preset_passmanagers/generate_preset_pass_manager.hpp"
#include "primitives/backend_sampler_v2.hpp"
#include "service/qiskit_runtime_service.hpp"
#include <unordered_map>
#include <string>
#include <vector>
#include <iomanip>

using namespace Qiskit::circuit;
using namespace Qiskit::transpiler;
using namespace Qiskit::primitives;

std::string center(const std::string& text, int width) {
    int pad = width - (int)text.size();
    if (pad <= 0) return text;
    int left = pad / 2;
    int right = pad - left;
    return std::string(left, ' ') + text + std::string(right, ' ');
}

void print_histogram(const std::unordered_map<std::string, unsigned long long>& counts,
                     int num_qubits, int height = 16, int col_width = 7) {
    int num_states = 1 << num_qubits;

    // Build ordered list of states and their counts
    std::vector<std::string> states;
    std::vector<unsigned long long> values;
    unsigned long long max_count = 0;
    for (int i = 0; i < num_states; i++) {
        std::string state;
        for (int b = num_qubits - 1; b >= 0; b--) {
            state += ((i >> b) & 1) ? '1' : '0';
        }
        states.push_back(state);
        auto it = counts.find(state);
        unsigned long long count = (it != counts.end()) ? it->second : 0;
        values.push_back(count);
        if (count > max_count) max_count = count;
    }

    // Compute scaled bar heights (minimum 1 if count > 0, so small bars show)
    std::vector<int> bar_heights(num_states);
    for (int col = 0; col < num_states; col++) {
        if (values[col] == 0) {
            bar_heights[col] = 0;
        } else {
            int h = (int)(values[col] * height / max_count);
            bar_heights[col] = (h < 1) ? 1 : h;
        }
    }

    // Build the bar block: use upper-half + lower-half blocks for solid fill
    // Each row prints two half-blocks to eliminate the gap between lines
    std::string block_top = " ";
    std::string block_mid = " ";
    for (int k = 0; k < col_width - 2; k++) {
        block_top += "\xe2\x96\x84";  // lower half block (fills bottom of line above)
        block_mid += "\xe2\x96\x88";  // full block
    }
    block_top += " ";
    block_mid += " ";
    std::string blank(col_width, ' ');

    // Draw rows from top to bottom
    for (int row = height; row >= 1; row--) {
        for (int col = 0; col < num_states; col++) {
            if (bar_heights[col] == row) {
                // Top of this bar: print count label centered
                std::cout << center(std::to_string(values[col]), col_width);
            } else if (bar_heights[col] > row) {
                // Body of this bar
                std::cout << block_mid;
            } else if (bar_heights[col] == 0 && row == 1) {
                // Zero count: show the number centered just above baseline
                std::cout << center(std::to_string(values[col]), col_width);
            } else {
                std::cout << blank;
            }
        }
        std::cout << "\n";
    }

    // Baseline
    for (int col = 0; col < num_states * col_width; col++) {
        std::cout << "\xe2\x94\x80";
    }
    std::cout << "\n";

    // State labels centered
    for (int col = 0; col < num_states; col++) {
        std::cout << center(states[col], col_width);
    }
    std::cout << std::endl;
}

int main() {
    // Build a Bell state circuit
    QuantumRegister qr(2);
    ClassicalRegister cr(2, std::string("meas"));
    QuantumCircuit circ(qr, cr);

    circ.h(0);
    circ.cx(0, 1);
    circ.measure(qr, cr);

    // Connect, transpile, and run
    auto service = Qiskit::service::QiskitRuntimeService();
    auto backend = service.backend("ibm_fez");
    auto pm = generate_preset_pass_manager(2, backend);
    auto transpiled = pm.run(circ);

    auto sampler = BackendSamplerV2(backend, 100);  // 100 shots
    auto job = sampler.run({SamplerPub(transpiled)});
    auto result = job->result();

    // Process results
    auto pub_result = result[0];
    auto meas_data = pub_result.data("meas");
    auto counts = meas_data.get_counts();

    std::cout << "\nMeasurement results:" << std::endl;
    print_histogram(counts, 2);

    return 0;
}
