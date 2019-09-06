const RES: f64 = 1.0 + 1.0 / 3.0;
const LIMIT: u32 = 10000000;
const EPS: f64 = 10e-8;

fn half_divide_method(
    mut a: f64,
    mut b: f64,
    stop: f64,
    f: impl Fn(f64) -> f64,
    iterations: u32,
) -> f64 {
    let e = 0.5;
    let mut x = (a + b) / 2.0;
    let mut i = 0;
    while f(x) >= stop && i < iterations {
        x = (a + b) / 2.0;
        let f1 = f(x - e);
        let f2 = f(x + e);
        if f1 < f2 {
            b = x;
        } else {
            a = x;
        }
        i += 1;
    }
    (a + b) / 2.0
}

fn parabola(x: f64) -> f64 {
    (x - 4.0 / 3.0).powf(2.0)
}

#[test]
#[should_panic]
fn test_minima() {
    let minima = half_divide_method(-2.0, 4.65, 0.0001, parabola, LIMIT);
    println!(
        "Minima: {}; Res: {}; Error: {}",
        minima,
        RES,
        (RES - minima).abs()
    );
    assert!((minima - RES).abs() < EPS);
}
