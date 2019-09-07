use bitvec::bitvec;

use crate::posit::MyBitVec;

#[test]
fn dummy_1() {
    let a = bitvec![0, 1, 0];
    let b = bitvec![1];

    let expected = bitvec![0, 1, 1];

    assert_eq!(a + b, expected);
}

#[test]
fn dummy_2() {
    let a = bitvec![0, 1, 0];
    let b = bitvec![0, 1, 1];

    let expected = bitvec![0, 0, 1];

    assert_eq!(b - a, expected);
}

#[test]
fn dummy_3() {
    let a: i16 = -5;
    let b: u16 =  5;
    assert_eq!(a.abs() as u16, b);
}

#[test]
fn test_mul_1() {
    let a = MyBitVec(bitvec![1, 1, 0, 0, 0]);
    let b = MyBitVec(bitvec![1, 0, 1]);

    let expected = MyBitVec(bitvec![1, 1, 1, 1, 0, 0, 0]);

    assert_eq!(a * b, expected);
}

#[test]
fn test_mul_2() {
    let a = MyBitVec(bitvec![1, 1, 1]);
    let b = MyBitVec(bitvec![1, 1]);

    let expected = MyBitVec(bitvec![1, 0, 1, 0, 1]);

    assert_eq!(a * b, expected);
}

#[test]
fn test_mul_reverse() {
    let a = MyBitVec(bitvec![1, 1, 0, 0, 0]);
    let b = MyBitVec(bitvec![1, 0, 1]);

    let expected = MyBitVec(bitvec![1, 1, 1, 1, 0, 0, 0]);

    assert_eq!(b * a, expected);
}

#[test]
fn test_mul_zero() {
    let a = MyBitVec(bitvec![1, 1, 0, 0, 0]);
    let b = MyBitVec(bitvec![]);

    let expected = MyBitVec(bitvec![]);

    assert_eq!(a * b, expected);
}
