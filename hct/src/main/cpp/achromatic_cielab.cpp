#include "achromatic_cielab.h"

num f(const num x) {
    if (x > 216.0 / 24389.0) {
        return cbrt(x);
    }
    return x / (108.0 / 841.0) + 4.0 / 29.0;
}

num fInv(const num x) {
    if (x > 6.0 / 29.0) {
        return x * x * x;
    }
    return 108.0 / 841.0 * (x - 4.0 / 29.0);
}

num cieLabLToCieXyzY(const num l) {
    return fInv((l + 16.0) / 116.0);
}

num cieXyzYToCieLabL(const num y) {
    return 116.0 * f(y) - 16.0;
}
