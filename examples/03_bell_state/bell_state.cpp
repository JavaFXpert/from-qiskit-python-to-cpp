// Example 03: Bell State — Create and measure a Bell pair.

#include "circuit/quantumcircuit.hpp"

using namespace Qiskit::circuit;

int main() {
    QuantumRegister qr(2);
    ClassicalRegister cr(2);
    QuantumCircuit circ(qr, cr);

    circ.h(0);
    circ.cx(0, 1);
    circ.measure(0, 0);
    circ.measure(1, 1);

    circ.print();
    std::cout << circ.to_qasm3() << std::endl;

    return 0;
}
