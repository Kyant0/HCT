package com.kyant.hct

import androidx.compose.runtime.Immutable
import androidx.compose.runtime.Stable
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.graphics.colorspace.ColorSpace
import androidx.compose.ui.graphics.colorspace.ColorSpaces
import dalvik.annotation.optimization.FastNative

@Immutable
data class Hct(
    val hue: Double,
    val chroma: Double,
    val tone: Double
) {
    @Stable
    fun toColor(): Color {
        val (r, g, b) = hctToColor(hue, chroma, tone)
        return Color(r.toFloat(), g.toFloat(), b.toFloat(), colorSpace = colorSpace)
    }

    companion object {
        private var useP3: Boolean = true
        private lateinit var colorSpace: ColorSpace

        fun init(useP3: Boolean) {
            Companion.useP3 = useP3
            colorSpace = if (Companion.useP3) ColorSpaces.DisplayP3 else ColorSpaces.Srgb
            initHct(useP3)
        }

        @Stable
        fun Color.toHct(): Hct {
            val color = this.convert(colorSpace)
            val (h, c, t) = colorToHct(color.red.toDouble(), color.green.toDouble(), color.blue.toDouble())
            return Hct(h, c, t)
        }

        private external fun initHct(useP3: Boolean)

        @FastNative
        private external fun hctToColor(hue: Double, chroma: Double, tone: Double): DoubleArray

        @FastNative
        private external fun colorToHct(red: Double, green: Double, blue: Double): DoubleArray
    }
}
