"""Example 04: GHZ Circuit — Scalable GHZ state with a loop."""

from qiskit import QuantumCircuit

n = 5
qc = QuantumCircuit(n, n)

qc.h(0)
for i in range(n - 1):
    qc.cx(i, i + 1)

qc.measure(range(n), range(n))

print(qc)
