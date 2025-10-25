# HCT (Android)

Google HCT color space library for Android

## Installation

[![Maven Central](https://img.shields.io/maven-central/v/io.github.kyant0/hct)](https://central.sonatype.com/artifact/io.github.kyant0/hct)

In build.gradle.kts, add

```kotlin
implementation("io.github.kyant0.hct:hct:<version>") // common code only
implementation("io.github.kyant0.hct:hct-compose:<version>") // with Jetpack Compose extensions
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
