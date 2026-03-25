// Example 08: Local Transpilation — Transpile against a custom target.

#include "circuit/quantumcircuit.hpp"
#include "transpiler/target.hpp"
#include "transpiler/staged_pass_manager.hpp"

using namespace Qiskit::circuit;
using namespace Qiskit::transpiler;

int main() {
    // Build a simple circuit
    QuantumRegister qr(4);
    ClassicalRegister cr(4);
    QuantumCircuit circ(qr, cr);

    circ.h(0);
    circ.cx(0, 1);
    circ.cx(1, 2);
    circ.cx(2, 3);
    circ.measure(qr, cr);

    std::cout << "Original circuit:" << std::endl;
    circ.print();

    // Define a local target: H and CX on a 4-qubit linear topology
    auto target = Target(
        {"h", "cx"},
        {{0, 1}, {1, 0}, {1, 2}, {2, 1}, {2, 3}, {3, 2}}
    );

    // Transpile using staged pass manager
    auto pass = StagedPassManager(
        {"init", "layout", "routing", "translation", "optimization"},
        target
    );
    auto transpiled = pass.run(circ);

    std::cout << "\nTranspiled circuit:" << std::endl;
    transpiled.print();

    return 0;
}
