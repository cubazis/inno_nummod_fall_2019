use bitvec::bitvec;
use bitvec::cursor::BigEndian;
use bitvec::vec::BitVec;

use crate::posit::QPosit;
use crate::regime::Regime;

type MQPosit = QPosit<BigEndian, u32>;

#[test]
fn test_add() {
    let a = MQPosit {
        is_negative: false,
        regime: Regime::ZERO,
        exp: 1,
        frac: BitVec::new(),
    };

    let b = MQPosit {
        is_negative: false,
        regime: Regime::ZERO,
        exp: 0,
        frac: bitvec![BigEndian, u32; 1],
    };

    let expected = MQPosit {
        is_negative: false,
        regime: Regime::ZERO,
        exp: 1,
        frac: bitvec![BigEndian, u32; 1, 1],
    };

    assert_eq!(a + b, expected);
}

#[test]
fn test_add_2() {
    let a = MQPosit {
        is_negative: false,
        regime: Regime::ONE,
        exp: 1,
        frac: BitVec::new(),
    };

    let b = MQPosit {
        is_negative: false,
        regime: Regime::ZERO,
        exp: 0,
        frac: bitvec![BigEndian, u32; 1],
    };

    let expected = MQPosit {
        is_negative: false,
        regime: Regime::ONE,
        exp: 1,
        frac: bitvec![BigEndian, u32; 0, 0, 0, 0, 1, 1],
    };

    assert_eq!(a + b, expected);
}

#[test]
fn test_add_3() {
    let a = MQPosit {
        is_negative: true,
        regime: Regime::ONE,
        exp: 1,
        frac: BitVec::new(),
    };

    let b = MQPosit {
        is_negative: false,
        regime: Regime::ZERO,
        exp: 0,
        frac: bitvec![BigEndian, u32; 1],
    };

    println!("A: {:?}", a);
    println!("B: {:?}", b);

    let expected = MQPosit {
        is_negative: true,
        regime: Regime::ONE,
        exp: 0,
        frac: bitvec![BigEndian, u32; 1,1,1,0,1],
    };

    assert_eq!(a + b, expected);
}

#[test]
fn test_add_4() {
    let a = MQPosit {
        is_negative: false,
        regime: Regime::ONE,
        exp: 1,
        frac: BitVec::new(),
    };

    let b = MQPosit {
        is_negative: true,
        regime: Regime::ZERO,
        exp: 0,
        frac: bitvec![BigEndian, u32; 1],
    };

    println!("A: {:?}", a);
    println!("B: {:?}", b);

    let expected = MQPosit {
        is_negative: false,
        regime: Regime::ONE,
        exp: 0,
        frac: bitvec![BigEndian, u32; 1,1,1,0,1],
    };

    assert_eq!(a + b, expected);
}

#[test]
fn test_add_5() {
    let a = MQPosit {
        is_negative: true,
        regime: Regime::ONE,
        exp: 1,
        frac: BitVec::new(),
    };

    let b = MQPosit {
        is_negative: false,
        regime: Regime::ZERO,
        exp: 0,
        frac: bitvec![BigEndian, u32; 1],
    };

    println!("A: {:?}", a);
    println!("B: {:?}", b);

    let expected = MQPosit {
        is_negative: true,
        regime: Regime::ONE,
        exp: 0,
        frac: bitvec![BigEndian, u32; 1,1,1,0,1],
    };

    assert_eq!(b + a, expected);
}


#[test]
fn test_add_reverse() {
    let a = MQPosit {
        is_negative: false,
        regime: Regime::ZERO,
        exp: 1,
        frac: BitVec::new(),
    };

    let b = MQPosit {
        is_negative: false,
        regime: Regime::ZERO,
        exp: 0,
        frac: bitvec![BigEndian, u32; 1],
    };

    let expected = MQPosit {
        is_negative: false,
        regime: Regime::ZERO,
        exp: 1,
        frac: bitvec![BigEndian, u32; 1, 1],
    };

    assert_eq!(b + a, expected);
}

#[test]
fn test_add_overflow() {
    let a = MQPosit {
        is_negative: false,
        regime: Regime::ZERO,
        exp: 1,
        frac: bitvec![BigEndian, u32; 1],
    };

    let b = MQPosit {
        is_negative: false,
        regime: Regime::ZERO,
        exp: 1,
        frac: bitvec![BigEndian, u32; 1],
    };

    let expected = MQPosit {
        is_negative: false,
        regime: Regime::ZERO,
        exp: 2,
        frac: bitvec![BigEndian, u32; 1,0],
    };

    assert_eq!(a + b, expected);
}

#[test]
fn test_add_neg() {
    let a = MQPosit {
        is_negative: false,
        regime: Regime::ZERO,
        exp: 1,
        frac: BitVec::new(),
    };

    let b = MQPosit {
        is_negative: true,
        regime: Regime::ZERO,
        exp: 0,
        frac: bitvec![BigEndian, u32; 1],
    };

    let expected = MQPosit {
        is_negative: false,
        regime: Regime::new(-1),
        exp: 3,
        frac: BitVec::new(),
    };

    assert_eq!(a + b, expected);
}

#[test]
fn test_add_neg_double() {
    let a = MQPosit {
        is_negative: true,
        regime: Regime::ZERO,
        exp: 1,
        frac: BitVec::new(),
    };

    let b = MQPosit {
        is_negative: true,
        regime: Regime::ZERO,
        exp: 0,
        frac: bitvec![BigEndian, u32; 1],
    };

    let expected = MQPosit {
        is_negative: true,
        regime: Regime::ZERO,
        exp: 1,
        frac: bitvec![BigEndian, u32; 1, 1],
    };

    assert_eq!(a + b, expected);
}
