"""Example 09: Backend Transpilation — Transpile for a real IBM backend."""

from qiskit import QuantumCircuit
from qiskit.compiler import transpile
from qiskit_ibm_runtime import QiskitRuntimeService

# Build circuit
qc = QuantumCircuit(3, 3)
qc.h(0)
qc.cx(0, 1)
qc.cx(1, 2)
qc.measure([0, 1, 2], [0, 1, 2])

print("Original circuit:")
print(qc)

# Connect to IBM Quantum and transpile for a real backend
service = QiskitRuntimeService()
backend = service.backend("ibm_torino")
transpiled = transpile(qc, backend, optimization_level=2)

print(f"\nTranspiled for {backend.name}:")
print(transpiled)
