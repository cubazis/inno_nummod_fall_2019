use std::cmp::Ordering;
use std::ops::{Mul, Neg, Sub};

use bitvec::cursor::BigEndian;
use bitvec::prelude::*;
use bitvec::vec::BitVec;
use std::ops::Add;

use crate::regime::*;

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct QPosit<C, T>
where
    C: Cursor,
    T: BitStore,
{
    pub is_negative: bool,
    pub regime: Regime,
    pub exp: usize,
    pub frac: BitVec<C, T>,
}

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct MyBitVec<C, T>(pub BitVec<C, T>)
where
    C: Cursor,
    T: BitStore;

impl<C, T> Mul for MyBitVec<C, T>
where
    C: Cursor,
    T: BitStore,
{
    type Output = Self;

    fn mul(self, rhs: Self) -> Self::Output {
        let mut out = BitVec::<C, T>::new();
        let offset = rhs.0.len();
        for (i, v) in rhs.0.iter().enumerate() {
            if v {
                let mut res = self.0.clone();
                if offset > i {
                    for _ in 1..(offset - i) {
                        res.push(false);
                    }
                }
                out += res;
            }
        }
        MyBitVec(out)
    }
}

impl<C, T> From<f64> for QPosit<C,T> where C: Cursor, T: BitStore {
    fn from(x: f64) -> Self {
        let mut bits: u64 = unsafe { std::mem::transmute(x) };
        let sign: bool = bits >> 63 == 0;
        let mut exponent: i16 = ((bits >> 52) & 0x7ff) as i16;
        let mantissa = if exponent == 0 {
            (bits & 0xfffffffffffff) << 1
        } else {
            (bits & 0xfffffffffffff) | 0x10000000000000
        };
        exponent -= 1023;
        let mantissa = mantissa << 12;

        let regime_num = (exponent.abs() as usize) / QPosit::<C, T>::MAX_EXP;
        let exp = (exponent.abs() as usize) % QPosit::<C, T>::MAX_EXP;

        let fraction: BitVec<C, T> = {
            let mut vec = BitVec::new();
            for i in (0..64u64).rev() {
                let bit = mantissa & (1 << i)  != 0;
                vec.push(bit);
            }
            vec
        };

        QPosit {
            is_negative: !sign,
            regime: Regime {
                is_negative: exponent < 0,
                num: regime_num
            },
            exp: exp,
            frac: QPosit::<C,T>::truncate_zeros(fraction)
        }
    }
}


impl<C, T> QPosit<C, T>
where
    C: Cursor,
    T: BitStore,
{
    pub const ES: usize = 2;

    const MAX_EXP: usize = 1 << QPosit::<C, T>::ES;
    const USEED: usize = 1 << QPosit::<C, T>::MAX_EXP;

    pub fn truncate_zeros(frac: BitVec<C, T>) -> BitVec<C, T> {
        let mut new_frac = frac.clone();
        while !new_frac.is_empty() {
            match new_frac.last() {
                Some(l) if !l => new_frac.pop(),
                _ => return new_frac
            };
        }
        new_frac
    }

    pub fn exp_to_bitvec(exp: usize) -> BitVec<C,T> {
        let mut out = BitVec::new();

        let mut mask = 1;
        let mut index: usize = 0;
        while mask <= exp {
            let bit = ((exp & mask) >> index) != 0;
            out.push(bit);
            mask <<= 1;
            index += 1;
        }
        if out.len() < QPosit::<C, T>::ES {
            out.push(false);
        }

        out.reverse();
        out
    }

    pub fn pack(self, limit: usize) -> BitVec<C,T> {

        let mut out = BitVec::<C, T>::new();

        let mut count: usize = 1;
        out.push(self.is_negative);

        out.extend(self.regime.to_slice());

        let mut exp = QPosit::<C,T>::exp_to_bitvec(self.exp);
        exp.truncate(QPosit::<C,T>::ES);

        out.extend(exp);
        out.extend(self.frac);

        out.truncate(limit);
        out
    }
}

impl<C, T> Mul for QPosit<C, T>
where
    C: Cursor,
    T: BitStore,
{
    type Output = Self;

    fn mul(self, rhs: Self) -> Self::Output {
        let mut left_frac = self.frac.clone();
        let mut right_frac = rhs.frac.clone();

        // Summed length of fraction bits
        let length = left_frac.len().saturating_add(right_frac.len()).saturating_add(1);

        // Add hidden bits
        left_frac.insert(0, true);
        right_frac.insert(0, true);

        // Multiply fractions
        let left_frac_wrapper = MyBitVec(left_frac);
        let right_frac_wrapper = MyBitVec(right_frac);
        let MyBitVec(mut new_frac) = left_frac_wrapper * right_frac_wrapper;

        // Exponent carry bit
        let carry_exp = (new_frac.len() > length) as usize;

        // Remove hidden bit
        new_frac.remove(0);

        // Calculate exponent
        let temp_exp = self.exp.saturating_add(rhs.exp).saturating_add(carry_exp);

        // Carry regime
        let (new_exp, carry_regime) = if temp_exp >= QPosit::<C, T>::MAX_EXP {
            (temp_exp - QPosit::<C, T>::MAX_EXP, Regime::ONE)
        } else {
            (temp_exp, Regime::ZERO)
        };

        QPosit {
            is_negative: self.is_negative != rhs.is_negative,
            regime: self.regime + rhs.regime + carry_regime,
            exp: new_exp,
            frac: QPosit::<C, T>::truncate_zeros(new_frac),
        }
    }
}

impl<C, T> Neg for QPosit<C, T>
where
    C: Cursor,
    T: BitStore,
{
    type Output = Self;

    fn neg(self) -> Self::Output {
        QPosit {
            is_negative: !self.is_negative,
            regime: self.regime,
            exp: self.exp,
            frac: self.frac,
        }
    }
}

impl<C, T> Sub for QPosit<C, T>
where
    C: Cursor,
    T: BitStore,
{
    type Output = Self;

    fn sub(self, rhs: Self) -> Self::Output {
        self + (-rhs)
    }
}

impl<C, T> Add for QPosit<C, T>
where
    C: Cursor,
    T: BitStore,
{
    type Output = Self;

    fn add(self, rhs: Self) -> Self::Output {
        // Equality check
        let sign_equal = self.is_negative == rhs.is_negative;

        // Sort absolute values
        let (left_max, right_min) = {
            match self.regime.cmp(&rhs.regime) {
                Ordering::Equal => match self.exp.cmp(&rhs.exp) {
                    Ordering::Equal => match self.frac.cmp(&rhs.frac) {
                        Ordering::Equal => (self, rhs),
                        Ordering::Less => (rhs, self),
                        Ordering::Greater => (self, rhs),
                    },
                    Ordering::Less => (rhs, self),
                    Ordering::Greater => (self, rhs),
                },
                Ordering::Less => (rhs, self),
                Ordering::Greater => (self, rhs),
            }
        };

        // Fraction shift calculation
        let regime_diff = left_max.regime - right_min.regime;
        let offset = regime_diff.num * (1 << QPosit::<C, T>::ES) +
            if left_max.exp < right_min.exp { right_min.exp - left_max.exp } else { left_max.exp - right_min.exp };

        let mut left_frac = left_max.frac.clone();
        let mut right_frac = right_min.frac.clone();

        if left_frac.len() < right_frac.len() {
            for _ in 0..(right_frac.len() - left_frac.len()) {
                left_frac.push(false);
            }
        }

        // Add hidden bits
        left_frac.insert(0, true);
        right_frac.insert(0, true);

        // Shift smaller fraction to the right
        for _ in 0..offset {
            right_frac.insert(0, false);
            left_frac.push(false);
        }

        let frac_length = right_frac.len();

        if sign_equal {
            let mut final_frac = left_frac + right_frac;
            let carry_exp = (final_frac.len() > frac_length) as usize;

            final_frac.remove(0);
            let exp = left_max.exp + carry_exp;

            let (new_exp, carry_regime) = if exp >= QPosit::<C, T>::MAX_EXP {
                (exp - QPosit::<C, T>::MAX_EXP, Regime::ONE)
            } else {
                (exp, Regime::ZERO)
            };

            QPosit {
                is_negative: left_max.is_negative,
                regime: left_max.regime + carry_regime,
                exp: new_exp,
                frac: final_frac,
            }
        } else {
            left_frac.insert(0, false);
            right_frac.insert(0, false);

            let mut final_frac = left_frac - right_frac;


            let mut one = bitvec![BigEndian, u32; 1];
            for _ in 0..(frac_length - 1) {
                one.push(false);
                final_frac.push(false);
            }

            let sign = final_frac.remove(0);

            final_frac = if !sign { final_frac } else { -final_frac };

            let mut carry_exp = 0;
            while final_frac < one {
                final_frac <<= 1;
                carry_exp += 1;
            }

            final_frac.remove(0);

            let mut exp = left_max.exp;
            let mut carry_regime = Regime::ZERO;

            while exp < carry_exp {
                exp += QPosit::<C, T>::MAX_EXP;
                carry_regime = carry_regime + Regime::ONE;
            }

            QPosit {
                is_negative: left_max.is_negative,
                regime: left_max.regime - carry_regime,
                exp: exp - carry_exp,
                frac: QPosit::<C, T>::truncate_zeros(final_frac),
            }
        }
    }
}

//impl<C, T> Into<f64> for QPosit<C, T> where C: Cursor, T: BitStore {
//    fn into(self) -> f64 {
//        let sign: u64 = self.is_negative << 63;
//        let exponent: i16 = (self.regime.is_negative as i16)
//    }
//}
