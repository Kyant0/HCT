# HCT (Android)

Google HCT color space library for Android

## Installation

[![JitPack Release](https://jitpack.io/v/Kyant0/HCT.svg)](https://jitpack.io/#Kyant0/HCT)

```kotlin
// settings.gradle.kts in root project
dependencyResolutionManagement {
    repositories {
        maven("https://jitpack.io")
    }
}

// build.gradle.kts in module
implementation("com.github.Kyant0.HCT:hct:<version>") // common code only
implementation("com.github.Kyant0.HCT:hct-compose:<version>")
```

## Usages

### Common

```kotlin
HctUtils.hctFromInt(0xFFFF0000.toInt()) // Convert ARGB to HCT
HctUtils.hctToInt(0.0, 50.0, 40.0) // Convert HCT to ARGB
```

### Compose

```kotlin
Color.Red.toHct() // Convert Color to HCT
Hct(0.0, 50.0, 40.0).toColor() // Convert HCT to Color
```
