"""Example 08: Local Transpilation — Transpile against a custom target."""

from qiskit import QuantumCircuit
from qiskit.transpiler import Target, InstructionProperties
from qiskit.circuit.library import HGate, CXGate
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

# Define a local target: H and CX on a 4-qubit linear topology
target = Target(num_qubits=4)
target.add_instruction(HGate(), {(i,): InstructionProperties() for i in range(4)})
target.add_instruction(CXGate(), {
    (0, 1): InstructionProperties(),
    (1, 0): InstructionProperties(),
    (1, 2): InstructionProperties(),
    (2, 1): InstructionProperties(),
    (2, 3): InstructionProperties(),
    (3, 2): InstructionProperties(),
})

pm = generate_preset_pass_manager(optimization_level=2, target=target)
transpiled = pm.run(qc)

print("\nTranspiled circuit:")
print(transpiled)
