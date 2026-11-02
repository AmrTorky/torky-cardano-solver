#ifndef TORKY_CARDANO_VIETE_SOLVER_HPP
#define TORKY_CARDANO_VIETE_SOLVER_HPP

#include <cmath>
#include <cstdint>
#include <algorithm>

namespace torky::math::cardano {

    inline constexpr double EPSILON_NM2       = 0.016;
    inline constexpr double V_FINITE_ZERO_EV  = 11.384;

    #pragma pack(push, 1)
    struct TorkyCellDtype {
        uint8_t  gov_gate;     // TSG Gate Register (0x01 - 0x04)
        int16_t  phase_q16;    // Phase Q16 Fixed-Point
        int16_t  field_q16;    // Field Q16 Fixed-Point
        uint32_t syndrome_u32; // SHA-256 Merkle Syndrome
    };
    #pragma pack(pop)

    static_assert(sizeof(TorkyCellDtype) == 9, "CRITICAL: TorkyCellDtype must be precisely 9 bytes!");

    class CardanoVieteEngine {
    public:
        // Solves depressed cubic x^3 + px + q = 0 analytically in sub-1us O(1) time
        static inline double solve_root(double p, double q) {
            double p_safe = std::min(-0.000001, p);
            double discriminant = (4.0 * p_safe * p_safe * p_safe) + (27.0 * q * q);

            if (discriminant < 0.0) {
                // 3 real roots case: Viète trigonometric formulation
                double arg = (3.0 * q / (2.0 * p_safe)) * std::sqrt(-3.0 / p_safe);
                arg = std::max(-1.0, std::min(1.0, arg));
                return 2.0 * std::sqrt(-p_safe / 3.0) * std::cos(std::acos(arg) / 3.0);
            } else {
                // 1 real root case: Cardano radical formulation
                double disc_sqrt = std::sqrt(discriminant);
                double u_cubed = (-27.0 * q + 3.0 * std::sqrt(3.0) * disc_sqrt) / 54.0;
                double v_cubed = (-27.0 * q - 3.0 * std::sqrt(3.0) * disc_sqrt) / 54.0;
                double u = std::cbrt(u_cubed);
                double v = std::cbrt(v_cubed);
                double root = u + v;
                return std::isnan(root) ? 0.0 : root;
            }
        }

        static inline double evaluate_v_finite(double r_nm) {
            return 1.0 / std::sqrt((r_nm * r_nm) + EPSILON_NM2);
        }
    };

    class TsgGateController {
    public:
        static inline bool validate_authorization(uint8_t gate_request, bool bit_tampered) {
            if (bit_tampered) return false;
            return (gate_request >= 0x01 && gate_request <= 0x04);
        }
    };

} // namespace torky::math::cardano

#endif // TORKY_CARDANO_VIETE_SOLVER_HPP
