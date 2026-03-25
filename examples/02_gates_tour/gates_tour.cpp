// Example 02: Gates Tour — Single-qubit, multi-qubit, and rotation gates.

#include "circuit/quantumcircuit.hpp"
#include <cmath>

using namespace Qiskit::circuit;

int main() {
    QuantumRegister qr(4);
    ClassicalRegister cr(4);
    QuantumCircuit circ(qr, cr);

    // Single-qubit gates
    circ.h(0);
    circ.x(1);
    circ.y(2);
    circ.z(3);
    circ.s(0);
    circ.t(1);

    // Rotation gates
    circ.rx(M_PI / 4, 0);
    circ.ry(M_PI / 3, 1);
    circ.rz(M_PI / 2, 2);

    // Multi-qubit gates
    circ.cx(0, 1);
    circ.cz(2, 3);
    circ.swap(0, 2);
    circ.ccx(0, 1, 2);

    circ.barrier({0, 1, 2, 3});
    circ.measure(qr, cr);

    circ.print();

    return 0;
}
