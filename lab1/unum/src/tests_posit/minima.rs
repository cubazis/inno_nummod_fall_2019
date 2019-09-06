use lazy_static::lazy_static;

use crate::Posit;

const LIMIT: u32 = 10000000;
lazy_static! {
    static ref RES: Posit = Posit::from(4.0 / 3.0);
    static ref EPS: Posit = Posit::from(10e-8);
}

fn half_divide_method(
    mut left: Posit,
    mut right: Posit,
    stop: Posit,
    f: impl Fn(Posit) -> Posit,
    iterations: u32,
) -> Posit {
    let mut x: Posit = (left.clone() + right.clone()) / 2.0.into();
    let mut i = 0;
    while f(x.clone()) >= stop && i < iterations {
        if (right.clone() - left.clone()) < EPS.clone() {
            break;
        }
        let left_third = left.clone() + (right.clone() - left.clone()) / 3.0.into();
        let right_third = right.clone() - (right.clone() - left.clone()) / 3.0.into();
        let f1 = f(left_third.clone());
        let f2 = f(right_third.clone());
        if f1 < f2 {
            right = right_third;
        } else {
            left = left_third;
        }
        x = (left.clone() + right.clone()) / 2.0.into();
        i += 1;
    }
    x
}

fn parabola(x: Posit) -> Posit {
    (x - RES.clone()).pow(2)
}

#[test]
fn test_minima() {
    let minima = half_divide_method(Posit::from(-2.0), Posit::from(4.65), Posit::from(0.000001), parabola, LIMIT);
    let err = (RES.clone() - minima.clone()).abs();
    println!(
        "Minima: {:?}\nRes: {:?}\nError: {:?}",
        minima.clone(),
        &*RES,
        &err,
    );
    assert!(err < EPS.clone());
}
