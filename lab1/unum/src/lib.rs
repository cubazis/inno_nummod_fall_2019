mod posit;
mod regime;

#[cfg(test)]
mod inner_tests;
//mod tests_float;
mod tests_posit;

// Exports

//#[cfg(test)]
//mod inner_tests {
//    use super::*;
//
//    #[test]
//    fn carry_exp_positive() {
//        let lhs = Posit32(0x76000000); // + 16^2  * 2^3 * (1 + 0)
//        let rhs = Posit32(0x48000000); // + 16^0  * 2^1 * (1 + 0)
//
//        let res: Posit32 = lhs * rhs;
//        let expected = Posit32(0x78000000); // + 16^3 * 2^0 * (1 + 0)
//        assert_eq!(res, expected);
//    }
//
//    #[test]
//    fn carry_frac_positive() {
//        let lhs = Posit32(0x77ffffff); // + 16^2  * 2^3 * (1 + 33554431/33554432)
//        let rhs = Posit32(0xec000001); // + 16^2  * 2^3 * (1 + 1/33554432)
//
//        let res: Posit32 = lhs * rhs;
//        let expected = Posit32(0x78000000); // + 16^3 * 2^0 * (1 + 0)
//        assert_eq!(res, expected);
//    }
//
//}
