// https://doi.org/10.1364/OE.413659, M. Safdar, J. Hardeberg, and M. Ronnier Luo, "ZCAM, a colour appearance model based on a high dynamic range uniform colour space," Opt. Express  29, 6036-6052 (2021).

#include "zcam.h"

#include "cat.h"

constexpr num B = 1.15;
constexpr num G = 0.66;
constexpr num C1 = 3424.0 / 4096;
constexpr num C2 = 2413.0 / 128;
constexpr num C3 = 2392.0 / 128;
constexpr num ETA = 2610.0 / 16384;
constexpr num RHO = 1.7 * 2523.0 / 32;
constexpr num EPSILON = 3.7035226210190005E-11;

inline num pq(const num x) {
    const num x_p = pow(x, 1.0 / RHO);
    const num numer = C1 - x_p;
    const num denom = C3 * x_p - C2;

    return 10000.0 * pow(numer / denom, 1.0 / ETA);
}

inline num pqInv(const num x) {
    const num xp = pow(x / 10000.0, ETA);
    const num numer = C1 + C2 * xp;
    const num denom = 1.0 + C3 * xp;

    return pow(numer / denom, RHO);
}

inline void xyzToIzazbz(num izazbz[3], const CieXyzAbs &xyz) {
    const num xp = B * xyz.x - (B - 1) * xyz.z;
    const num yp = G * xyz.y - (G - 1) * xyz.x;

    const num rp = pqInv(0.41478972 * xp + 0.579999 * yp + 0.0146480 * xyz.z);
    const num gp = pqInv(-0.2015100 * xp + 1.120649 * yp + 0.0531008 * xyz.z);
    const num bp = pqInv(-0.0166008 * xp + 0.264800 * yp + 0.6684799 * xyz.z);

    const num az = 3.524000 * rp + -4.066708 * gp + 0.542708 * bp;
    const num bz = 0.199076 * rp + 1.096799 * gp + -1.295875 * bp;
    const num Iz = gp - EPSILON;

    izazbz[0] = Iz;
    izazbz[1] = az;
    izazbz[2] = bz;
}

inline num xyzToIz(const CieXyzAbs &xyz) {
    const num xp = B * xyz.x - (B - 1) * xyz.z;
    const num yp = G * xyz.y - (G - 1) * xyz.x;

    const num gp = pqInv(-0.2015100 * xp + 1.120649 * yp + 0.0531008 * xyz.z);

    const num Iz = gp - EPSILON;

    return Iz;
}

inline num hpToEz(const num hp) {
    return 1.015 + cos((89.038 + hp) * PI / 180.0);
}

inline num izToQz(const num Iz, const ZcamViewingConditions &cond) {
    return cond.Iz_coeff * pow(Iz, 1.6 * cond.surroundFactor / cond.Qz_denom);
}

inline num izToQz_raw(const num Iz, const num Iz_coeff, const num surroundFactor, const num Qz_denom) {
    return Iz_coeff * pow(Iz, 1.6 * surroundFactor / Qz_denom);
}

ZcamViewingConditions zcamViewingConditions(
    const num surroundFactor,
    const num adaptingLuminance,
    const num backgroundLuminance,
    const CieXyzAbs &referenceWhite
) {
    const num F_b = sqrt(backgroundLuminance / referenceWhite.y);
    const num F_l = 0.171 * cbrt(adaptingLuminance) * (1.0 - exp(-48.0 / 9.0 * adaptingLuminance));

    num d = surroundFactor * (1.0 - 1.0 / 3.6 * exp((-adaptingLuminance - 42.0) / 92.0));
    if (d < 0.0) {
        d = 0.0;
    } else if (d > 1.0) {
        d = 1.0;
    }

    const num Iz_coeff = 2700.0 * pow(surroundFactor, 2.2) * sqrt(F_b) * pow(F_l, 0.2);
    const num ez_coeff = pow(F_l, 0.2);
    const num Qz_denom = pow(F_b, 0.12);

    const num Iz_w = xyzToIz(referenceWhite);
    const num Mz_denom = pow(Iz_w, 0.78) * pow(F_b, 0.1);

    const num Qz_w = izToQz_raw(Iz_w, Iz_coeff, surroundFactor, Qz_denom);

    return {
        .surroundFactor = surroundFactor,
        .referenceWhite = referenceWhite,
        .degreeOfAdaptation = d,

        .Iz_coeff = Iz_coeff,
        .ez_coeff = ez_coeff,
        .Qz_denom = Qz_denom,
        .Mz_denom = Mz_denom,
        .Qz_w = Qz_w
    };
}

ZcamViewingConditions zcamViewingConditionsDefault() {
    constexpr num surroundFactor = 0.69;
    constexpr num adaptingLuminance = 0.4 * LUMINANCE;
    const num backgroundLuminance = cieLabLToCieXyzY(50.0) * LUMINANCE;
    const CieXyzAbs referenceWhite = cieXyzToCieXyzAbs(D65_SRGB, LUMINANCE);

    return zcamViewingConditions(surroundFactor, adaptingLuminance, backgroundLuminance, referenceWhite);
}

CieXyzAbs zcamToCieXyzAbs(const Zcam &zcam) {
    const auto [surroundFactor, referenceWhite, degreeOfAdaptation, Iz_coeff, ez_coeff, Qz_denom, Mz_denom, Qz_w] =
        zcam.cond;

    const num Iz = pow(zcam.J * Qz_w / (Iz_coeff * 100.0), Qz_denom / (1.6 * surroundFactor));

    if (zcam.C == 0L) {
        const num I = Iz + EPSILON;

        const num I_p = pow(I, 1.0 / RHO);
        const num numer = C1 - I_p;
        const num denom = C3 * I_p - C2;
        const num r = 10000.0 * pow(numer / denom, 1.0 / ETA);

        const num x = 0.978212040147826 * r;
        const num y = 1.0285173324996046 * r;
        const num z = 1.1190554598 * r;

        return {x, y, z};
    }

    const num Mz = zcam.C * Qz_w / 100.0;
    const num ez = hpToEz(zcam.h);
    const num Cz_p = pow(Mz * Mz_denom / (100.0 * pow(ez, 0.068) * ez_coeff), 1.0 / 0.37 / 2.0);
    const num hzRad = zcam.h * PI / 180.0;
    const num az = Cz_p * cos(hzRad);
    const num bz = Cz_p * sin(hzRad);

    const num I = Iz + EPSILON;

    const num r = pq(I + 0.2772100865430786 * az + 0.11609463231223775 * bz);
    const num g = pq(I);
    const num b = pq(I + 0.042585801245220344 * az + -0.7538445798999202 * bz);

    const num xp = 1.9242264357876067 * r + -1.0047923125953655 * g + 0.03765140403061801 * b;
    const num yp = 0.35031676209499907 * r + 0.7264811939316552 * g + -0.06538442294808502 * b;
    const num z = -0.09098281098284758 * r + -0.31272829052307400 * g + 1.5227665613052606 * b;

    const num x = (xp + (B - 1) * z) / B;
    const num y = (yp + (G - 1) * x) / G;

    if (referenceWhite.x / referenceWhite.y != D65_SRGB.x ||
        referenceWhite.z / referenceWhite.y != D65_SRGB.z) {
        const CieXyzAbs unadaptedXyz = cat02(
            {x, y, z},
            cieXyzToCieXyzAbs(D65_SRGB, 100.0),
            1.0,
            referenceWhite,
            1.0,
            CieXyzAbs{100.0, 100.0, 100.0}
        );
        return unadaptedXyz;
    }

    return {x, y, z};
}

Zcam cieXyzAbsToZcam(const CieXyzAbs &xyz, const ZcamViewingConditions &cond) {
    CieXyzAbs adaptedXyz = xyz;
    if (cond.referenceWhite.x / cond.referenceWhite.y != D65_SRGB.x ||
        cond.referenceWhite.z / cond.referenceWhite.y != D65_SRGB.z) {
        adaptedXyz = cat02(
            xyz,
            cond.referenceWhite,
            1.0,
            cieXyzToCieXyzAbs(D65_SRGB, 100.0),
            1.0,
            CieXyzAbs{100.0, 100.0, 100.0}
        );
    }

    num izazbz[3];
    xyzToIzazbz(izazbz, adaptedXyz);

    const num Iz = izazbz[0];
    const num az = izazbz[1];
    const num bz = izazbz[2];

    const num hzRad = atan2(bz, az) * 180.0 / PI;
    const num hz = hzRad < 0.0 ? hzRad + 360.0 : hzRad;

    const num ez = hpToEz(hz);

    const num Qz = izToQz(Iz, cond);
    const num Qz_w = cond.Qz_w;

    const num Jz = 100.0 * (Qz / Qz_w);
    const num Mz = 100.0 * pow(az * az + bz * bz, 0.37) * (pow(ez, 0.068) * cond.ez_coeff / cond.Mz_denom);
    const num Cz = 100.0 * (Mz / Qz_w);

    return {Jz, Cz, hz, cond};
}

num getAchromaticZcamJByCieXyzY(const num y, const ZcamViewingConditions &cond) {
    const num r = y / 1.0285173324996046;
    const num r_p = pow(r / 10000.0, ETA);
    const num I_p = (C1 + C2 * r_p) / (1.0 + C3 * r_p);
    const num I_z = pow(I_p, RHO) - EPSILON;
    const num Jz = pow(I_z, 1.6 * cond.surroundFactor / cond.Qz_denom) * cond.Iz_coeff * 100.0 / cond.Qz_w;
    return Jz;
}
