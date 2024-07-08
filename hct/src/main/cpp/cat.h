#ifndef CAT16_H
#define CAT16_H

#include "ciexyz.h"

CieXyzAbs cat02(
    const CieXyzAbs &xyz,
    const CieXyzAbs &source,
    num degreeOfAdaptationForSource,
    const CieXyzAbs &target,
    num degreeOfAdaptationForTarget,
    const CieXyzAbs &baseline
);

CieXyzAbs cat16(
    const CieXyzAbs &xyz,
    const CieXyzAbs &source,
    num degreeOfAdaptationForSource,
    const CieXyzAbs &target,
    num degreeOfAdaptationForTarget,
    const CieXyzAbs &baseline
);

#endif //CAT16_H
