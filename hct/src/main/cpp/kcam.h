#ifndef KCAM_H
#define KCAM_H

#include "achromatic_cielab.h"

namespace KCAM {
#define LUMINANCE 203.0

    struct Kcam {
        num J;
        num C;
        num h;
    };

    CieXyzAbs kcamToCieXyzAbs(const Kcam &kcam, num epsilon);

    Kcam cieXyzAbsToKcam(const CieXyzAbs &xyz);
}

#endif //KCAM_H
