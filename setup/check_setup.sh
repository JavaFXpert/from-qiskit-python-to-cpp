#!/usr/bin/env bash
# check_setup.sh — Verify the workshop environment is ready.

set -e
OK="\033[0;32m✔\033[0m"
FAIL="\033[0;31m✘\033[0m"
errors=0

echo "=== From Qiskit Python to C++ — Environment Check ==="
echo

# 1. C++ compiler
if command -v g++ &>/dev/null; then
  echo -e "$OK  g++ found: $(g++ --version | head -1)"
elif command -v clang++ &>/dev/null; then
  echo -e "$OK  clang++ found: $(clang++ --version | head -1)"
else
  echo -e "$FAIL  No C++ compiler found (need g++ or clang++)"
  errors=$((errors + 1))
fi

# 2. CMake
if command -v cmake &>/dev/null; then
  echo -e "$OK  cmake found: $(cmake --version | head -1)"
else
  echo -e "$FAIL  cmake not found (need 3.11+)"
  errors=$((errors + 1))
fi

# 3. Python
if command -v python3 &>/dev/null; then
  echo -e "$OK  python3 found: $(python3 --version)"
else
  echo -e "$FAIL  python3 not found"
  errors=$((errors + 1))
fi

# 4. Qiskit version
QISKIT_VER=$(python3 -c "import qiskit; print(qiskit.__version__)" 2>/dev/null || true)
if [ -n "$QISKIT_VER" ]; then
  echo -e "$OK  qiskit $QISKIT_VER"
else
  echo -e "$FAIL  qiskit not installed (python3 -m pip install "qiskit>=2.4.0rc1")"
  errors=$((errors + 1))
fi

# 5. Qiskit C API paths
QISKIT_INC=$(python3 -c "import qiskit.capi; print(qiskit.capi.get_include())" 2>/dev/null || true)
QISKIT_LIB=$(python3 -c "import qiskit.capi; print(qiskit.capi.get_lib())" 2>/dev/null || true)
if [ -n "$QISKIT_INC" ] && [ -d "$QISKIT_INC" ]; then
  echo -e "$OK  Qiskit C header: $QISKIT_INC"
else
  echo -e "$FAIL  Qiskit C header not found (need qiskit >= 2.4)"
  errors=$((errors + 1))
fi
if [ -n "$QISKIT_LIB" ] && [ -f "$QISKIT_LIB" ]; then
  echo -e "$OK  Qiskit C library: $QISKIT_LIB"
else
  echo -e "$FAIL  Qiskit C library not found"
  errors=$((errors + 1))
fi

# 6. qiskit-cpp headers
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
QISKIT_CPP_SRC="${QISKIT_CPP_SRC:-$SCRIPT_DIR/../qiskit-cpp/src}"
if [ -f "$QISKIT_CPP_SRC/circuit/quantumcircuit.hpp" ]; then
  echo -e "$OK  qiskit-cpp headers: $QISKIT_CPP_SRC"
else
  echo -e "$FAIL  qiskit-cpp headers not found at $QISKIT_CPP_SRC"
  echo "       Clone with: git clone https://github.com/Qiskit/qiskit-cpp.git"
  errors=$((errors + 1))
fi

echo
if [ $errors -eq 0 ]; then
  echo "All checks passed. Ready to build!"
else
  echo "$errors check(s) failed. Please fix and re-run."
  exit 1
fi
