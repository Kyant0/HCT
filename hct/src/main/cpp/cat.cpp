// https://doi.org/10.1364/OE.26.007724, Qiyan Zhai and Ming R. Luo, "Study of chromatic adaptation via neutral white matches on different viewing media," Opt. Express 26, 7724-7739 (2018)

#include "cat.h"

constexpr num M_CAT02[3][3] = {
    {0.7328, 0.4296, -0.1624},
    {-0.7036, 1.6975, 0.0061},
    {0.0030, 0.0136, 0.9834},
};
constexpr num M_CAT02_INV[3][3] = {
    {1.096123820835514159, -0.278869000218287244, 0.182745179382773085},
    {0.454369041975359164, 0.473533154307411715, 0.072097803717229120},
    {-0.009627608738429354, -0.005698031216113420, 1.015325639954542775}
};

constexpr num M_CAT16[3][3] = {
    {0.401288, 0.650173, -0.051461},
    {-0.250268, 1.204414, 0.045854},
    {-0.002079, 0.048952, 0.953127},
};
constexpr num M_CAT16_INV[3][3] = {
    {1.862067855087233, -1.0112546305316843, 0.14918677544445177},
    {0.3875265432361371, 0.6214474419314752, -0.008973985167612516},
    {-0.015841498849333856, -0.03412293802851556, 1.0499644368778496},
};

CieXyzAbs cat02(
    const CieXyzAbs &xyz,
    const CieXyzAbs &source,
    const num degreeOfAdaptationForSource,
    const CieXyzAbs &target,
    const num degreeOfAdaptationForTarget,
    const CieXyzAbs &baseline
) {
    const auto [x_beta, y_beta, z_beta] = xyz;
    const auto [x_w_beta, y_w_beta, z_w_beta] = source;
    const num d_beta = degreeOfAdaptationForSource;
    const auto [x_w_sigma, y_w_sigma, z_w_sigma] = target;
    const num d_sigma = degreeOfAdaptationForTarget;
    const auto [x_w_o, y_w_o, z_w_o] = baseline;

    // Step 1. Calculate the cone-like responses
    const num r_beta = M_CAT02[0][0] * x_beta + M_CAT02[0][1] * y_beta + M_CAT02[0][2] * z_beta;
    const num g_beta = M_CAT02[1][0] * x_beta + M_CAT02[1][1] * y_beta + M_CAT02[1][2] * z_beta;
    const num b_beta = M_CAT02[2][0] * x_beta + M_CAT02[2][1] * y_beta + M_CAT02[2][2] * z_beta;

    const num r_w_beta = M_CAT02[0][0] * x_w_beta + M_CAT02[0][1] * y_w_beta + M_CAT02[0][2] * z_w_beta;
    const num g_w_beta = M_CAT02[1][0] * x_w_beta + M_CAT02[1][1] * y_w_beta + M_CAT02[1][2] * z_w_beta;
    const num b_w_beta = M_CAT02[2][0] * x_w_beta + M_CAT02[2][1] * y_w_beta + M_CAT02[2][2] * z_w_beta;

    const num r_w_sigma = M_CAT02[0][0] * x_w_sigma + M_CAT02[0][1] * y_w_sigma + M_CAT02[0][2] * z_w_sigma;
    const num g_w_sigma = M_CAT02[1][0] * x_w_sigma + M_CAT02[1][1] * y_w_sigma + M_CAT02[1][2] * z_w_sigma;
    const num b_w_sigma = M_CAT02[2][0] * x_w_sigma + M_CAT02[2][1] * y_w_sigma + M_CAT02[2][2] * z_w_sigma;

    const num r_w_o = M_CAT02[0][0] * x_w_o + M_CAT02[0][1] * y_w_o + M_CAT02[0][2] * z_w_o;
    const num g_w_o = M_CAT02[1][0] * x_w_o + M_CAT02[1][1] * y_w_o + M_CAT02[1][2] * z_w_o;
    const num b_w_o = M_CAT02[2][0] * x_w_o + M_CAT02[2][1] * y_w_o + M_CAT02[2][2] * z_w_o;

    // Step 2. Calculate the scaling factors for each channel
    const num d_r_beta = d_beta * y_w_beta / y_w_o * r_w_o / r_w_beta + 1.0 - d_beta;
    const num d_g_beta = d_beta * y_w_beta / y_w_o * g_w_o / g_w_beta + 1.0 - d_beta;
    const num d_b_beta = d_beta * y_w_beta / y_w_o * b_w_o / b_w_beta + 1.0 - d_beta;

    const num d_r_sigma = d_sigma * y_w_sigma / y_w_o * r_w_o / r_w_sigma + 1.0 - d_sigma;
    const num d_g_sigma = d_sigma * y_w_sigma / y_w_o * g_w_o / g_w_sigma + 1.0 - d_sigma;
    const num d_b_sigma = d_sigma * y_w_sigma / y_w_o * b_w_o / b_w_sigma + 1.0 - d_sigma;

    const num d_r = d_r_beta / d_r_sigma;
    const num d_g = d_g_beta / d_g_sigma;
    const num d_b = d_b_beta / d_b_sigma;

    // Step 3. Calculate the adaptations in cone-like space for each channel
    const num r_sigma = d_r * r_beta;
    const num g_sigma = d_g * g_beta;
    const num b_sigma = d_b * b_beta;

    // Step 4. Calculate the corresponding color under the output illuminant
    const num x_sigma = M_CAT02_INV[0][0] * r_sigma + M_CAT02_INV[0][1] * g_sigma + M_CAT02_INV[0][2] * b_sigma;
    const num y_sigma = M_CAT02_INV[1][0] * r_sigma + M_CAT02_INV[1][1] * g_sigma + M_CAT02_INV[1][2] * b_sigma;
    const num z_sigma = M_CAT02_INV[2][0] * r_sigma + M_CAT02_INV[2][1] * g_sigma + M_CAT02_INV[2][2] * b_sigma;

    return {x_sigma, y_sigma, z_sigma};
}

CieXyzAbs cat16(
    const CieXyzAbs &xyz,
    const CieXyzAbs &source,
    const num degreeOfAdaptationForSource,
    const CieXyzAbs &target,
    const num degreeOfAdaptationForTarget,
    const CieXyzAbs &baseline
) {
    const auto [x_beta, y_beta, z_beta] = xyz;
    const auto [x_w_beta, y_w_beta, z_w_beta] = source;
    const num d_beta = degreeOfAdaptationForSource;
    const auto [x_w_sigma, y_w_sigma, z_w_sigma] = target;
    const num d_sigma = degreeOfAdaptationForTarget;
    const auto [x_w_o, y_w_o, z_w_o] = baseline;

    // Step 1. Calculate the cone-like responses
    const num r_beta = M_CAT16[0][0] * x_beta + M_CAT16[0][1] * y_beta + M_CAT16[0][2] * z_beta;
    const num g_beta = M_CAT16[1][0] * x_beta + M_CAT16[1][1] * y_beta + M_CAT16[1][2] * z_beta;
    const num b_beta = M_CAT16[2][0] * x_beta + M_CAT16[2][1] * y_beta + M_CAT16[2][2] * z_beta;

    const num r_w_beta = M_CAT16[0][0] * x_w_beta + M_CAT16[0][1] * y_w_beta + M_CAT16[0][2] * z_w_beta;
    const num g_w_beta = M_CAT16[1][0] * x_w_beta + M_CAT16[1][1] * y_w_beta + M_CAT16[1][2] * z_w_beta;
    const num b_w_beta = M_CAT16[2][0] * x_w_beta + M_CAT16[2][1] * y_w_beta + M_CAT16[2][2] * z_w_beta;

    const num r_w_sigma = M_CAT16[0][0] * x_w_sigma + M_CAT16[0][1] * y_w_sigma + M_CAT16[0][2] * z_w_sigma;
    const num g_w_sigma = M_CAT16[1][0] * x_w_sigma + M_CAT16[1][1] * y_w_sigma + M_CAT16[1][2] * z_w_sigma;
    const num b_w_sigma = M_CAT16[2][0] * x_w_sigma + M_CAT16[2][1] * y_w_sigma + M_CAT16[2][2] * z_w_sigma;

    const num r_w_o = M_CAT16[0][0] * x_w_o + M_CAT16[0][1] * y_w_o + M_CAT16[0][2] * z_w_o;
    const num g_w_o = M_CAT16[1][0] * x_w_o + M_CAT16[1][1] * y_w_o + M_CAT16[1][2] * z_w_o;
    const num b_w_o = M_CAT16[2][0] * x_w_o + M_CAT16[2][1] * y_w_o + M_CAT16[2][2] * z_w_o;

    // Step 2. Calculate the scaling factors for each channel
    const num d_r_beta = d_beta * y_w_beta / y_w_o * r_w_o / r_w_beta + 1.0 - d_beta;
    const num d_g_beta = d_beta * y_w_beta / y_w_o * g_w_o / g_w_beta + 1.0 - d_beta;
    const num d_b_beta = d_beta * y_w_beta / y_w_o * b_w_o / b_w_beta + 1.0 - d_beta;

    const num d_r_sigma = d_sigma * y_w_sigma / y_w_o * r_w_o / r_w_sigma + 1.0 - d_sigma;
    const num d_g_sigma = d_sigma * y_w_sigma / y_w_o * g_w_o / g_w_sigma + 1.0 - d_sigma;
    const num d_b_sigma = d_sigma * y_w_sigma / y_w_o * b_w_o / b_w_sigma + 1.0 - d_sigma;

    const num d_r = d_r_beta / d_r_sigma;
    const num d_g = d_g_beta / d_g_sigma;
    const num d_b = d_b_beta / d_b_sigma;

    // Step 3. Calculate the adaptations in cone-like space for each channel
    const num r_sigma = d_r * r_beta;
    const num g_sigma = d_g * g_beta;
    const num b_sigma = d_b * b_beta;

    // Step 4. Calculate the corresponding color under the output illuminant
    const num x_sigma = M_CAT16_INV[0][0] * r_sigma + M_CAT16_INV[0][1] * g_sigma + M_CAT16_INV[0][2] * b_sigma;
    const num y_sigma = M_CAT16_INV[1][0] * r_sigma + M_CAT16_INV[1][1] * g_sigma + M_CAT16_INV[1][2] * b_sigma;
    const num z_sigma = M_CAT16_INV[2][0] * r_sigma + M_CAT16_INV[2][1] * g_sigma + M_CAT16_INV[2][2] * b_sigma;

    return {x_sigma, y_sigma, z_sigma};
}
