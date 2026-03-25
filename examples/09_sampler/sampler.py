"""Example 10: Sampler — Full end-to-end: build, transpile, sample, process."""

from qiskit import QuantumCircuit
from qiskit.transpiler.preset_passmanagers import generate_preset_pass_manager
from qiskit_ibm_runtime import QiskitRuntimeService, SamplerV2

# Build a Bell state circuit
qc = QuantumCircuit(2, 2)
qc.h(0)
qc.cx(0, 1)
qc.measure([0, 1], [0, 1])

# Connect, transpile, and run
service = QiskitRuntimeService()
backend = service.backend("ibm_torino")
pm = generate_preset_pass_manager(optimization_level=2, backend=backend)
transpiled = pm.run(qc)

sampler = SamplerV2(backend)
job = sampler.run([transpiled], shots=100)
result = job.result()

# Process results
pub_result = result[0]
counts = pub_result.data.c.get_counts()

print("Measurement results:")
for bitstring, count in sorted(counts.items()):
    print(f"  {bitstring}: {count}")
