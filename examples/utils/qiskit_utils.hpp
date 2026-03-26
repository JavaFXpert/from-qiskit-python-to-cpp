// Utility functions for Qiskit C++ workshop examples.
// Header-only — include from any example.

#ifndef QISKIT_UTILS_HPP
#define QISKIT_UTILS_HPP

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <iomanip>

// ---------------------------------------------------------------------------
// counts_to_probabilities
// Convert raw measurement counts to probabilities (0.0 – 1.0).
// ---------------------------------------------------------------------------
inline std::unordered_map<std::string, double> counts_to_probabilities(
        const std::unordered_map<std::string, unsigned long long>& counts) {
    unsigned long long total = 0;
    for (auto& kv : counts) total += kv.second;

    std::unordered_map<std::string, double> probs;
    if (total > 0) {
        for (auto& kv : counts) {
            probs[kv.first] = static_cast<double>(kv.second) / total;
        }
    }
    return probs;
}

// ---------------------------------------------------------------------------
// print_histogram
// Display a vertical bar chart of measurement counts in the terminal.
//   counts     – measurement results from get_counts()
//   num_qubits – number of qubits (determines how many basis states to show)
//   height     – maximum bar height in rows
//   col_width  – character width of each column
// ---------------------------------------------------------------------------
namespace detail {
    inline std::string center(const std::string& text, int width) {
        int pad = width - static_cast<int>(text.size());
        if (pad <= 0) return text;
        int left = pad / 2;
        int right = pad - left;
        return std::string(left, ' ') + text + std::string(right, ' ');
    }
}

inline void print_histogram(
        const std::unordered_map<std::string, unsigned long long>& counts,
        int num_qubits, int height = 16, int col_width = 7) {
    int num_states = 1 << num_qubits;

    // Build ordered list of states and their counts
    std::vector<std::string> states;
    std::vector<unsigned long long> values;
    unsigned long long max_count = 0;
    for (int i = 0; i < num_states; i++) {
        std::string state;
        for (int b = num_qubits - 1; b >= 0; b--) {
            state += ((i >> b) & 1) ? '1' : '0';
        }
        states.push_back(state);
        auto it = counts.find(state);
        unsigned long long count = (it != counts.end()) ? it->second : 0;
        values.push_back(count);
        if (count > max_count) max_count = count;
    }

    // Compute scaled bar heights (minimum 1 if count > 0)
    std::vector<int> bar_heights(num_states);
    for (int col = 0; col < num_states; col++) {
        if (values[col] == 0) {
            bar_heights[col] = 0;
        } else {
            int h = static_cast<int>(values[col] * height / max_count);
            bar_heights[col] = (h < 1) ? 1 : h;
        }
    }

    // Build the bar block string
    std::string block_mid = " ";
    for (int k = 0; k < col_width - 2; k++) block_mid += "\xe2\x96\x88";
    block_mid += " ";
    std::string blank(col_width, ' ');

    // Draw rows from top to bottom
    for (int row = height; row >= 1; row--) {
        for (int col = 0; col < num_states; col++) {
            if (bar_heights[col] == row) {
                std::cout << detail::center(std::to_string(values[col]), col_width);
            } else if (bar_heights[col] > row) {
                std::cout << block_mid;
            } else if (bar_heights[col] == 0 && row == 1) {
                std::cout << detail::center(std::to_string(values[col]), col_width);
            } else {
                std::cout << blank;
            }
        }
        std::cout << "\n";
    }

    // Baseline
    for (int col = 0; col < num_states * col_width; col++) {
        std::cout << "\xe2\x94\x80";
    }
    std::cout << "\n";

    // State labels centered
    for (int col = 0; col < num_states; col++) {
        std::cout << detail::center(states[col], col_width);
    }
    std::cout << std::endl;
}

// ---------------------------------------------------------------------------
// print_probabilities
// Print counts as probabilities, showing all basis states in order.
// ---------------------------------------------------------------------------
inline void print_probabilities(
        const std::unordered_map<std::string, unsigned long long>& counts,
        int num_qubits) {
    auto probs = counts_to_probabilities(counts);
    int num_states = 1 << num_qubits;

    std::cout << std::fixed << std::setprecision(4);
    for (int i = 0; i < num_states; i++) {
        std::string state;
        for (int b = num_qubits - 1; b >= 0; b--) {
            state += ((i >> b) & 1) ? '1' : '0';
        }
        auto it = probs.find(state);
        double p = (it != probs.end()) ? it->second : 0.0;
        std::cout << "  " << state << ": " << p << std::endl;
    }
}

#endif // QISKIT_UTILS_HPP
