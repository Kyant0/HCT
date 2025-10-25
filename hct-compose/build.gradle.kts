plugins {
    alias(libs.plugins.android.library)
    alias(libs.plugins.kotlin.android)
    alias(libs.plugins.kotlin.compose)
    id("com.vanniktech.maven.publish")
}

android {
    namespace = "com.kyant.hct"
    compileSdk = 36
    buildToolsVersion = "36.1.0"

    defaultConfig {
        minSdk = 21

        consumerProguardFiles("consumer-rules.pro")
    }

    buildTypes {
        release {
            isMinifyEnabled = false
            proguardFiles(getDefaultProguardFile("proguard-android-optimize.txt"), "proguard-rules.pro")
        }
    }
    kotlin {
        jvmToolchain(21)
    }
    buildFeatures {
        compose = true
    }
}

dependencies {
    api(project(":hct"))
    implementation(libs.androidx.compose.runtime)
    implementation(libs.androidx.compose.ui.graphics)
}

mavenPublishing {
    publishToMavenCentral()
    signAllPublications()

    coordinates("io.github.kyant0.hct", "hct-compose", "1.0.2")

    pom {
        name.set("HCT")
        description.set("Google HCT color space library for Android")
        inceptionYear.set("2025")
        url.set("https://github.com/Kyant0/HCT")
        licenses {
            license {
                name.set("The Apache License, Version 2.0")
                url.set("http://www.apache.org/licenses/LICENSE-2.0.txt")
                distribution.set("repo")
            }
        }
        developers {
            developer {
                id.set("Kyant0")
                name.set("Kyant")
                url.set("https://github.com/Kyant0")
            }
        }
        scm {
            url.set("https://github.com/Kyant0/HCT")
            connection.set("scm:git:git://github.com/Kyant0/HCT.git")
            developerConnection.set("scm:git:ssh://git@github.com/Kyant0/HCT.git")
        }
    }
}
