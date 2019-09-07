use nalgebra::Vector6;
use std::cmp::Ordering;
use bitvec::cursor::BigEndian;
use bitvec::vec::BitVec;

use crate::posit::QPosit;
use crate::regime::*;

type Posit = QPosit<BigEndian, u32>;
type BitSlice = bitvec::slice::BitSlice<BigEndian, u32>;

const BS: [f64; 5] = [5.0, 8.0, 12.0, 15.0, 20.0];

type Vector = Vec<Posit>;

impl Ord for Posit {
    fn cmp(&self, other: &Self) -> Ordering {
        match (self.is_negative, other.is_negative) {
            (true, false) => Ordering::Less,
            (false, true) => Ordering::Greater,
            _ => match self.regime.cmp(&other.regime) {
                Ordering::Equal => match self.exp.cmp(&other.exp) {
                    Ordering::Equal => self.frac.cmp(&other.frac),
                    x => x
                },
                x => x
            }
        }
    }
}

impl PartialOrd for Posit {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        Some(self.cmp(&other))
    }
}

fn slice_to_usize(s: &mut BitSlice) -> usize {
    s.reverse();
    let mut out = 0;
    for (i, l) in s.iter().enumerate() {
        out += (l as usize) << i;
    }
    out
}

fn from_slice(slice: &BitSlice) -> Posit {
    let is_negative = slice[0];
    let (new_slice, regime) = Regime::from_slice(&slice[1..]);
    let exp = slice_to_usize(BitVec::from_bitslice(&new_slice[0..Posit::ES]).as_mut_bitslice());
    let frac = BitVec::from_bitslice(&new_slice[Posit::ES..]);

    QPosit {
        is_negative,
        regime,
        exp,
        frac
    }
}

fn pow(pos: Posit, power: u32) -> Posit {
    let mut start = pos.clone();
    for _ in 0..power {
        start = start * pos.clone();
    }
    start
}

fn init_v1(a: f64) -> Vector {
    vec![
        Posit::from(10f64.powf(a)),
        Posit::from(1223f64),
        Posit::from(10f64.powf(a - 1.0)),
        Posit::from(10f64.powf(a - 2.0)),
        Posit::from(3f64),
        Posit::from(-(10f64.powf(a - 5.0))),
    ]
}

fn init_v2(a: f64) -> Vector {
    vec![
        Posit::from(10f64.powf(a)),
        Posit::from(2f64),
        Posit::from(-(10f64.powf(a + 1.0))),
        Posit::from(10f64.powf(a)),
        Posit::from(2111f64),
        Posit::from(10f64.powf(a + 3.0)),
    ]
}

fn one() -> Posit {
    Posit {
        is_negative: false,
        regime: Regime::ONE,
        exp: 0,
        frac: BitVec::new()
    }
}

fn dot(v1: Vector, v2: Vector) -> Posit {
    let mut sum = one();
    for i in 0..6 {
        let a = v1[i].clone();
        let b = v2[i].clone();
        println!("A {:?}", &a);
        println!("B {:?}", &b);
        let res = a * b;
        println!("DIFF {:?}", &res);
        println!("SUM {:?}", &sum);
        sum = sum + res;
    }
    sum - one()
}

#[test]
fn test_a5() {
    let x = init_v1(5.0);
    for &i in &BS {
        let EPS: Posit = Posit::from(10e-8f64);
        let RES: Posit = Posit::from(8779.0f64);
        let y: Vector = init_v2(i);
        let dot_res = dot(x.clone(),y);
        println!("BS {:?} DOT_RES {:?}", i, dot_res);
        println!("RES {:?}", RES);
        assert!((dot_res - RES) < EPS);
    }
}

#[test]
fn test_a10() {
    let x = init_v1(10.0);
    for &i in &BS {
        let EPS: Posit = Posit::from(10e-8f64);
        let RES: Posit = Posit::from(8779.0f64);
        let y = init_v2(i);
        let dot_res = dot(x.clone(),y);
        println!("BS {:?} {:?}", i, dot_res);
        println!("{:?}", RES);
        assert!((dot_res - RES) < EPS);
    }
}
