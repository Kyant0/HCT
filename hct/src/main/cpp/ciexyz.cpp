#include "ciexyz.h"

constexpr num mSrgb2Xyz[3][3] = {
        {0.4123907992659595,  0.35758433938387796, 0.1804807884018343},
        {0.21263900587151036, 0.7151686787677559,  0.07219231536073371},
        {0.01933081871559185, 0.11919477979462599, 0.9505321522496606},
};

constexpr num mXyz2Srgb[3][3] = {
        {3.2409699419045213,  -1.5373831775700935,  -0.4986107602930033},
        {-0.9692436362808798, 1.8759675015077206,   0.04155505740717561},
        {0.05563007969699361, -0.20397695888897657, 1.0569715142428786},
};

constexpr num mP3Rgb2Xyz[3][3] = {
        {0.48657094864821626, 0.26566769316909294,  0.1982172852343625},
        {0.22897456406974884, 0.6917385218365062,   0.079286914093745},
        {0.0,                 0.045113381858902575, 1.0439443689009757},
};
constexpr num mXyz2P3Rgb[3][3] = {
        {2.493496911941425,    -0.9313836179191236,  -0.4027107844507169},
        {-0.8294889695615749,  1.7626640603183468,   0.0236246858419436},
        {0.035845830243784335, -0.07617238926804171, 0.9568845240076874}
};

LinSrgb cieXyzToLinSrgb(const CieXyz &ciexyz, const RgbColorSpace rgbColorSpace) {
    switch (rgbColorSpace) {
        case SRGB:
            return {
                    mXyz2Srgb[0][0] * ciexyz.x + mXyz2Srgb[0][1] * ciexyz.y + mXyz2Srgb[0][2] * ciexyz.z,
                    mXyz2Srgb[1][0] * ciexyz.x + mXyz2Srgb[1][1] * ciexyz.y + mXyz2Srgb[1][2] * ciexyz.z,
                    mXyz2Srgb[2][0] * ciexyz.x + mXyz2Srgb[2][1] * ciexyz.y + mXyz2Srgb[2][2] * ciexyz.z,
            };
        case P3:
            return {
                    mXyz2P3Rgb[0][0] * ciexyz.x + mXyz2P3Rgb[0][1] * ciexyz.y + mXyz2P3Rgb[0][2] * ciexyz.z,
                    mXyz2P3Rgb[1][0] * ciexyz.x + mXyz2P3Rgb[1][1] * ciexyz.y + mXyz2P3Rgb[1][2] * ciexyz.z,
                    mXyz2P3Rgb[2][0] * ciexyz.x + mXyz2P3Rgb[2][1] * ciexyz.y + mXyz2P3Rgb[2][2] * ciexyz.z,
            };
        default:
            return {NAN, NAN, NAN};
    }
}

CieXyz linSrgbToCieXyz(const LinSrgb &linSrgb, const RgbColorSpace rgbColorSpace) {
    switch (rgbColorSpace) {
        case SRGB:
            return {
                    mSrgb2Xyz[0][0] * linSrgb.r + mSrgb2Xyz[0][1] * linSrgb.g + mSrgb2Xyz[0][2] * linSrgb.b,
                    mSrgb2Xyz[1][0] * linSrgb.r + mSrgb2Xyz[1][1] * linSrgb.g + mSrgb2Xyz[1][2] * linSrgb.b,
                    mSrgb2Xyz[2][0] * linSrgb.r + mSrgb2Xyz[2][1] * linSrgb.g + mSrgb2Xyz[2][2] * linSrgb.b,
            };
        case P3:
            return {
                    mP3Rgb2Xyz[0][0] * linSrgb.r + mP3Rgb2Xyz[0][1] * linSrgb.g + mP3Rgb2Xyz[0][2] * linSrgb.b,
                    mP3Rgb2Xyz[1][0] * linSrgb.r + mP3Rgb2Xyz[1][1] * linSrgb.g + mP3Rgb2Xyz[1][2] * linSrgb.b,
                    mP3Rgb2Xyz[2][0] * linSrgb.r + mP3Rgb2Xyz[2][1] * linSrgb.g + mP3Rgb2Xyz[2][2] * linSrgb.b,
            };
        default:
            return {NAN, NAN, NAN};
    }
}

CieXyz cieXyzAbsToCieXyz(const CieXyzAbs &cieXyzAbs, const num luminance) {
    return {
            cieXyzAbs.x / luminance,
            cieXyzAbs.y / luminance,
            cieXyzAbs.z / luminance,
    };
}

CieXyzAbs cieXyzToCieXyzAbs(const CieXyz &cieXyz, const num luminance) {
    return {
            cieXyz.x * luminance,
            cieXyz.y * luminance,
            cieXyz.z * luminance,
    };
}
