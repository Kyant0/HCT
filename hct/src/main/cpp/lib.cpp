#include <jni.h>

#include "hct.h"

extern "C" {
JNIEXPORT void JNICALL
Java_com_kyant_hct_Hct_00024Companion_initHct(JNIEnv *, jobject, jboolean useP3) {
    initHctCond(useP3 ? P3 : SRGB);
}

JNIEXPORT jdoubleArray JNICALL
Java_com_kyant_hct_Hct_00024Companion_hctToColor(
        JNIEnv *env, jobject,
        jdouble hue, jdouble chroma, jdouble tone) {
    const Hct hct = {hue, chroma, tone};
    const auto [r, g, b] = hctToSrgb(hct);
    jdoubleArray result = env->NewDoubleArray(3);
    jdouble *resultPtr = env->GetDoubleArrayElements(result, nullptr);
    resultPtr[0] = r;
    resultPtr[1] = g;
    resultPtr[2] = b;
    env->ReleaseDoubleArrayElements(result, resultPtr, 0);
    return result;
}

JNIEXPORT jdoubleArray JNICALL
Java_com_kyant_hct_Hct_00024Companion_colorToHct(
        JNIEnv *env, jobject,
        jdouble red, jdouble green, jdouble blue) {
    const Srgb srgb = {red, green, blue};
    const auto [h, c, t] = srgbToHct(srgb);
    jdoubleArray result = env->NewDoubleArray(3);
    jdouble *resultPtr = env->GetDoubleArrayElements(result, nullptr);
    resultPtr[0] = h;
    resultPtr[1] = c;
    resultPtr[2] = t;
    env->ReleaseDoubleArrayElements(result, resultPtr, 0);
    return result;
}
}
