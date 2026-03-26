// Example 09: Sampler — Full end-to-end: build, transpile, sample, process.
// Requires: QRMI and IBM Quantum credentials.

#include "circuit/quantumcircuit.hpp"
#include "transpiler/preset_passmanagers/generate_preset_pass_manager.hpp"
#include "primitives/backend_sampler_v2.hpp"
#include "service/qiskit_runtime_service.hpp"
#include <map>
#include <string>
#include <vector>
#include <iomanip>

using namespace Qiskit::circuit;
using namespace Qiskit::transpiler;
using namespace Qiskit::primitives;

void print_histogram(const std::map<std::string, int>& counts,
                     int num_qubits, int height = 20, int col_width = 6) {
    int num_states = 1 << num_qubits;

    // Build ordered list of states and their counts
    std::vector<std::string> states;
    std::vector<int> values;
    int max_count = 0;
    for (int i = 0; i < num_states; i++) {
        std::string state;
        for (int b = num_qubits - 1; b >= 0; b--) {
            state += ((i >> b) & 1) ? '1' : '0';
        }
        states.push_back(state);
        auto it = counts.find(state);
        int count = (it != counts.end()) ? it->second : 0;
        values.push_back(count);
        if (count > max_count) max_count = count;
    }

    // Draw bars top-down, row by row
    for (int row = height; row >= 1; row--) {
        int threshold = max_count * row / height;
        for (int col = 0; col < num_states; col++) {
            if (values[col] >= threshold && threshold > 0) {
                // Print count label on the top row of each bar
                if (row == height || max_count * (row + 1) / height > values[col]) {
                    std::cout << std::setw(col_width) << values[col];
                } else {
                    std::string bar(col_width - 1, '\xe2' == '\xe2' ? ' ' : ' ');
                    std::cout << " ";
                    for (int k = 0; k < col_width - 2; k++) std::cout << "\u2588";
                    std::cout << " ";
                }
            } else {
                std::cout << std::string(col_width, ' ');
            }
        }
        std::cout << std::endl;

        // Print bar blocks for this row
        if (row > 1) {
            for (int col = 0; col < num_states; col++) {
                int bar_threshold = max_count * (row - 1) / height;
                if (values[col] > bar_threshold && bar_threshold >= 0) {
                    std::cout << " ";
                    for (int k = 0; k < col_width - 2; k++) std::cout << "\u2588";
                    std::cout << " ";
                } else {
                    std::cout << std::string(col_width, ' ');
                }
            }
            std::cout << std::endl;
        }
    }

    // Baseline
    for (int col = 0; col < num_states; col++) {
        for (int k = 0; k < col_width; k++) std::cout << "\u2500";
    }
    std::cout << std::endl;

    // State labels
    for (int col = 0; col < num_states; col++) {
        std::cout << std::setw(col_width) << states[col];
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
