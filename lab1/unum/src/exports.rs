use super::*;

#[no_mangle]
#[doc(hidden)]
pub extern "C" fn posit_new() -> *mut u8 {
    Box::into_raw(Box::new(Posit { bits: bitvec![] })) as *mut _
}

#[no_mangle]
#[doc(hidden)]
pub unsafe extern "C" fn posit_neg(p: *mut u8) -> *mut u8 {
    let p = (p as *mut Posit).as_ref().unwrap();
    let r = Box::new(-p.clone());
    Box::into_raw(r) as *mut _
}

#[no_mangle]
#[doc(hidden)]
pub unsafe extern "C" fn posit_add(lhs: *mut u8, rhs: *mut u8) -> *mut u8 {
    let lhs = (lhs as *mut Posit).as_ref().unwrap();
    let rhs = (rhs as *mut Posit).as_ref().unwrap();
    let r = Box::new(lhs.clone() + rhs.clone());
    Box::into_raw(r) as *mut _
}

#[no_mangle]
#[doc(hidden)]
pub unsafe extern "C" fn posit_sub(lhs: *mut u8, rhs: *mut u8) -> *mut u8 {
    let lhs = (lhs as *mut Posit).as_ref().unwrap();
    let rhs = (rhs as *mut Posit).as_ref().unwrap();
    let r = Box::new(lhs.clone() - rhs.clone());
    Box::into_raw(r) as *mut _
}

#[no_mangle]
#[doc(hidden)]
pub unsafe extern "C" fn posit_mul(lhs: *mut u8, rhs: *mut u8) -> *mut u8 {
    let lhs = (lhs as *mut Posit).as_ref().unwrap();
    let rhs = (rhs as *mut Posit).as_ref().unwrap();
    let r = Box::new(lhs.clone() * rhs.clone());
    Box::into_raw(r) as *mut _
}

#[no_mangle]
#[doc(hidden)]
pub unsafe extern "C" fn posit_div(lhs: *mut u8, rhs: *mut u8) -> *mut u8 {
    let lhs = (lhs as *mut Posit).as_ref().unwrap();
    let rhs = (rhs as *mut Posit).as_ref().unwrap();
    let r = Box::new(lhs.clone() / rhs.clone());
    Box::into_raw(r) as *mut _
}

#[no_mangle]
#[doc(hidden)]
pub unsafe extern "C" fn posit_free(p: *mut u8) {
    Box::from_raw(p as *mut Posit);
}
