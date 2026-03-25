// Example 04: GHZ Circuit — Scalable GHZ state with a loop.

#include "circuit/quantumcircuit.hpp"

using namespace Qiskit::circuit;

int main() {
    const int n = 5;
    QuantumRegister qr(n);
    ClassicalRegister cr(n);
    QuantumCircuit circ(qr, cr);

    circ.h(0);
    for (int i = 0; i < n - 1; i++) {
        circ.cx(i, i + 1);
    }

    circ.measure(qr, cr);
    circ.print();

    return 0;
}
