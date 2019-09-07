use nalgebra::Vector6;

const BS: [f64; 5] = [5.0, 8.0, 12.0, 15.0, 20.0];
const EPS: f64 = 10e-8;
const RES: f64 = 8779.0;

fn init_v1(a: f64) -> Vector6<f64> {
    [
        10f64.powf(a),
        1223f64,
        10f64.powf(a - 1.0),
        10f64.powf(a - 2.0),
        3f64,
        -(10f64.powf(a - 5.0)),
    ]
    .into()
}

fn init_v2(a: f64) -> Vector6<f64> {
    [
        10f64.powf(a),
        2f64,
        -(10f64.powf(a + 1.0)),
        10f64.powf(a),
        2111f64,
        10f64.powf(a + 3.0),
    ]
    .into()
}

#[test]
fn test_a5() {
    let x = init_v1(5.0);
    for &i in &BS {
        let y = init_v2(i);
        let dot = x.dot(&y);
        println!("{:?}", dot);
        assert!((dot - RES).abs() < EPS);
    }
}

#[test]
fn test_a10() {
    let x = init_v1(10.0);
    for &i in &BS {
        let y = init_v2(i);
        let dot = x.dot(&y);
        println!("{:?}", dot);
        assert!((dot - RES).abs() < EPS);
    }
}
