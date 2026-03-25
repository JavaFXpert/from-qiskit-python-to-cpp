// Example 05: Parameterized Circuit — Parameters and binding.

#include "circuit/quantumcircuit.hpp"
#include <cmath>

using namespace Qiskit::circuit;

int main() {
    QuantumRegister qr(2);
    ClassicalRegister cr(2);
    QuantumCircuit circ(qr, cr);

    Parameter theta("theta");
    Parameter phi("phi");

    circ.h(0);
    circ.rx(theta, 0);
    circ.ry(phi, 1);
    circ.cx(0, 1);
    circ.measure(qr, cr);

    std::cout << "Parameterized circuit:" << std::endl;
    circ.print();

    // Bind parameters
    circ.assign_parameters({"theta", "phi"}, {M_PI / 4, M_PI / 3});

    std::cout << "\nBound circuit:" << std::endl;
    circ.print();

    return 0;
}
