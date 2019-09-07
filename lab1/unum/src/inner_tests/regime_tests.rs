use super::*;

use crate::regime::*;
use bitvec::prelude::*;

#[test]
fn test_constructor() {
    let a = Regime::new(5);

    let expected = Regime {
        is_negative: false,
        num: 5,
    };

    assert_eq!(a, expected);
}

#[test]
fn test_constructor_neg() {
    let a = Regime::new(-5);

    let expected = Regime {
        is_negative: true,
        num: 5,
    };

    assert_eq!(a, expected);
}

#[test]
fn test_to_slice_pos() {
    let r = Regime::new(5);
    let expected = bitvec![BigEndian, u32; 1, 1, 1, 1, 1, 1, 0];

    assert_eq!(r.to_slice(), expected);
}

#[test]
fn test_to_slice_neg() {
    let r = Regime::new(-5);
    let expected = bitvec![BigEndian, u32; 0, 0, 0, 0, 0, 1];

    assert_eq!(r.to_slice(), expected);
}

#[test]
fn test_from_slice_pos() {
    let v = bitvec![BigEndian, u32; 1, 1, 1, 1, 1, 1, 0];
    let expected = Regime::new(5);

    let (_, res) = Regime::from_slice(v.as_bitslice());

    assert_eq!(res, expected);
}

#[test]
fn test_from_slice_neg() {
    let v = bitvec![BigEndian, u32; 0, 0, 0, 0, 0, 1];
    let expected = Regime::new(-5);

    let (_, res) = Regime::from_slice(v.as_bitslice());

    assert_eq!(res, expected);
}

#[test]
fn test_from_slice_pos_short() {
    let v = bitvec![BigEndian, u32; 1, 1, 1, 1, 1, 1];
    let expected = Regime::new(5);

    let (_, res) = Regime::from_slice(v.as_bitslice());

    assert_eq!(res, expected);
}

#[test]
fn test_from_slice_neg_short() {
    let v = bitvec![BigEndian, u32; 0, 0, 0, 0, 0];
    let expected = Regime::new(-5);

    let (_, res) = Regime::from_slice(v.as_bitslice());

    assert_eq!(res, expected);
}

#[test]
fn test_addition() {
    let a = Regime::new(-5);
    let b = Regime::new(6);

    let expected = Regime::new(1);

    assert_eq!(a + b, expected);
}

#[test]
fn test_addition_zero() {
    let a = Regime::new(-5);
    let b = Regime::new(5);

    let expected = Regime::ZERO;

    assert_eq!(a + b, expected);
}

#[test]
fn test_addition_zero_reverse() {
    let a = Regime::new(-5);
    let b = Regime::new(5);

    let expected = Regime::ZERO;

    assert_eq!(b + a, expected);
}

#[test]
fn test_neq() {
    let a = Regime::new(-5);
    let expected = Regime::new(5);

    assert_eq!(-a, expected);
}

#[test]
fn test_neq_zero() {
    let a = Regime::ZERO;
    let expected = Regime::ZERO;

    assert_eq!(-a, expected);
}

#[test]
fn test_subtraction() {
    let a = Regime::new(-5);
    let b = Regime::new(5);

    let expected = Regime::new(10);

    assert_eq!(b - a, expected);
}

#[test]
fn test_subtraction_reverse() {
    let a = Regime::new(-5);
    let b = Regime::new(5);

    let expected = Regime::new(-10);

    assert_eq!(a - b, expected);
}
