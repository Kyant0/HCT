#include "srgb.h"

static num f(const num x) {
    return x >= 0.00313066844250063
           ? 1.055 * pow(x, 1.0 / 2.4) - 0.055
           : 12.92 * x;
}

static num fInv(const num x) {
    return x >= 0.0404482362771082
           ? pow((x + 0.055) / 1.055, 2.4)
           : x / 12.92;
}

Srgb linSrgbToSrgb(const LinSrgb &linSrgb) {
    return {
            f(linSrgb.r),
            f(linSrgb.g),
            f(linSrgb.b),
    };
}

LinSrgb srgbToLinSrgb(const Srgb &srgb) {
    return {
            fInv(srgb.r),
            fInv(srgb.g),
            fInv(srgb.b),
    };
}
