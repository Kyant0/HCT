use core::intrinsics;

#[link(name = "m")]
unsafe extern "C" {
    fn atan2(a: f64, b: f64) -> f64;
    fn hypot(x: f64, y: f64) -> f64;
}

#[inline]
pub fn atan2d(a: f64, b: f64) -> f64 {
    unsafe { atan2(a, b) }
}

#[inline]
pub fn hypotd(x: f64, y: f64) -> f64 {
    unsafe { hypot(x, y) }
}

#[inline]
pub fn powf(x: f64, n: f64) -> f64 {
    unsafe { intrinsics::powf64(x, n) }
}

#[inline]
pub fn cos(x: f64) -> f64 {
    unsafe { intrinsics::cosf64(x) }
}

#[inline]
pub fn sin(x: f64) -> f64 {
    unsafe { intrinsics::sinf64(x) }
}

#[inline]
pub const fn lerp(start: f64, stop: f64, amount: f64) -> f64 {
    start + (stop - start) * amount
}

#[inline]
pub const fn sanitize_degrees(degrees: f64) -> f64 {
    let degrees = degrees % 360.0;
    if degrees < 0.0 {
        degrees + 360.0
    } else {
        degrees
    }
}
