"""Example 05: Parameterized Circuit — Parameters and expressions."""

from qiskit import QuantumCircuit
from qiskit.circuit import Parameter

qc = QuantumCircuit(2, 2)

# Create symbolic parameters
theta = Parameter('theta')
phi = Parameter('phi')

# Use parameters directly in gates
qc.h(0)
qc.rx(theta, 0)
qc.ry(phi, 1)
qc.cx(0, 1)
qc.measure([0, 1], [0, 1])

print("Parameterized circuit:")
print(qc)
print(f"\nParameters: {qc.parameters}")

# Parameter expressions support arithmetic
a = theta + 0.5
b = phi * 2.0
print(f"\ntheta + 0.5 = {a}")
print(f"phi * 2.0   = {b}")
