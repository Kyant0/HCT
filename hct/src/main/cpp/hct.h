#ifndef HCT_H
#define HCT_H

#include "rgb_color_spaces.h"
#include "srgb.h"

void initHctCond(RgbColorSpace rgbColorSpace);

struct Hct {
    num h;
    num c;
    num t;
};

Srgb hctToSrgb(const Hct &hct);

Hct srgbToHct(const Srgb &srgb);

#endif //HCT_H
