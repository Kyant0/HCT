package com.kyant.hct

import androidx.compose.runtime.Immutable

@Immutable
interface SchemeStyle {
    data object Monochrome : SchemeStyle {
        override val primaryPaletteStyle = TonalPaletteStyle({ 36.0 / 2 })
        override val secondaryPaletteStyle = TonalPaletteStyle({ 16.0 / 2 })
        override val tertiaryPaletteStyle = TonalPaletteStyle({ 24.0 / 2 }, { it.hue + 60.0 })
        override val neutralPaletteStyle = TonalPaletteStyle({ 6.0 / 2 })
        override val neutralVariantPaletteStyle = TonalPaletteStyle({ 8.0 / 2 })
        override val errorPaletteStyle = TonalPaletteStyle({ 36.0 / 2 }, { 20.0 })
    }

    data object Neutral : SchemeStyle {
        override val primaryPaletteStyle = TonalPaletteStyle({ 36.0 / 2 })
        override val secondaryPaletteStyle = TonalPaletteStyle({ 16.0 / 2 })
        override val tertiaryPaletteStyle = TonalPaletteStyle({ 24.0 / 2 }, { it.hue + 60.0 })
        override val neutralPaletteStyle = TonalPaletteStyle({ 6.0 / 2 })
        override val neutralVariantPaletteStyle = TonalPaletteStyle({ 8.0 / 2 })
        override val errorPaletteStyle = TonalPaletteStyle({ 36.0 / 2 }, { 20.0 })
    }

    data object TonalSpot : SchemeStyle {
        override val primaryPaletteStyle = TonalPaletteStyle({ 36.0 / 2 })
        override val secondaryPaletteStyle = TonalPaletteStyle({ 16.0 / 2 })
        override val tertiaryPaletteStyle = TonalPaletteStyle({ 24.0 / 2 }, { it.hue + 60.0 })
        override val neutralPaletteStyle = TonalPaletteStyle({ 6.0 / 2 })
        override val neutralVariantPaletteStyle = TonalPaletteStyle({ 8.0 / 2 })
        override val errorPaletteStyle = TonalPaletteStyle({ 36.0 / 2 }, { 20.0 })
    }

    data object Vibrant : SchemeStyle {
        override val primaryPaletteStyle = TonalPaletteStyle({ 36.0 / 2 })
        override val secondaryPaletteStyle = TonalPaletteStyle({ 16.0 / 2 })
        override val tertiaryPaletteStyle = TonalPaletteStyle({ 24.0 / 2 }, { it.hue + 60.0 })
        override val neutralPaletteStyle = TonalPaletteStyle({ 6.0 / 2 })
        override val neutralVariantPaletteStyle = TonalPaletteStyle({ 8.0 / 2 })
        override val errorPaletteStyle = TonalPaletteStyle({ 36.0 / 2 }, { 20.0 })
    }

    data object Fidelity : SchemeStyle {
        override val primaryPaletteStyle = TonalPaletteStyle({ 36.0 / 2 })
        override val secondaryPaletteStyle = TonalPaletteStyle({ 16.0 / 2 })
        override val tertiaryPaletteStyle = TonalPaletteStyle({ 24.0 / 2 }, { it.hue + 60.0 })
        override val neutralPaletteStyle = TonalPaletteStyle({ 6.0 / 2 })
        override val neutralVariantPaletteStyle = TonalPaletteStyle({ 8.0 / 2 })
        override val errorPaletteStyle = TonalPaletteStyle({ 36.0 / 2 }, { 20.0 })
    }

    data object Content : SchemeStyle {
        override val primaryPaletteStyle = TonalPaletteStyle({ 36.0 / 2 })
        override val secondaryPaletteStyle = TonalPaletteStyle({ 16.0 / 2 })
        override val tertiaryPaletteStyle = TonalPaletteStyle({ 24.0 / 2 }, { it.hue + 60.0 })
        override val neutralPaletteStyle = TonalPaletteStyle({ 6.0 / 2 })
        override val neutralVariantPaletteStyle = TonalPaletteStyle({ 8.0 / 2 })
        override val errorPaletteStyle = TonalPaletteStyle({ 36.0 / 2 }, { 20.0 })
    }

    val primaryPaletteStyle: TonalPaletteStyle
    val secondaryPaletteStyle: TonalPaletteStyle
    val tertiaryPaletteStyle: TonalPaletteStyle
    val neutralPaletteStyle: TonalPaletteStyle
    val neutralVariantPaletteStyle: TonalPaletteStyle
    val errorPaletteStyle: TonalPaletteStyle
}

@Immutable
data class TonalPaletteStyle(
    val chroma: (Hct) -> Double,
    val hue: (Hct) -> Double = { it.hue }
)
