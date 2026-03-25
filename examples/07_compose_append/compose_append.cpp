// Example 07: Circuit Composition — Compose and append sub-circuits.

#include "circuit/quantumcircuit.hpp"

using namespace Qiskit::circuit;

int main() {
    // Main circuit
    QuantumRegister qr(4);
    ClassicalRegister cr(4);
    QuantumCircuit circ(qr, cr);

    circ.h(0);
    circ.cx(0, 1);

    // Sub-circuit
    QuantumRegister sub_qr(2);
    ClassicalRegister sub_cr(2);
    QuantumCircuit sub(sub_qr, sub_cr);

    sub.cz(0, 1);
    sub.ry(1.57, 0);

    // Compose sub-circuit onto qubits 2,3 and clbits 2,3
    circ.compose(sub, {2, 3}, {2, 3});

    circ.barrier({0, 1, 2, 3});
    circ.measure(qr, cr);

    std::cout << "Composed circuit:" << std::endl;
    circ.print();

    return 0;
}
