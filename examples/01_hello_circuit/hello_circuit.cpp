// Example 01: Hello Circuit — Create a simple 2-qubit circuit.

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
    std::cout << std::endl;
    std::cout << circ.to_qasm3() << std::endl;

    return 0;
}
