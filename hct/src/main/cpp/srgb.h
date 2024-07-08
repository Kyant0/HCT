#ifndef SRGB_H
#define SRGB_H

#include "math_ext.h"

struct Srgb {
    num r;
    num g;
    num b;
};

struct LinSrgb {
    num r;
    num g;
    num b;
};

Srgb linSrgbToSrgb(const LinSrgb &linSrgb);

LinSrgb srgbToLinSrgb(const Srgb &srgb);

#endif //SRGB_H
