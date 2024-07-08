#ifndef CIEXYZ_H
#define CIEXYZ_H

#include "srgb.h"
#include "rgb_color_spaces.h"

struct CieXyz {
    num x;
    num y;
    num z;
};

constexpr CieXyz D65_SRGB = {
    0.3127 / 0.3290,
    1.0,
    (1.0 - 0.3127 - 0.3290) / 0.3290
};

LinSrgb cieXyzToLinSrgb(const CieXyz &ciexyz, RgbColorSpace rgbColorSpace);

CieXyz linSrgbToCieXyz(const LinSrgb &linSrgb, RgbColorSpace rgbColorSpace);

struct CieXyzAbs {
    num x;
    num y;
    num z;
};

CieXyz cieXyzAbsToCieXyz(const CieXyzAbs &cieXyzAbs, num luminance);

CieXyzAbs cieXyzToCieXyzAbs(const CieXyz &cieXyz, num luminance);

#endif //CIEXYZ_H
