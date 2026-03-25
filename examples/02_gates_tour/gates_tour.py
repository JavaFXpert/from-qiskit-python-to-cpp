"""Example 02: Gates Tour — Single-qubit, multi-qubit, and rotation gates."""

from qiskit import QuantumCircuit
import math

qc = QuantumCircuit(4, 4)

# Single-qubit gates
qc.h(0)
qc.x(1)
qc.y(2)
qc.z(3)
qc.s(0)
qc.t(1)

# Rotation gates
qc.rx(math.pi / 4, 0)
qc.ry(math.pi / 3, 1)
qc.rz(math.pi / 2, 2)

# Multi-qubit gates
qc.cx(0, 1)
qc.cz(2, 3)
qc.swap(0, 2)
qc.ccx(0, 1, 2)

qc.barrier()
qc.measure([0, 1, 2, 3], [0, 1, 2, 3])

print(qc)
