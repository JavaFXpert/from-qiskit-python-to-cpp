"""Example 05: Parameterized Circuit — Parameters and binding."""

from qiskit import QuantumCircuit
from qiskit.circuit import Parameter
import math

qc = QuantumCircuit(2, 2)

theta = Parameter('theta')
phi = Parameter('phi')

qc.h(0)
qc.rx(theta, 0)
qc.ry(phi, 1)
qc.cx(0, 1)
qc.measure([0, 1], [0, 1])

print("Parameterized circuit:")
print(qc)
print(f"\nParameters: {qc.parameters}")

# Bind parameters
bound = qc.assign_parameters({theta: math.pi / 4, phi: math.pi / 3})
print("\nBound circuit:")
print(bound)
