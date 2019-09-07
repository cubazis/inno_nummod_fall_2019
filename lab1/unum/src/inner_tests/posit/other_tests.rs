use bitvec::bitvec;
use bitvec::cursor::BigEndian;
use bitvec::vec::BitVec;

use crate::posit::QPosit;
use crate::regime::Regime;

type MQPosit = QPosit<BigEndian, u32>;

#[test]
fn test_truncate_zeros() {
    let a = bitvec![BigEndian, u32; 1,0, 0];
    let expected = bitvec![BigEndian, u32; 1];

    assert_eq!(MQPosit::truncate_zeros(a), expected);
}

#[test]
fn test_truncate_zeros_empty() {
    let a = bitvec![BigEndian, u32; 0];
    let expected = BitVec::<BigEndian, u32>::new();

    assert_eq!(MQPosit::truncate_zeros(a), expected);
}

#[test]
fn test_pack() {
    let a = MQPosit {
        is_negative: false,
        regime: Regime::ZERO,
        exp: 1,
        frac: BitVec::new(),
    };

    let expected = bitvec![BigEndian, u32; 0, 1, 0, 0, 1];

    assert_eq!(a.pack(5), expected);
}

#[test]
fn test_exp_to_bitvec_1() {
    let a = MQPosit::exp_to_bitvec(2);
    let expected = bitvec![BigEndian, u32; 1, 0];

    assert_eq!(a, expected);
}

#[test]
fn test_exp_to_bitvec_2() {
    let a = MQPosit::exp_to_bitvec(1);
    let expected = bitvec![BigEndian, u32; 0, 1];

    assert_eq!(a, expected);
}

#[test]
fn test_from_f64() {
    let a = 2.0f64;

    let expected = MQPosit {
        is_negative: false,
        regime: Regime::ZERO,
        exp: 1,
        frac: BitVec::new()
    };

    assert_eq!(MQPosit::from(a), expected);
}

#[test]
fn test_from_f64_2() {
    let a = 1.5f64;

    let expected = MQPosit {
        is_negative: false,
        regime: Regime::ZERO,
        exp: 0,
        frac: bitvec![BigEndian, u32; 1]
    };

    assert_eq!(MQPosit::from(a), expected);
}
#[test]
fn test_from_f64_3() {
    let a = 3.5f64;

    let expected = MQPosit {
        is_negative: false,
        regime: Regime::ZERO,
        exp: 1,
        frac: bitvec![BigEndian, u32; 1, 1]
    };

    assert_eq!(MQPosit::from(a), expected);
}



