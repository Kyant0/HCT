#![feature(core_float_math)]
#![allow(internal_features)]
#![feature(core_intrinsics)]
#![no_std]

use crate::hct_solver::HctSolver;
use core::ffi::c_int;
use core::ptr::null_mut;
use jni_sys::{jdouble, jdoubleArray, jint, jobject, JNIEnv};

mod color_utils;
mod hct_solver;
mod math_utils;

#[unsafe(no_mangle)]
extern "C" fn rust_eh_personality() {}

#[link(name = "c")]
unsafe extern "C" {
    fn abort() -> !;
}

#[panic_handler]
fn panic(_info: &core::panic::PanicInfo) -> ! {
    unsafe { abort() }
}

#[allow(non_snake_case)]
#[unsafe(no_mangle)]
pub extern "system" fn Java_com_kyant_hct_HctUtils_hctToInt(
    _env: *mut JNIEnv,
    _this: jobject,
    hue: jdouble,
    chroma: jdouble,
    tone: jdouble,
) -> jint {
    let argb = HctSolver::solve_to_int(hue, chroma, tone);
    argb as jint
}

#[allow(non_snake_case)]
#[unsafe(no_mangle)]
pub extern "system" fn Java_com_kyant_hct_HctUtils_hctFromInt(
    env: *mut JNIEnv,
    _this: jobject,
    argb: c_int,
) -> jdoubleArray {
    let hct = HctSolver::solve_from_int(argb as u32);
    let jni = unsafe { (**env).v1_6 };

    let result = unsafe { (jni.NewDoubleArray)(env, 3) };
    if result.is_null() {
        return null_mut();
    }

    unsafe {
        (jni.SetDoubleArrayRegion)(env, result, 0, 3, hct.as_ptr());
    }

    result
}
