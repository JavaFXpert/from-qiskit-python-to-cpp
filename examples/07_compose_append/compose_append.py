"""Example 07: Circuit Composition — Compose and append sub-circuits."""

from qiskit import QuantumCircuit

# Main circuit
qc = QuantumCircuit(4, 4)
qc.h(0)
qc.cx(0, 1)

# Sub-circuit
sub = QuantumCircuit(2, 2)
sub.cz(0, 1)
sub.ry(1.57, 0)

# Compose sub-circuit onto qubits 2,3 and clbits 2,3
composed = qc.compose(sub, qubits=[2, 3], clbits=[2, 3])

composed.barrier()
composed.measure([0, 1, 2, 3], [0, 1, 2, 3])

print("Composed circuit:")
print(composed)
