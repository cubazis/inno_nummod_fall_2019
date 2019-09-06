use lazy_static::lazy_static;

use crate::Posit;

const LIMIT: u32 = 4801;
lazy_static! {
    static ref RES: Posit = Posit::from(1.644725755214774951);
    static ref EPS: Posit = Posit::from(10e-18);
}

#[test]
fn test_sum() {
    let mut sum = Posit::zero();
    for i in 1..LIMIT {
        sum = sum + Posit::from(1.0 / (i as f64)).pow(2);
        dbg!(i);
    }
    let err = (sum.clone() - RES.clone()).abs();
    println!("Sum: {:?}\nRes: {:?}\nErr: {:?}\nEps: {:?}", &sum, &*RES, &err, &*EPS);
    assert!(err < EPS.clone());
}
