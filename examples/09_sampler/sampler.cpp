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
    auto backend = service.backend("ibm_torino");
    auto pm = generate_preset_pass_manager(2, backend);
    auto transpiled = pm.run(circ);

    auto sampler = BackendSamplerV2(backend, 100);  // 100 shots
    auto result = sampler.run({SamplerPub(transpiled)});

    // Process results
    auto pub_result = result[0];
    auto meas_data = pub_result.data("meas");
    auto counts = meas_data.get_counts();

    std::cout << "Measurement results:" << std::endl;
    for (auto& kv : counts) {
        std::cout << "  " << kv.first << ": " << kv.second << std::endl;
    }

    return 0;
}
