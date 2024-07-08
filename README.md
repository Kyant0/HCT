# HCT (Kyant)

Another yet high performance **HCT (Kyant)** color space and **dynamic color scheme** library for Android Jetpack
Compose.

Compared to the Google's HCT, the HCT (Kyant) color space has the following advantages:

- It supports the P3 color space
- It doesn't have the blue-purple shift issue
- It's faster to compute one color (about 0.02ms in the worst case)
- It's more numerically accurate

## Usages

### Prerequisites

- Add the dependency

[![JitPack Release](https://jitpack.io/v/Kyant0/HCT.svg)](https://jitpack.io/#Kyant0/HCT)

```kotlin
allprojects {
    repositories {
        maven("https://jitpack.io")
    }
}

implementation("com.github.Kyant0:HCT:<version>")
```

- Initialize the library

```kotlin
System.loadLibrary("com.kyant.hct")
Hct.init(useP3 = true)
```

### HCT manipulation

- Create a raw `HCT`

```kotlin
val hct = Hct(hue = 240.0, chroma = 20.0, tone = 40.0)
```

Note: chroma < 28.0 for every color in P3 color space

- Convert `HCT` to `Color`

```kotlin
hct.toColor()
```

### Dynamic schemes

- Create a `DynamicScheme`

```kotlin
val isDark = isSystemInDarkTheme()
val scheme = remember(isDark) {
    DynamicScheme(
        sourceColorHct = hct,
        style = SchemeStyle.TonalSpot,
        isDark = isDark
    )
}
```

Note: Predefined scheme styles may not be the same as the Google's due to the different chroma scale

- Use color roles

```kotlin
scheme.primary
scheme.primaryContainer
// ... and more
```

Note: The library doesn't provide the deprecated colors roles, for example `background`, please
use `surfaceContainerXXX` instead

- Create a custom `SchemeStyle`

```kotlin
val SchemeYou = object : SchemeStyle {
    override val primaryPaletteStyle = TonalPaletteStyle(chroma = { 15.0 })
    override val secondaryPaletteStyle = TonalPaletteStyle({ 5.0 })
    override val tertiaryPaletteStyle = TonalPaletteStyle({ 10.0 }, hue = { it.hue + 120.0 })
    override val neutralPaletteStyle = TonalPaletteStyle({ 0.0 })
    override val neutralVariantPaletteStyle = TonalPaletteStyle({ 2.0 })
    override val errorPaletteStyle = TonalPaletteStyle({ 28.0 }, { 20.0 })
}
```

## References

- [M. Safdar, J. Hardeberg, and M. Ronnier Luo, "ZCAM, a colour appearance model based on a high dynamic range uniform colour space," Opt. Express  29, 6036-6052 (2021).](https://doi.org/10.1364/OE.413659)
- [Qiyan Zhai and Ming R. Luo, "Study of chromatic adaptation via neutral white matches on different viewing media," Opt. Express 26, 7724-7739 (2018)](https://doi.org/10.1364/OE.26.007724)
