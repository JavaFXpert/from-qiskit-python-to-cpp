// Example 05: Parameterized Circuit — Parameters and expressions.

#include "circuit/quantumcircuit.hpp"
#include <cmath>

using namespace Qiskit::circuit;

int main() {
    QuantumRegister qr(2);
    ClassicalRegister cr(2);
    QuantumCircuit circ(qr, cr);

    // Create symbolic parameters
    Parameter theta("theta");
    Parameter phi("phi");

    // Use parameters directly in gates
    circ.h(0);
    circ.rx(theta, 0);
    circ.ry(phi, 1);
    circ.cx(0, 1);
    circ.measure(qr, cr);

    std::cout << "Parameterized circuit:" << std::endl;
    circ.print();

    // Parameter expressions support arithmetic
    Parameter a = theta + 0.5;
    Parameter b = phi * 2.0;
    std::cout << "\ntheta + 0.5 = " << a.as_str() << std::endl;
    std::cout << "phi * 2.0   = " << b.as_str() << std::endl;

    // Numeric parameters can be compared
    Parameter p1(M_PI / 2.0);
    Parameter p2(M_PI / 2.0);
    std::cout << "\npi/2 == pi/2? " << (p1 == p2 ? "yes" : "no") << std::endl;

    return 0;
}
