// Example 09: Backend Transpilation — Transpile for a real IBM backend.
// Requires: qiskit-ibm-runtime-c and IBM Quantum credentials.

#include "circuit/quantumcircuit.hpp"
#include "compiler/transpile.hpp"
#include "service/qiskit_runtime_service.hpp"

using namespace Qiskit::circuit;

int main() {
    // Build circuit
    QuantumRegister qr(3);
    ClassicalRegister cr(3);
    QuantumCircuit circ(qr, cr);

    circ.h(0);
    circ.cx(0, 1);
    circ.cx(1, 2);
    circ.measure(qr, cr);

    std::cout << "Original circuit:" << std::endl;
    circ.print();

    // Connect to IBM Quantum and transpile for a real backend
    auto service = Qiskit::service::QiskitRuntimeService();
    auto backend = service.backend("ibm_torino");
    auto transpiled = Qiskit::compiler::transpile(circ, backend);

    std::cout << "\nTranspiled circuit:" << std::endl;
    transpiled.print();

    return 0;
}
