use super::*;

macro_rules! test {
    ($name: ident: ($left: expr) < ($right: expr)) => {
        #[test]
        fn $name() {
            let lhs = Posit { bits: $left };
            let rhs = Posit { bits: $right };
            assert_eq!(lhs.partial_cmp(&rhs), Some(cmp::Ordering::Less));
        }
    };
    ($name: ident: ($left: expr) = ($right: expr)) => {
        #[test]
        fn $name() {
            assert_eq!($left, $right);
        }
    };
    ($name: ident: ($left: expr) * ($right: expr) = ($expected: expr)) => {
        #[test]
        fn $name() {
            {
                let lhs = Posit { bits: $left };
                let rhs = Posit { bits: $right };
                let res = lhs * rhs;
                assert_eq!(res.bits, $expected);
            }
            {
                let lhs = Posit { bits: $right };
                let rhs = Posit { bits: $left };
                let res = lhs * rhs;
                assert_eq!(res.bits, $expected);
            }
        }
    };
    ($name: ident: ($left: expr) / ($right: expr) = ($expected: expr)) => {
        #[test]
        fn $name() {
            let lhs = Posit { bits: $left };
            let rhs = Posit { bits: $right };
            let res = lhs / rhs;
            assert_eq!(res.bits[..$expected.len()], $expected);
        }
    };
    ($name: ident: ($left: expr) + ($right: expr) = ($expected: expr)) => {
        #[test]
        fn $name() {
            {
                let lhs = Posit { bits: $left };
                let rhs = Posit { bits: $right };
                let res = lhs + rhs;
                assert_eq!(res.bits, $expected);
            }
            {
                let lhs = Posit { bits: $right };
                let rhs = Posit { bits: $left };
                let res = lhs + rhs;
                assert_eq!(res.bits, $expected);
            }
            {
                let lhs = Posit { bits: $expected };
                let rhs = Posit { bits: $left };
                let res = lhs - rhs;
                assert_eq!(res.bits, $right);
            }
            {
                let lhs = Posit { bits: $expected };
                let rhs = Posit { bits: $right };
                let res = lhs - rhs;
                assert_eq!(res.bits, $left);
            }
        }
    };
    ($name: ident: ($left: expr) ^ ($right: expr) = ($expected: expr)) => {
        #[test]
        fn $name() {
            let lhs = Posit { bits: $left };
            let rhs: usize = $right;
            let res = lhs.pow(rhs);
            assert_eq!(res.bits, $expected);
        }
    };
}

test! { convert_1:
    (Posit::from(0.625_f64).bits) =
    (bitvec![0, 0, 1, 1, 1, 0, 1])
}

test! { convert_2:
    (Posit::from(10e-8).bits[..13]) =
    (bitvec![0,  0, 0, 0, 0, 0, 0, 1,  0, 0,  1, 0, 1])
}

test! { convert_3:
    (Posit::from(10f64.powf(5.0)).bits[..16]) =
    (bitvec![0,  1, 1, 1, 1, 1, 0,  0, 0,  1, 0, 0, 0, 0, 1, 1])
}

test! { convert_4:
    (Posit::from(-0.625_f64).bits) =
    (bitvec![1, 0, 1, 1, 1, 0, 1])
}

test! { convert_5:
    (Posit::from(3f64.powf(2.0)).bits) =
    (bitvec![0, 1, 0, 1, 1, 0, 0, 1])
}

test! { convert_6:
    (Posit::from(3f64).bits) =
    (bitvec![0, 1, 0, 0, 1, 1])
}

test! { convert_7:
    (Posit::from(3f64.powf(3.0)).bits) =
    (bitvec![0, 1, 1, 0, 0, 0, 1, 0, 1, 1])
}

test! { multiplication_1:
    (bitvec![0, 1, 0, 0, 1, 0]) * // 2
    (bitvec![0, 1, 0, 0, 0, 1]) = // 1.5
    (bitvec![0, 1, 0, 0, 1, 1]) // 3
}

test! { multiplication_2:
    (bitvec![0, 1, 0, 1, 1]) * // 8
    (bitvec![0, 1, 0, 1, 1, 1]) = // 12
    (bitvec![0, 1, 1, 0, 1, 0, 1]) // 96
}

test! { multiplication_3:
    (bitvec![0, 1, 0, 0, 0, 0, 1]) * // 1.25
    (bitvec![1, 1, 0, 0, 0, 1]) = // -1.5
    (bitvec![1, 1, 0, 0, 0, 1, 1, 1]) // -1.875
}

test! { multiplication_4:
    (Posit::from(10f64.powf(5.0)).bits[..16].to_owned()) *
    (Posit::from(10f64.powf(5.0)).bits[..16].to_owned()) =
    (bitvec![
        0,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 0, // 16^8 *
        0, 1, // 2^1 *
        0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1 // (1 + 0b001010010001001 / 2**15)
    ])
}

test! { multiplication_5:
    (Posit::from(3f64.powf(3.0)).bits) *
    (Posit::from(3f64).bits) =
    (bitvec![
        0,
        1, 1, 0,
        1, 0,
        0, 1, 0, 0, 0, 1
    ])
}
test! { multiplication_6:
    (bitvec![0, 1, 1, 0, 0, 0, 1, 0, 1, 1]) * // 16 * 1 * 1.6875 = 27
    (bitvec![0, 1, 0, 0, 1, 1]) = // 2 * 1.5 = 3
    (bitvec![
        0,
        1, 1, 0,
        1, 0,
        0, 1, 0, 0, 0, 1
    ])
}

test! { division_1:
    (bitvec![0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]) /
    (bitvec![0, 1, 0, 0, 1, 1]) =
    (bitvec![0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1])
}

test! { division_2:
    (bitvec![0, 1, 0, 1, 1]) / // 8
    (bitvec![0, 1, 0, 1, 0]) = // 4
    (bitvec![0, 1, 0, 0, 1]) // 2
}

test! { add_sub_zero:
    (bitvec![0, 1, 0, 0, 1]) + // 2
    (Posit::zero().bits) =
    (bitvec![0, 1, 0, 0, 1])
}

test! { add_sub_1:
    (bitvec![0, 1, 0, 0, 1]) + // 2
    (bitvec![0, 1, 0, 0, 0, 1]) = // 1.5
    (bitvec![0, 1, 0, 0, 1, 1, 1]) // 3.5
}

test! { add_sub_2:
    (bitvec![0, 1, 0, 0, 1]) + // 2
    (bitvec![0, 1, 0, 0, 1, 1]) = // 3
    (bitvec![0, 1, 0, 1, 0, 0, 1]) // 5
}

test! { add_sub_3:
    (bitvec![0, 1, 0, 1, 1]) + // 8
    (bitvec![0, 1, 0, 1, 1, 1]) = // 12
    (bitvec![0, 1, 1, 0, 0, 0, 0, 1]) // 20
}

test! { add_sub_4:
    (bitvec![1, 1, 0, 1, 1]) + // -8
    (bitvec![0, 1, 0, 1, 1, 1]) = // 12
    (bitvec![0, 1, 0, 1]) // 4
}

test! { add_sub_5:
    ((Posit::from(10f64.powf(5.0)) * Posit::from(10f64.powf(5.0))).bits) +
    ((-Posit::from(10f64.powf(5.0)) * Posit::from(10f64.powf(5.0))).bits) =
    (bitvec![])
}

test! { add_sub_6:
    ((Posit::from(3f64.powf(3.0)) * Posit::from(3f64.powf(1.0))).bits) =
    ((Posit::from(3f64.powf(2.0)) * Posit::from(3f64.powf(2.0))).bits)
}

test! { lt_1:
    (bitvec![1, 1, 0, 1, 1]) < (bitvec![0, 1, 0, 1, 1, 1])
}

test! { lt_2:
    (bitvec![1, 1, 0, 1, 1, 1]) < (bitvec![1, 1, 0, 1, 1])
}

test! { lt_3:
    (bitvec![0, 1, 0, 0, 0]) < (bitvec![0, 1, 0, 0, 0, 1])
}

test! { lt_4:
    (bitvec![1, 1, 0, 0, 0, 1]) < (bitvec![1, 1, 0, 0, 0])
}

test! { lt_5:
    (bitvec![0, 0, 0, 0, 0, 1, 1, 0, 0]) <
    (bitvec![0, 0, 0, 0, 0, 1, 1, 0, 1])
}

test! { pow_1:
    (bitvec![0, 1, 0, 0, 0, 1]) ^ (2) =
    (bitvec![0, 1, 0, 0, 1, 0, 0, 1])
}

test! { pow_2:
    (bitvec![0, 1, 0, 0, 1, 1]) ^ (3) =
    (bitvec![0, 1, 1, 0, 0, 0, 1, 0, 1, 1])
}
