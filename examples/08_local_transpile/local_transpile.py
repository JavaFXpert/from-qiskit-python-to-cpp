"""Example 08: Local Transpilation — Transpile against a custom target."""

from qiskit import QuantumCircuit
from qiskit.transpiler.preset_passmanagers import generate_preset_pass_manager

# Build a simple circuit
qc = QuantumCircuit(4, 4)
qc.h(0)
qc.cx(0, 1)
qc.cx(1, 2)
qc.cx(2, 3)
qc.measure([0, 1, 2, 3], [0, 1, 2, 3])

print("Original circuit:")
print(qc)

# Transpile for a 4-qubit linear topology with H and CX gates
pm = generate_preset_pass_manager(
    optimization_level=2,
    basis_gates=["h", "cx"],
    coupling_map=[(0, 1), (1, 0), (1, 2), (2, 1), (2, 3), (3, 2)],
)
transpiled = pm.run(qc)

print("\nTranspiled circuit:")
print(transpiled)
