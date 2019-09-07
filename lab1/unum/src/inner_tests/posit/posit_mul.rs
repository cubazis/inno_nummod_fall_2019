use bitvec::bitvec;
use bitvec::cursor::BigEndian;
use bitvec::vec::BitVec;

use crate::posit::QPosit;
use crate::regime::Regime;

type MQPosit = QPosit<BigEndian, u32>;

#[test]
fn test_mult() {
    let a = MQPosit {
        is_negative: false,
        regime: Regime::new(2),
        exp: 3,
        frac: BitVec::new(),
    };

    let b = MQPosit {
        is_negative: false,
        regime: Regime::new(0),
        exp: 1,
        frac: BitVec::new(),
    };

    let expected = MQPosit {
        is_negative: false,
        regime: Regime::new(3),
        exp: 0,
        frac: BitVec::new(),
    };

    assert_eq!(a * b, expected);
}

#[test]
fn test_mult_neq_regime() {
    let a = MQPosit {
        is_negative: false,
        regime: Regime::new(-2),
        exp: 3,
        frac: BitVec::new(),
    };

    let b = MQPosit {
        is_negative: false,
        regime: Regime::new(0),
        exp: 1,
        frac: BitVec::new(),
    };

    let expected = MQPosit {
        is_negative: false,
        regime: Regime::new(-1),
        exp: 0,
        frac: BitVec::new(),
    };

    assert_eq!(a * b, expected);
}

#[test]
fn test_mul_reverse() {
    let a = MQPosit {
        is_negative: false,
        regime: Regime::new(2),
        exp: 3,
        frac: BitVec::new(),
    };

    let b = MQPosit {
        is_negative: false,
        regime: Regime::new(0),
        exp: 1,
        frac: BitVec::new(),
    };

    let expected = MQPosit {
        is_negative: false,
        regime: Regime::new(3),
        exp: 0,
        frac: BitVec::new(),
    };

    assert_eq!(b * a, expected);
}

#[test]
fn test_mul_frac() {
    let a = MQPosit {
        is_negative: false,
        regime: Regime::new(2),
        exp: 3,
        frac: bitvec![BigEndian, u32; 0, 1],
    };

    let b = MQPosit {
        is_negative: false,
        regime: Regime::new(2),
        exp: 3,
        frac: bitvec![BigEndian, u32; 0, 1],
    };

    let expected = MQPosit {
        is_negative: false,
        regime: Regime::new(5),
        exp: 2,
        frac: bitvec![BigEndian, u32; 1, 0, 0, 1],
    };

    assert_eq!(a * b, expected);
}

#[test]
fn test_mul_frac_overflow() {
    let a = MQPosit {
        is_negative: false,
        regime: Regime::new(2),
        exp: 3,
        frac: bitvec![BigEndian, u32; 1, 1],
    };

    let b = MQPosit {
        is_negative: false,
        regime: Regime::new(2),
        exp: 3,
        frac: bitvec![BigEndian, u32; 1],
    };

    let expected = MQPosit {
        is_negative: false,
        regime: Regime::new(5),
        exp: 3,
        frac: bitvec![BigEndian, u32; 0, 1, 0, 1],
    };

    assert_eq!(a * b, expected);
}

#[test]
fn test_mul_frac_overflow_neg() {
    let a = MQPosit {
        is_negative: true,
        regime: Regime::new(2),
        exp: 3,
        frac: bitvec![BigEndian, u32; 1, 1],
    };

    let b = MQPosit {
        is_negative: false,
        regime: Regime::new(2),
        exp: 3,
        frac: bitvec![BigEndian, u32; 1],
    };

    let expected = MQPosit {
        is_negative: true,
        regime: Regime::new(5),
        exp: 3,
        frac: bitvec![BigEndian, u32; 0, 1, 0, 1],
    };

    assert_eq!(a * b, expected);
}

