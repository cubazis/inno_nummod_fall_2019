use std::cmp::Ordering;
use std::ops::{Add, Neg, Sub};

use bitvec::cursor::BigEndian;
use bitvec::vec::BitVec;

type BitSlice = bitvec::slice::BitSlice<BigEndian, u32>;

#[derive(Copy, Clone, Debug, Eq, PartialEq)]
pub struct Regime {
    pub is_negative: bool,
    pub num: usize,
}

impl Ord for Regime {
    fn cmp(&self, other: &Self) -> Ordering {
        if self.is_negative == other.is_negative {
            self.num.cmp(&other.num)
        } else {
            if self.is_negative {
                Ordering::Less
            } else {
                Ordering::Greater
            }
        }
    }
}

impl PartialOrd for Regime {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        Some(self.cmp(other))
    }
}

impl Add for Regime {
    type Output = Self;

    fn add(self, rhs: Self) -> Self::Output {
        if self.is_negative == rhs.is_negative {
            Regime {
                is_negative: self.is_negative,
                num: self.num.saturating_add(rhs.num),
            }
        } else {
            match self.num.cmp(&rhs.num) {
                Ordering::Less => Regime {
                    is_negative: rhs.is_negative,
                    num: rhs.num - self.num,
                },
                Ordering::Greater => Regime {
                    is_negative: self.is_negative,
                    num: self.num - rhs.num,
                },
                Ordering::Equal => Regime::ZERO,
            }
        }
    }
}

impl Neg for Regime {
    type Output = Self;

    fn neg(self) -> Self::Output {
        if !self.is_negative && self.num == 0 {
            self
        } else {
            Regime {
                is_negative: !self.is_negative,
                num: self.num,
            }
        }
    }
}

impl Sub for Regime {
    type Output = Self;

    fn sub(self, rhs: Self) -> Self::Output {
        self + (-rhs)
    }
}

impl Regime {
    pub const ZERO: Regime = Regime {
        is_negative: false,
        num: 0,
    };

    pub const ONE: Regime = Regime {
        is_negative: false,
        num: 1,
    };

    pub fn new(num: i32) -> Regime {
        Regime {
            is_negative: num < 0,
            num: (num.abs() as usize),
        }
    }

    pub fn to_slice(&self) -> BitVec {
        let mut out = BitVec::new();
        for _ in 1..=self.num {
            out.push(!self.is_negative);
        }
        if !self.is_negative {
            out.push(!self.is_negative);
        }
        out.push(self.is_negative);
        out
    }

    pub fn from_slice(slice: &BitSlice) -> (&BitSlice, Regime) {
        let sign = slice[0];
        let regime_num = match slice[1..].iter().position(|b| b != sign) {
            Some(idx) => idx + 1,
            None => slice.len(),
        };

        let final_num = if sign { regime_num - 1 } else { regime_num };
        (
            &slice[regime_num..],
            Regime {
                is_negative: !sign,
                num: final_num,
            },
        )
    }
}
