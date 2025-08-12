package com.kyant.hct

import androidx.compose.runtime.Immutable
import androidx.compose.runtime.Stable
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.graphics.toArgb

@Immutable
data class Hct(
    val hue: Double,
    val chroma: Double,
    val tone: Double
) {

    @Stable
    fun toArgb(): Int {
        return HctUtils.hctToInt(hue, chroma, tone)
    }

    @Stable
    fun toColor(): Color {
        return Color(this.toArgb())
    }

    companion object {

        @Stable
        fun Color.toHct(): Hct {
            val hct = HctUtils.hctFromInt(this.toArgb())
            if (hct == null || hct.size != 3) {
                throw IllegalStateException("Failed to convert Color to HCT")
            }
            return Hct(hue = hct[0], chroma = hct[1], tone = hct[2])
        }
    }
}
