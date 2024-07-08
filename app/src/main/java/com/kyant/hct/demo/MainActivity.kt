package com.kyant.hct.demo

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.compose.foundation.background
import androidx.compose.foundation.isSystemInDarkTheme
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.safeDrawingPadding
import androidx.compose.foundation.rememberScrollState
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.foundation.text.BasicText
import androidx.compose.foundation.verticalScroll
import androidx.compose.runtime.remember
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.text.TextStyle
import androidx.compose.ui.text.font.FontWeight
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import com.kyant.hct.DynamicScheme
import com.kyant.hct.Hct
import com.kyant.hct.SchemeStyle

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContent {
            val isDark = isSystemInDarkTheme()
            val scheme = remember(isDark) {
                DynamicScheme(
                    sourceColorHct = Hct(240.0, 20.0, 40.0),
                    style = SchemeStyle.TonalSpot,
                    isDark = isDark
                )
            }

            Column(
                Modifier
                    .background(scheme.surfaceContainerLowest)
                    .safeDrawingPadding()
                    .verticalScroll(rememberScrollState())
                    .padding(16.dp),
                verticalArrangement = Arrangement.spacedBy(4.dp)
            ) {
                BasicText(
                    "HCT",
                    Modifier.padding(8.dp, 24.dp, 8.dp, 8.dp),
                    style = TextStyle.Default.copy(
                        fontSize = 24.sp,
                        fontWeight = FontWeight.Bold
                    ),
                    color = { scheme.onSurface }
                )

                Box(
                    Modifier
                        .fillMaxWidth()
                        .height(80.dp)
                        .background(Hct(240.0, 20.0, 40.0).toColor(), RoundedCornerShape(16.dp)),
                    contentAlignment = Alignment.Center
                ) {
                    BasicText(
                        "HCT(240.0, 20.0, 40.0)",
                        color = { Color.White }
                    )
                }
                Box(
                    Modifier
                        .fillMaxWidth()
                        .height(80.dp)
                        .background(Hct(30.0, 20.0, 55.0).toColor(), RoundedCornerShape(16.dp)),
                    contentAlignment = Alignment.Center
                ) {
                    BasicText(
                        "HCT(30.0, 20.0, 55.0)",
                        color = { Color.White }
                    )
                }
                Box(
                    Modifier
                        .fillMaxWidth()
                        .height(80.dp)
                        .background(Hct(30.0, 0.0, 55.0).toColor(), RoundedCornerShape(16.dp)),
                    contentAlignment = Alignment.Center
                ) {
                    BasicText(
                        "HCT(30.0, 0.0, 55.0)",
                        color = { Color.White }
                    )
                }
                Box(
                    Modifier
                        .fillMaxWidth()
                        .height(80.dp)
                        .background(Hct(120.0, 20.0, 90.0).toColor(), RoundedCornerShape(16.dp)),
                    contentAlignment = Alignment.Center
                ) {
                    BasicText(
                        "HCT(120.0, 20.0, 90.0)",
                        color = { Color.Black }
                    )
                }
                Box(
                    Modifier
                        .fillMaxWidth()
                        .height(80.dp)
                        .background(Hct(100.0, 15.0, 90.0).toColor(), RoundedCornerShape(16.dp)),
                    contentAlignment = Alignment.Center
                ) {
                    BasicText(
                        "HCT(100.0, 15.0, 90.0)",
                        color = { Color.Black }
                    )
                }
                Box(
                    Modifier
                        .fillMaxWidth()
                        .height(80.dp)
                        .background(Hct(150.0, 5.0, 98.0).toColor(), RoundedCornerShape(16.dp)),
                    contentAlignment = Alignment.Center
                ) {
                    BasicText(
                        "HCT(150.0, 5.0, 99.0)",
                        color = { Color.Black }
                    )
                }

                BasicText(
                    "DynamicScheme",
                    Modifier.padding(8.dp, 24.dp, 8.dp, 8.dp),
                    style = TextStyle.Default.copy(
                        fontSize = 24.sp,
                        fontWeight = FontWeight.Bold
                    ),
                    color = { scheme.onSurface }
                )

                Box(
                    Modifier
                        .fillMaxWidth()
                        .height(80.dp)
                        .background(scheme.primary, RoundedCornerShape(16.dp)),
                    contentAlignment = Alignment.Center
                ) {
                    BasicText(
                        "primary",
                        color = { scheme.onPrimary }
                    )
                }
                Box(
                    Modifier
                        .fillMaxWidth()
                        .height(80.dp)
                        .background(scheme.primaryContainer, RoundedCornerShape(16.dp)),
                    contentAlignment = Alignment.Center
                ) {
                    BasicText(
                        "primaryContainer",
                        color = { scheme.onPrimaryContainer }
                    )
                }
                Box(
                    Modifier
                        .fillMaxWidth()
                        .height(80.dp)
                        .background(scheme.tertiary, RoundedCornerShape(16.dp)),
                    contentAlignment = Alignment.Center
                ) {
                    BasicText(
                        "tertiary",
                        color = { scheme.onTertiary }
                    )
                }
            }
        }
    }
}
