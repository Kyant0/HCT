#include "kcam.h"

namespace KCAM {
    constexpr num C1 = 3424.0 / 4096;
    constexpr num C2 = 2413.0 / 128;
    constexpr num C3 = 2392.0 / 128;
    constexpr num ETA = 2610.0 / 16384;
    constexpr num RHO = 1.7 * 2523.0 / 32;
    constexpr num EPSILON = 3.7035226210190005E-11;

    constexpr num Cz_coeff = 90.045639314581067;

    constexpr num M[3][3] = {
        {0.674207838, 0.38279934, -0.047570458},
        {0.14928416, 0.73962834, 0.0833273},
        {0.07094108, 0.174768, 0.67097002},
    };
    constexpr num M_INV[3][3] = {
        {1.6613730557740694, -0.9145230923250666, 0.2313620767186149},
        {-0.3250758740427037, 1.5718470383669363, -0.21825383186729408},
        {-0.09098281098284759, -0.31272829052307405, 1.5227665613052606}
    };

    constexpr num M_2[2][3] = {
        {3.524000, -4.066708, 0.542708},
        {0.199076, 1.096799, -1.295875},
    };
    constexpr num M_2_INV[2][2] = {
        {0.2772100865430786, 0.11609463231223782},
        {0.042585801245220344, -0.75384457989992},
    };

    constexpr num ETA_INV = 8192.0 / 1305;
    constexpr num ETA_INV_MINUS_ONE = 6887.0 / 1305;
    constexpr num RHO_INV = 320.0 / 42891;
    constexpr num ONE_MINUS_RHO_INV = 42571.0 / 42891;
    constexpr num K = 5980.0 / 42891;

    constexpr num M_DPQ[3] = {
        20406.295480136619,
        -98671.041672811817,
        13700.654334535426,
    };

    inline num pq(const num x) {
        const num xp = pow(abs(x), RHO_INV);
        const num numer = C1 - xp;
        const num denom = C3 * xp - C2;
        const num d = numer / denom;

        return 10000.0 * signum(x) * signum(d) * pow(abs(d), ETA_INV);
    }

    inline num pqInv(const num x) {
        const num xp = pow(abs(x) / 10000.0, ETA);
        const num numer = C1 + C2 * xp;
        const num denom = 1.0 + C3 * xp;

        return signum(x) * pow(numer / denom, RHO);
    }

    inline num dpqDx(const num x) {
        const num x_abs = abs(x);
        const num xp = pow(x_abs, RHO_INV);
        const num numer = C1 - xp;
        const num xq = pow(x_abs, ONE_MINUS_RHO_INV);
        const num denom = C3 * xp - C2;
        const num numer_abs = abs(numer);
        const num denom_abs = abs(denom);
        return signum(numer_abs) * pow(numer_abs / denom_abs, ETA_INV_MINUS_ONE) * (
                   signum(denom) * RHO_INV / (xq * denom_abs)
                   - signum(numer_abs) * K * numer_abs / (xq * denom * denom));
    }

    inline num hpToEz(const num hp) {
        return 1.015 + cos((89.038 + hp) * PI / 180.0);
    }

    void bisect(CieXyzAbs &xyz, const num rp, const num bp, const num targetY, const num initialJz, const num epsilon) {
        num jLow = initialJz - 15.0;
        num jHigh = initialJz + 15.0;

        do {
            const num jMid = (jLow + jHigh) / 2.0;
            const num Iz = pqInv(cieLabLToCieXyzY(jMid) * LUMINANCE);
            const num I = Iz + EPSILON;

            const num r = pq(I + rp);
            const num g = pq(I);
            const num b = pq(I + bp);

            xyz.x = M_INV[0][0] * r + M_INV[0][1] * g + M_INV[0][2] * b;
            xyz.y = M_INV[1][0] * r + M_INV[1][1] * g + M_INV[1][2] * b;
            xyz.z = M_INV[2][0] * r + M_INV[2][1] * g + M_INV[2][2] * b;

            if (xyz.y > targetY) {
                jHigh = jMid;
            } else {
                jLow = jMid;
            }
        } while (abs(jHigh - jLow) > epsilon);
    }

    CieXyzAbs kcamToCieXyzAbs(const Kcam &kcam, const num epsilon) {
        const num Jz = kcam.J;
        const num targetY = cieLabLToCieXyzY(Jz) * LUMINANCE;

        if (kcam.C == 0L) {
            const num g = targetY / 1.0285173324996046;
            const num x = 0.978212040147826 * g;
            const num z = 1.1190554598 * g;

            return {x, targetY, z};
        }

        const num ez = hpToEz(kcam.h);
        const num Cz_p = pow(kcam.C / Cz_coeff / pow(ez, 0.068), 1.0 / 0.37 / 2.0);
        const num hzRad = kcam.h * PI / 180.0;
        const num az = Cz_p * cos(hzRad);
        const num bz = Cz_p * sin(hzRad);
        const num rp = M_2_INV[0][0] * az + M_2_INV[0][1] * bz;
        const num bp = M_2_INV[1][0] * az + M_2_INV[1][1] * bz;

        CieXyzAbs xyz{};

        if (Jz <= 15.0) {
            bisect(xyz, rp, bp, targetY, Jz, epsilon);
            return xyz;
        }

        num currentIz = pqInv(targetY);
        const num tolerance = epsilon;
        static constexpr int minIterations = 3;
        static constexpr int maxIterations = 8;
        int iterations = 1;

        while (true) {
            const num I = currentIz + EPSILON;

            const num r = pq(I + rp);
            const num g = pq(I);
            const num b = pq(I + bp);

            xyz.x = M_INV[0][0] * r + M_INV[0][1] * g + M_INV[0][2] * b;
            xyz.y = M_INV[1][0] * r + M_INV[1][1] * g + M_INV[1][2] * b;
            xyz.z = M_INV[2][0] * r + M_INV[2][1] * g + M_INV[2][2] * b;

            const num y = xyz.y;
            const num f = y - targetY;

            if (iterations >= minIterations && abs(f) < tolerance) {
                return xyz;
            }

            const num dyDI = M_DPQ[0] * dpqDx(I + rp) + M_DPQ[1] * dpqDx(I) + M_DPQ[2] * dpqDx(I + bp);
            currentIz -= f / dyDI;

            iterations++;

            if (iterations > maxIterations) {
                bisect(xyz, rp, bp, targetY, Jz, epsilon);
                return xyz;
            }
        }
    }

    Kcam cieXyzAbsToKcam(const CieXyzAbs &xyz) {
        const num Jz = cieXyzYToCieLabL(xyz.y / LUMINANCE);

        const num rp = pqInv(M[0][0] * xyz.x + M[0][1] * xyz.y + M[0][2] * xyz.z);
        const num gp = pqInv(M[1][0] * xyz.x + M[1][1] * xyz.y + M[1][2] * xyz.z);
        const num bp = pqInv(M[2][0] * xyz.x + M[2][1] * xyz.y + M[2][2] * xyz.z);

        const num az = M_2[0][0] * rp + M_2[0][1] * gp + M_2[0][2] * bp;
        const num bz = M_2[1][0] * rp + M_2[1][1] * gp + M_2[1][2] * bp;

        const num hzRad = atan2(bz, az) * 180.0 / PI;
        const num hz = hzRad < 0.0 ? hzRad + 360.0 : hzRad;

        const num ez = hpToEz(hz);
        const num Cz = Cz_coeff * pow(az * az + bz * bz, 0.37) * pow(ez, 0.068);

        return {Jz, Cz, hz};
    }
}
