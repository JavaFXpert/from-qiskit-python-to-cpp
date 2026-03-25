"""Example 06: Observables — Building quantum observables."""

from qiskit.quantum_info import SparsePauliOp

# Build from label
obs1 = SparsePauliOp.from_list([("IIXY", 1.0), ("ZZII", -0.5)])
print("Observable from list:")
print(obs1)

# Identity and zero
identity = SparsePauliOp.from_list([("IIII", 1.0)])
zero = SparsePauliOp.from_list([("IIII", 0.0)])
print(f"\nIdentity:\n{identity}")

# Arithmetic
combined = obs1 + identity
print(f"\nobs1 + identity:\n{combined}")
