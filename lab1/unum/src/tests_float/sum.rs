const EPS: f64 = 10e-18;
const RES: f64 = 1.644725755214774951;
const LIMIT: u32 = 4801;

#[test]
#[should_panic]
fn test_sum() {
    let mut sum = 0.0;
    for i in 1..LIMIT {
        sum += (i as f64).powf(-2.0);
    }
    println!("Sum: {}, Res: {}", sum, RES);
    assert!((sum - RES).abs() < EPS)
}
