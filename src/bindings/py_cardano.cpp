#include "torky_continuum_core.hpp"

extern "C" {
    void solve_cardano_c(double p, double q, double* out_roots, double* out_residual, double* out_latency_us) {
        auto res = torky::continuum::solve_cardano_viete(p, q);
        out_roots[0] = res.roots[0];
        out_roots[1] = res.roots[1];
        out_roots[2] = res.roots[2];
        *out_residual = res.residual_kcal_mol;
        *out_latency_us = res.latency_us;
    }
}
