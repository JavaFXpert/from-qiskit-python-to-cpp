cmake_minimum_required(VERSION 3.11)
project(qiskit-cpp-workshop LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------------------------------------------------------------------
# Auto-discover Qiskit C API paths from the pip package (Qiskit >= 2.4)
# ---------------------------------------------------------------------------
execute_process(
  COMMAND python3 -c "import qiskit.capi; print(qiskit.capi.get_include())"
  OUTPUT_VARIABLE QISKIT_INCLUDE
  OUTPUT_STRIP_TRAILING_WHITESPACE
  RESULT_VARIABLE _qk_inc_rc)
execute_process(
  COMMAND python3 -c "import qiskit.capi; print(qiskit.capi.get_lib())"
  OUTPUT_VARIABLE QISKIT_LIB
  OUTPUT_STRIP_TRAILING_WHITESPACE
  RESULT_VARIABLE _qk_lib_rc)

if(_qk_inc_rc OR _qk_lib_rc)
  message(FATAL_ERROR
    "Could not locate Qiskit C API. "
    "Install with: python3 -m pip install \"qiskit>=2.4.0rc1\"")
endif()

get_filename_component(QISKIT_LIB_DIR "${QISKIT_LIB}" DIRECTORY)
message(STATUS "Qiskit C API include: ${QISKIT_INCLUDE}")
message(STATUS "Qiskit C API library: ${QISKIT_LIB}")

# ---------------------------------------------------------------------------
# qiskit-cpp header-only SDK
# ---------------------------------------------------------------------------
if(NOT QISKIT_CPP_SRC)
  set(QISKIT_CPP_SRC "${CMAKE_CURRENT_SOURCE_DIR}/../qiskit-cpp/src"
      CACHE PATH "Path to qiskit-cpp src/ directory")
endif()

if(NOT EXISTS "${QISKIT_CPP_SRC}/circuit/quantumcircuit.hpp")
  message(FATAL_ERROR
    "qiskit-cpp headers not found at ${QISKIT_CPP_SRC}. "
    "Clone with: git clone https://github.com/Qiskit/qiskit-cpp.git")
endif()
message(STATUS "qiskit-cpp headers: ${QISKIT_CPP_SRC}")

# ---------------------------------------------------------------------------
# Python library (needed at runtime by Qiskit's _accelerate.abi3.so)
# ---------------------------------------------------------------------------
find_package(Python3 COMPONENTS Development)
if(NOT Python3_FOUND)
  # Fallback: discover Python library path manually
  execute_process(
    COMMAND python3 -c "import sysconfig; print(sysconfig.get_config_var('LIBDIR'))"
    OUTPUT_VARIABLE _py_libdir OUTPUT_STRIP_TRAILING_WHITESPACE)
  execute_process(
    COMMAND python3 -c "import sysconfig; print(sysconfig.get_config_var('LDLIBRARY'))"
    OUTPUT_VARIABLE _py_ldlib OUTPUT_STRIP_TRAILING_WHITESPACE)
  find_library(PYTHON_LIB NAMES "${_py_ldlib}" python3 PATHS "${_py_libdir}" NO_DEFAULT_PATH)
  message(STATUS "Python library (fallback): ${PYTHON_LIB}")
else()
  set(PYTHON_LIB Python3::Python)
  message(STATUS "Python library: ${Python3_LIBRARIES}")
endif()

# ---------------------------------------------------------------------------
# Dependencies
# ---------------------------------------------------------------------------
find_package(Threads REQUIRED)

include(FetchContent)
FetchContent_Declare(json
  URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_MakeAvailable(json)

# ---------------------------------------------------------------------------
# Helper: add a workshop example
# ---------------------------------------------------------------------------
function(add_workshop_example NAME DIR)
  add_executable(${NAME} ${DIR}/${NAME}.cpp)
  target_include_directories(${NAME} PRIVATE
    ${QISKIT_INCLUDE}
    ${QISKIT_CPP_SRC})
  target_link_libraries(${NAME} PRIVATE
    "${QISKIT_LIB}"
    ${PYTHON_LIB}
    nlohmann_json::nlohmann_json
    Threads::Threads)
  set_target_properties(${NAME} PROPERTIES
    BUILD_RPATH "${QISKIT_LIB_DIR}"
    INSTALL_RPATH "${QISKIT_LIB_DIR}")
endfunction()

# ---------------------------------------------------------------------------
# Local examples (no hardware credentials required)
# ---------------------------------------------------------------------------
add_workshop_example(hello_circuit    01_hello_circuit)
add_workshop_example(gates_tour       02_gates_tour)
add_workshop_example(bell_state       03_bell_state)
add_workshop_example(ghz_circuit      04_ghz_circuit)
add_workshop_example(parameterized    05_parameterized)
add_workshop_example(observables      06_observables)
add_workshop_example(compose_append   07_compose_append)

# ---------------------------------------------------------------------------
# Hardware examples (require qiskit-ibm-runtime-c and credentials)
# ---------------------------------------------------------------------------
option(ENABLE_HARDWARE_EXAMPLES
  "Build examples that require IBM Quantum credentials" OFF)

if(ENABLE_HARDWARE_EXAMPLES)
  add_workshop_example(transpile 08_transpile)
  add_workshop_example(sampler   09_sampler)
endif()
