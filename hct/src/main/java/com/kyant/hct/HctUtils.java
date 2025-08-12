package com.kyant.hct;

import dalvik.annotation.optimization.FastNative;

public class HctUtils {

    static {
        System.loadLibrary("hct");
    }

    @FastNative
    public static native double[] hctFromInt(int argb);

    @FastNative
    public static native int hctToInt(double hue, double chroma, double tone);
}
