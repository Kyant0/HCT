#include "hct.h"

#include "kcam.h"
#include "srgb.h"

constexpr num HCT_EPSILON = 1E-3;
constexpr num CHROMA_EPSILON = 0.1;

constexpr LinSrgb UNDEFINED_LIN_SRGB = {NAN, NAN, NAN};
constexpr Srgb BLACK = {0.0, 0.0, 0.0};
constexpr Srgb WHITE = {1.0, 1.0, 1.0};

/**
 * approximate lower bounds of max chroma for all hues,
 * and approximate upper bounds of max chroma for all hues,
 * for each tone from 0 to 100, in P3 color space.
 */
constexpr float CHROMA_LIMITS[202] = {
    0.0, 0.0,
    2.351562, 9.296875,
    3.226562, 11.867187,
    3.773438, 13.289063,
    4.265625, 14.546875,
    4.648437, 15.421875,
    4.976562, 16.351563,
    5.25, 16.953125,
    5.523437, 17.5,
    5.796875, 17.992187,
    6.015625, 18.757813,
    6.234375, 19.195313,
    6.507812, 19.6875,
    6.726562, 20.125,
    6.945312, 20.5625,
    7.164062, 21.21875,
    7.382812, 21.601562,
    7.601562, 22.039062,
    7.820312, 22.421875,
    8.039062, 22.804688,
    8.203125, 23.132812,
    8.421875, 23.789062,
    8.640625, 24.117188,
    8.804687, 24.445313,
    9.023437, 24.773438,
    9.1875, 25.101562,
    9.40625, 25.429688,
    9.570312, 25.703125,
    9.734375, 26.03125,
    9.953125, 26.578125,
    10.117187, 26.851563,
    10.28125, 27.125,
    10.445312, 27.398437,
    10.609375, 27.671875,
    10.773437, 27.945313,
    10.9375, 27.671875,
    11.101562, 27.5625,
    11.265625, 27.289062,
    11.429687, 27.015625,
    11.59375, 26.851562,
    11.703125, 26.578125,
    11.867187, 26.304688,
    12.03125, 26.140625,
    12.140625, 25.8125,
    12.304687, 25.539063,
    12.414062, 25.320312,
    12.523437, 24.992188,
    12.632812, 24.828125,
    12.796875, 24.5,
    12.90625, 24.28125,
    13.015625, 23.953125,
    13.125, 23.734375,
    13.234375, 23.515625,
    13.34375, 23.679688,
    13.453125, 23.84375,
    13.5625, 24.0625,
    13.671875, 23.625,
    13.78125, 23.242188,
    13.890625, 23.1875,
    14.0, 23.242188,
    14.109375, 23.242188,
    14.21875, 23.296875,
    14.328125, 23.351562,
    14.382812, 22.96875,
    14.492187, 22.476562,
    14.601562, 22.53125,
    14.710937, 22.695312,
    14.765625, 22.859375,
    14.875, 23.023438,
    14.929687, 23.1875,
    15.039062, 23.351563,
    14.65625, 23.515625,
    14.273437, 23.679687,
    13.835937, 23.789063,
    13.34375, 23.953125,
    12.851562, 24.117188,
    12.414062, 24.226562,
    11.921875, 24.390625,
    11.429687, 24.5,
    10.992187, 24.664063,
    10.5, 24.773438,
    10.0625, 24.9375,
    9.570312, 25.046875,
    9.132812, 25.210938,
    8.640625, 25.320312,
    8.203125, 25.484375,
    7.710937, 25.59375,
    7.273437, 25.046875,
    6.78125, 24.171875,
    6.289062, 23.1875,
    5.851562, 22.476562,
    5.359375, 21.765625,
    4.867187, 21.054687,
    4.375, 20.5625,
    3.882812, 20.070312,
    3.335937, 19.632813,
    2.789062, 19.195312,
    2.242187, 16.789063,
    1.640625, 10.609375,
    0.929687, 5.46875,
    0.169392, 0.169392,
};

RgbColorSpace hctRgbColorSpace;

void initHctCond(const RgbColorSpace rgbColorSpace) {
    hctRgbColorSpace = rgbColorSpace;
}

inline LinSrgb kcamToLinSrgb(const KCAM::Kcam &kcam, const num epsilon, const RgbColorSpace rgbColorSpace) {
    const CieXyz xyz = cieXyzAbsToCieXyz(kcamToCieXyzAbs(kcam, epsilon), LUMINANCE);
    if (xyz.x > D65_SRGB.x || xyz.y > D65_SRGB.y || xyz.z > D65_SRGB.z ||
        xyz.x < 0.0 || xyz.y < 0.0 || xyz.z < 0.0) {
        return UNDEFINED_LIN_SRGB;
    }
    return cieXyzToLinSrgb(xyz, rgbColorSpace);
}

inline bool isInGamut(const LinSrgb &srgb) {
    return srgb.r >= 0.0 && srgb.r <= 1.0 && srgb.g >= 0.0 && srgb.g <= 1.0 && srgb.b >= 0.0 && srgb.b <= 1.0;
}

inline num lerp(const num start, const num end, const num fraction) {
    return start + fraction * (end - start);
}

Srgb hctToSrgb(const Hct &hct) {
    if (hct.t <= HCT_EPSILON) return BLACK;
    if (hct.t >= 100.0 - HCT_EPSILON) return WHITE;

    const KCAM::Kcam kcam = {hct.t, min(hct.c, 28.0), hct.h};
    if (LinSrgb result = kcamToLinSrgb(kcam, HCT_EPSILON, hctRgbColorSpace); isInGamut(result)) {
        return linSrgbToSrgb(result);
    }

    KCAM::Kcam kcamTmp = kcam;
    LinSrgb result{};
    const int tone = static_cast<int>(hct.t);
    const num fraction = hct.t - tone;
    const float *chromas = &CHROMA_LIMITS[tone * 2];
    const float chromaLow1 = chromas[0];
    const float chromaHigh1 = chromas[1];
    const float chromaLow2 = chromas[2];
    const float chromaHigh2 = chromas[3];
    num cLow = lerp(chromaLow1, chromaLow2, fraction) - 1.0;
    num cHigh = lerp(chromaHigh1, chromaHigh2, fraction) + 1.0;

    do {
        const num cMid = (cLow + cHigh) / 2.0;
        kcamTmp.C = cMid;

        if (const LinSrgb clippedLinSrgb = kcamToLinSrgb(kcamTmp, HCT_EPSILON, hctRgbColorSpace);
            !isInGamut(clippedLinSrgb)) {
            cHigh = cMid;
        } else {
            cLow = cMid;
            result = clippedLinSrgb;
        }
    } while (abs(cHigh - cLow) > CHROMA_EPSILON);

    return linSrgbToSrgb(result);
}

Hct srgbToHct(const Srgb &srgb) {
    const CieXyz xyz = linSrgbToCieXyz(srgbToLinSrgb(srgb), hctRgbColorSpace);
    const auto [J, C, h] = KCAM::cieXyzAbsToKcam(cieXyzToCieXyzAbs(xyz, LUMINANCE));
    return {h, C, J};
}
