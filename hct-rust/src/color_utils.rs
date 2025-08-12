use crate::math_utils::powf;
use core::f64::math::{cbrt, round};

pub const Y_FROM_LINRGB: [f64; 3] = [0.2126, 0.7152, 0.0722];

pub const fn argb_from_rgb(red: u8, green: u8, blue: u8) -> u32 {
    (0xFF << 24) | ((red as u32) << 16) | ((green as u32) << 8) | (blue as u32)
}

pub fn argb_from_linrgb(linrgb: [f64; 3]) -> u32 {
    let r = delinearized(linrgb[0]);
    let g = delinearized(linrgb[1]);
    let b = delinearized(linrgb[2]);
    argb_from_rgb(r, g, b)
}

pub const fn red_from_argb(argb: u32) -> u8 {
    ((argb >> 16) & 0xFF) as u8
}

pub const fn green_from_argb(argb: u32) -> u8 {
    ((argb >> 8) & 0xFF) as u8
}

pub const fn blue_from_argb(argb: u32) -> u8 {
    (argb & 0xFF) as u8
}

pub fn argb_from_lstar(lstar: f64) -> u32 {
    let y = y_from_lstar(lstar);
    let component = delinearized(y);
    argb_from_rgb(component, component, component)
}

pub const fn y_from_lstar(lstar: f64) -> f64 {
    debug_assert!(lstar >= 0.0 && lstar <= 100.0);
    100.0 * lab_invf((lstar + 16.0) / 116.0)
}

pub fn linearized(rgb_component: u8) -> f64 {
    let normalized = rgb_component as f64 / 255.0;
    if normalized <= 0.040449936 {
        normalized / 12.92 * 100.0
    } else {
        powf((normalized + 0.055) / 1.055, 2.4) * 100.0
    }
}

pub fn delinearized(rgb_component: f64) -> u8 {
    let normalized = rgb_component / 100.0;
    let delinearized = if normalized <= 0.0031308 {
        normalized * 12.92
    } else {
        1.055 * powf(normalized, 1.0 / 2.4) - 0.055
    };
    round(delinearized * 255.0).clamp(0.0, 255.0) as u8
}

pub fn delinearized_double(rgb_component: f64) -> f64 {
    let normalized = rgb_component / 100.0;
    let delinearized = if normalized <= 0.0031308 {
        normalized * 12.92
    } else {
        1.055 * powf(normalized, 1.0 / 2.4) - 0.055
    };
    delinearized * 255.0
}

pub fn lab_f(t: f64) -> f64 {
    if t > 216.0 / 24389.0 {
        cbrt(t)
    } else {
        ((24389.0 / 27.0) * t + 16.0) / 116.0
    }
}

pub const fn lab_invf(ft: f64) -> f64 {
    let ft3 = ft * ft * ft;
    if ft3 > 216.0 / 24389.0 {
        ft3
    } else {
        (116.0 * ft - 16.0) / (24389.0 / 27.0)
    }
}
