package com.kyant.hct

import androidx.collection.MutableScatterMap
import androidx.compose.runtime.Stable
import androidx.compose.ui.graphics.Color

@Stable
class DynamicScheme(
    val sourceColorHct: Hct,
    val style: SchemeStyle,
    val isDark: Boolean
) {
    private val cache = MutableScatterMap<Hct, Color>(47)

    fun TonalPaletteStyle.getTone(lightTone: Double, darkTone: Double): Color {
        val tone = if (isDark) darkTone else lightTone
        val hct = Hct(
            hue = hue(sourceColorHct),
            chroma = chroma(sourceColorHct),
            tone = tone
        )
        return cache.getOrPut(hct) { hct.toColor() }
    }

    /*val primaryPaletteKeyColor: Color
        get() = style.primaryPaletteStyle.keyColor.toColor()

    val secondaryPaletteKeyColor: Color
        get() = style.secondaryPaletteStyle.keyColor.toColor()

    val tertiaryPaletteKeyColor: Color
        get() = style.tertiaryPaletteStyle.keyColor.toColor()

    val neutralPaletteKeyColor: Color
        get() = style.neutralPaletteStyle.keyColor.toColor()

    val neutralVariantPaletteKeyColor: Color
        get() = style.neutralVariantPaletteStyle.keyColor.toColor()*/

    val surface: Color
        get() = style.neutralPaletteStyle.getTone(98.0, 6.0)

    val surfaceDim: Color
        get() = style.neutralPaletteStyle.getTone(87.0, 6.0)

    val surfaceBright: Color
        get() = style.neutralPaletteStyle.getTone(98.0, 24.0)

    val surfaceContainerLowest: Color
        get() = style.neutralPaletteStyle.getTone(100.0, 4.0)

    val surfaceContainerLow: Color
        get() = style.neutralPaletteStyle.getTone(96.0, 10.0)

    val surfaceContainer: Color
        get() = style.neutralPaletteStyle.getTone(94.0, 12.0)

    val surfaceContainerHigh: Color
        get() = style.neutralPaletteStyle.getTone(92.0, 17.0)

    val surfaceContainerHighest: Color
        get() = style.neutralPaletteStyle.getTone(90.0, 22.0)

    val onSurface: Color
        get() = style.neutralPaletteStyle.getTone(10.0, 90.0)

    val surfaceVariant: Color
        get() = style.neutralPaletteStyle.getTone(90.0, 30.0)

    val onSurfaceVariant: Color
        get() = style.neutralPaletteStyle.getTone(30.0, 80.0)

    val inverseSurface: Color
        get() = style.neutralPaletteStyle.getTone(20.0, 90.0)

    val inverseOnSurface: Color
        get() = style.neutralPaletteStyle.getTone(95.0, 20.0)

    val outline: Color
        get() = style.neutralPaletteStyle.getTone(60.0, 50.0)

    val outlineVariant: Color
        get() = style.neutralPaletteStyle.getTone(80.0, 30.0)

    val shadow: Color
        get() = style.neutralPaletteStyle.getTone(0.0, 0.0)

    val scrim: Color
        get() = style.neutralPaletteStyle.getTone(0.0, 0.0)

    val surfaceTint: Color
        get() = style.neutralPaletteStyle.getTone(40.0, 80.0)

    val primary: Color
        get() = style.primaryPaletteStyle.getTone(40.0, 80.0)

    val onPrimary: Color
        get() = style.primaryPaletteStyle.getTone(100.0, 20.0)

    val primaryContainer: Color
        get() = style.primaryPaletteStyle.getTone(90.0, 30.0)

    val onPrimaryContainer: Color
        get() = style.primaryPaletteStyle.getTone(30.0, 90.0)

    val inversePrimary: Color
        get() = style.primaryPaletteStyle.getTone(80.0, 40.0)

    val secondary: Color
        get() = style.secondaryPaletteStyle.getTone(40.0, 80.0)

    val onSecondary: Color
        get() = style.secondaryPaletteStyle.getTone(100.0, 20.0)

    val secondaryContainer: Color
        get() = style.secondaryPaletteStyle.getTone(90.0, 30.0)

    val onSecondaryContainer: Color
        get() = style.secondaryPaletteStyle.getTone(10.0, 90.0)

    val tertiary: Color
        get() = style.tertiaryPaletteStyle.getTone(40.0, 80.0)

    val onTertiary: Color
        get() = style.tertiaryPaletteStyle.getTone(100.0, 20.0)

    val tertiaryContainer: Color
        get() = style.tertiaryPaletteStyle.getTone(90.0, 30.0)

    val onTertiaryContainer: Color
        get() = style.tertiaryPaletteStyle.getTone(30.0, 90.0)

    val error: Color
        get() = style.errorPaletteStyle.getTone(40.0, 80.0)

    val onError: Color
        get() = style.errorPaletteStyle.getTone(100.0, 20.0)

    val errorContainer: Color
        get() = style.errorPaletteStyle.getTone(90.0, 30.0)

    val onErrorContainer: Color
        get() = style.errorPaletteStyle.getTone(30.0, 90.0)

    val primaryFixed: Color
        get() = style.primaryPaletteStyle.getTone(90.0, 90.0)

    val primaryFixedDim: Color
        get() = style.primaryPaletteStyle.getTone(80.0, 80.0)

    val onPrimaryFixed: Color
        get() = style.primaryPaletteStyle.getTone(10.0, 10.0)

    val onPrimaryFixedVariant: Color
        get() = style.primaryPaletteStyle.getTone(30.0, 30.0)

    val secondaryFixed: Color
        get() = style.secondaryPaletteStyle.getTone(90.0, 90.0)

    val secondaryFixedDim: Color
        get() = style.secondaryPaletteStyle.getTone(80.0, 80.0)

    val onSecondaryFixed: Color
        get() = style.secondaryPaletteStyle.getTone(10.0, 10.0)

    val onSecondaryFixedVariant: Color
        get() = style.secondaryPaletteStyle.getTone(30.0, 30.0)

    val tertiaryFixed: Color
        get() = style.tertiaryPaletteStyle.getTone(90.0, 90.0)

    val tertiaryFixedDim: Color
        get() = style.tertiaryPaletteStyle.getTone(80.0, 80.0)

    val onTertiaryFixed: Color
        get() = style.tertiaryPaletteStyle.getTone(10.0, 10.0)

    val onTertiaryFixedVariant: Color
        get() = style.tertiaryPaletteStyle.getTone(30.0, 30.0)

    override fun equals(other: Any?): Boolean {
        if (this === other) return true
        if (other !is DynamicScheme) return false

        if (sourceColorHct != other.sourceColorHct) return false
        if (style != other.style) return false
        if (isDark != other.isDark) return false

        return true
    }

    override fun hashCode(): Int {
        var result = sourceColorHct.hashCode()
        result = 31 * result + style.hashCode()
        result = 31 * result + isDark.hashCode()
        return result
    }

    override fun toString(): String {
        return "DynamicScheme(sourceColorHct=$sourceColorHct, style=$style, isDark=$isDark)"
    }
}
