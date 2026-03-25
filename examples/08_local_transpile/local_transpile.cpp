// Example 08: Local Transpilation — Transpile against a custom target.

#include "circuit/quantumcircuit.hpp"
#include "transpiler/preset_passmanagers/generate_preset_pass_manager.hpp"

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

    // Transpile for a 4-qubit linear topology with H and CX gates
    auto pm = generate_preset_pass_manager(
        2,  // optimization_level
        std::vector<std::string>{"h", "cx"},
        std::vector<std::pair<uint32_t, uint32_t>>{
            {0, 1}, {1, 0}, {1, 2}, {2, 1}, {2, 3}, {3, 2}
        }
    );
    auto transpiled = pm.run(circ);

    std::cout << "\nTranspiled circuit:" << std::endl;
    transpiled.print();

    return 0;
}
