#ifndef ZCAM_H
#define ZCAM_H

#include "achromatic_cielab.h"

#define LUMINANCE 203.0

struct ZcamViewingConditions {
    num surroundFactor;
    CieXyzAbs referenceWhite;
    num degreeOfAdaptation;

    num Iz_coeff;
    num ez_coeff;
    num Qz_denom;
    num Mz_denom;
    num Qz_w;
};

struct Zcam {
    num J;
    num C;
    num h;
    ZcamViewingConditions cond;
};

ZcamViewingConditions zcamViewingConditions(
    num surroundFactor,
    num adaptingLuminance,
    num backgroundLuminance,
    const CieXyzAbs &referenceWhite
);

ZcamViewingConditions zcamViewingConditionsDefault();

CieXyzAbs zcamToCieXyzAbs(const Zcam &zcam);

Zcam cieXyzAbsToZcam(const CieXyzAbs &xyz, const ZcamViewingConditions &cond);

num getAchromaticZcamJByCieXyzY(num y, const ZcamViewingConditions &cond);

#endif //ZCAM_H
