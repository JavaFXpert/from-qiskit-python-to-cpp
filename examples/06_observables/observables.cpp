// Example 06: Observables — Building quantum observables.

#include "quantum_info/sparse_observable.hpp"
#include <complex>
#include <iostream>
#include <vector>
#include <utility>

using namespace Qiskit::quantum_info;

int main() {
    // Build from list
    std::vector<std::pair<std::string, std::complex<double>>> terms;
    terms.push_back(std::make_pair("IIXY", std::complex<double>(1.0, 0.0)));
    terms.push_back(std::make_pair("ZZII", std::complex<double>(-0.5, 0.0)));
    auto obs1 = SparseObservable::from_list(terms);
    std::cout << "Observable from list:" << std::endl;

    // Identity and zero
    auto identity = SparseObservable::identity(4);
    auto zero = SparseObservable::zero(4);
    std::cout << "Created identity and zero observables" << std::endl;

    // Arithmetic
    auto combined = obs1;
    combined += identity;
    std::cout << "Combined obs1 + identity" << std::endl;

    return 0;
}
