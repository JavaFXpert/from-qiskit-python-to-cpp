// Example 09: Sampler — Full end-to-end: build, transpile, sample, process.
// Requires: QRMI and IBM Quantum credentials.

#include "circuit/quantumcircuit.hpp"
#include "transpiler/preset_passmanagers/generate_preset_pass_manager.hpp"
#include "primitives/backend_sampler_v2.hpp"
#include "service/qiskit_runtime_service.hpp"

using namespace Qiskit::circuit;
using namespace Qiskit::transpiler;
using namespace Qiskit::primitives;

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

    // Find max count for scaling the histogram
    int num_qubits = 2;
    int num_states = 1 << num_qubits;  // 2^num_qubits
    int max_count = 0;
    for (auto& kv : counts) {
        if (kv.second > max_count) max_count = kv.second;
    }

    // Display histogram for all computational basis states in order
    std::cout << "Measurement results:" << std::endl;
    for (int i = 0; i < num_states; i++) {
        std::string state;
        for (int b = num_qubits - 1; b >= 0; b--) {
            state += ((i >> b) & 1) ? '1' : '0';
        }
        int count = 0;
        auto it = counts.find(state);
        if (it != counts.end()) count = it->second;
        int bar_len = (max_count > 0) ? count * 40 / max_count : 0;
        std::cout << "  " << state << " | ";
        for (int j = 0; j < bar_len; j++) std::cout << "\u2588";
        std::cout << " " << count << std::endl;
    }

    return 0;
}
