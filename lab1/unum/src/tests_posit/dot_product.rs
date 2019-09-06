use lazy_static::lazy_static;

use crate::Posit;

lazy_static! {
    static ref BS: Vec<usize> = vec![5, 8, 12, 15, 20];
    static ref EPS: Posit = 10e-8.into();
    static ref RES: Posit = 8779.0.into();
}

fn init_v1(a: usize) -> Vec<Posit> {
    vec![
        Posit::from(10f64).pow(a),
        1223f64.into(),
        Posit::from(10f64).pow(a - 1),
        Posit::from(10f64).pow(a - 2),
        3f64.into(),
        -(Posit::from(10f64).pow(a - 5)),
    ]
}

fn init_v2(a: usize) -> Vec<Posit> {
    vec![
        Posit::from(10f64).pow(a),
        2f64.into(),
        -(Posit::from(10f64).pow(a + 1)),
        Posit::from(10f64).pow(a),
        2111f64.into(),
        Posit::from(10f64).pow(a + 3),
    ]
}

fn dot(left: Vec<Posit>, right: Vec<Posit>) -> Posit {
    let mut res = Posit::zero();
    for (a, b) in left.into_iter().zip(right.into_iter()) {
        dbg!(&a, &b);
        let prod = a * b;
        dbg!(&res, &prod);
        res = res + prod;
        dbg!(&res);
    }
    res
}

#[test]
fn test_a5() {
    let x = init_v1(5);
    for &i in BS.iter() {
        dbg!(i);
        let y = init_v2(i);
        let dot = dot(x.clone(), y);
        println!("{:?}", dot);
        dbg!(&dot, RES.clone());
        let res = (dot - RES.clone()).abs();
        dbg!(&res, EPS.clone(), i);
        assert!(res < EPS.clone());
    }
}

#[test]
fn test_a10() {
    let x = init_v1(10);
    for &i in BS.iter() {
        let y = init_v2(i);
        let dot = dot(x.clone(), y);
        println!("{:?}", dot);
        assert!((dot - RES.clone()).abs() < EPS.clone());
    }
}
