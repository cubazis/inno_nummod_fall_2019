//
// Created by cubazis on 18.08.2019.
//

#include "posit.h"
#pragma warning( disable: 4146)

void checkExtraP32TwoBits(double f32, double temp, bool* bitsNPlusOne, bool* bitsMore) {
	temp /= 2;
	if (temp <= f32) {
		*bitsNPlusOne = 1;
		f32 -= temp;
	}
	if (f32 > 0)
		* bitsMore = 1;
}
uint_fast32_t convertFractionP32(double f32, uint_fast16_t fracLength, bool* bitsNPlusOne, bool* bitsMore) {

	uint_fast32_t frac = 0;

	if (f32 == 0) return 0;
	else if (f32 == INFINITY) return 0x80000000;

	f32 -= 1; //remove hidden bit
	if (fracLength == 0)
		checkExtraP32TwoBits(f32, 1.0, bitsNPlusOne, bitsMore);
	else {
		double temp = 1;
		while (true) {
			temp /= 2;
			if (temp <= f32) {
				f32 -= temp;
				fracLength--;
				frac = (frac << 1) + 1; //shift in one
				if (f32 == 0) {
					frac <<= (uint_fast16_t)fracLength;
					break;
				}

				if (fracLength == 0) {
					checkExtraP32TwoBits(f32, temp, bitsNPlusOne, bitsMore);
					break;
				}
			}
			else {

				frac <<= 1; //shift in a zero
				fracLength--;
				if (fracLength == 0) {
					checkExtraP32TwoBits(f32, temp, bitsNPlusOne, bitsMore);
					break;
				}
			}
		}
	}

	return frac;
}


posit subMagsP32(uint_fast32_t uiA, uint_fast32_t uiB) {

	uint_fast16_t regA, regB;
	uint_fast64_t frac64A = 0, frac64B = 0;
	uint_fast32_t fracA = 0, regime, tmp;
	bool sign, regSA, regSB, ecarry = 0, bitNPlusOne = 0, bitsMore = 0;
	int_fast8_t kA = 0;
	int_fast32_t expA = 0;
	int_fast16_t shiftRight;
	union ui32_p32 uZ;

	sign = signP32UI(uiA);
	if (sign)
		uiA = -uiA & 0xFFFFFFFF;
	else
		uiB = -uiB & 0xFFFFFFFF;

	if (uiA == uiB) { //essential, if not need special handling
		uZ.ui = 0;
		return uZ.p;
	}
	if ((int_fast32_t)uiA < (int_fast32_t)uiB) {
		uiA ^= uiB;
		uiB ^= uiA;
		uiA ^= uiB;
		(sign) ? (sign = 0) : (sign = 1); //A becomes B
	}
	regSA = signregP32UI(uiA);
	regSB = signregP32UI(uiB);

	tmp = (uiA << 2) & 0xFFFFFFFF;
	if (regSA) {
		while (tmp >> 31) {
			kA++;
			tmp = (tmp << 1) & 0xFFFFFFFF;
		}
	}
	else {
		kA = -1;
		while (!(tmp >> 31)) {
			kA--;
			tmp = (tmp << 1) & 0xFFFFFFFF;
		}
		tmp &= 0x7FFFFFFF;
	}

	expA = tmp >> 29; //to get 2 bits
	frac64A = ((0x40000000ULL | tmp << 1) & 0x7FFFFFFFULL) << 32;
	shiftRight = kA;


	tmp = (uiB << 2) & 0xFFFFFFFF;
	if (regSB) {
		while (tmp >> 31) {
			shiftRight--;
			tmp = (tmp << 1) & 0xFFFFFFFF;
		}

	}
	else {
		shiftRight++;
		while (!(tmp >> 31)) {
			shiftRight++;
			tmp = (tmp << 1) & 0xFFFFFFFF;
		}
		tmp &= 0x7FFFFFFF;

	}
	frac64B = ((0x40000000ULL | tmp << 1) & 0x7FFFFFFFULL) << 32;

	//This is 4kZ + expZ; (where kZ=kA-kB and expZ=expA-expB)
	shiftRight = (shiftRight << 2) + expA - (tmp >> 29);
	if (shiftRight > 63) {
		uZ.ui = uiA;
		if (sign) uZ.ui = -uZ.ui & 0xFFFFFFFF;
		return uZ.p;
	}
	else
		(frac64B >>= shiftRight);

	frac64A -= frac64B;

	while ((frac64A >> 59) == 0) {
		kA--;
		frac64A <<= 4;
	}
	ecarry = (0x4000000000000000 & frac64A);//(0x4000000000000000 & frac64A)>>62;
	while (!ecarry) {
		if (expA == 0) {
			kA--;
			expA = 3;
		}
		else
			expA--;
		frac64A <<= 1;
		ecarry = (0x4000000000000000 & frac64A);
	}

	if (kA < 0) {
		regA = -kA;
		regSA = 0;
		regime = 0x40000000 >> regA;
	}
	else {
		regA = kA + 1;
		regSA = 1;
		regime = 0x7FFFFFFF - (0x7FFFFFFF >> regA);
	}
	if (regA > 30) {
		//max or min pos. exp and frac does not matter.
		(regSA) ? (uZ.ui = 0x7FFFFFFF) : (uZ.ui = 0x1);
	}
	else {
		//remove hidden bits
		frac64A = (frac64A & 0x3FFFFFFFFFFFFFFF) >> (regA + 2); // 2 bits exp

		fracA = frac64A >> 32;

		if (regA <= 28) {
			bitNPlusOne |= (0x80000000 & frac64A);
			expA <<= (28 - regA);
		}
		else {
			if (regA == 30) {
				bitNPlusOne = expA & 0x2;
				bitsMore = (expA & 0x1);
				expA = 0;
			}
			else if (regA == 29) {
				bitNPlusOne = expA & 0x1;
				expA >>= 1;
			}
			if (fracA > 0) {
				fracA = 0;
				bitsMore = 1;
			}

		}

		uZ.ui = packToP32UI(regime, expA, fracA);
		//n+1 frac bit is 1. Need to check if another bit is 1 too if not round to even
		if (bitNPlusOne) {
			(0x7FFFFFFF & frac64A) ? (bitsMore = 1) : (bitsMore = 0);
			uZ.ui += (uZ.ui & 1) | bitsMore;
		}
	}
	if (sign) uZ.ui = -uZ.ui & 0xFFFFFFFF;
	return uZ.p;
}

posit addMagsP32(uint_fast32_t uiA, uint_fast32_t uiB) {
	uint_fast16_t regA, regB;
	uint_fast64_t frac64A = 0, frac64B = 0;
	uint_fast32_t fracA = 0, regime, tmp;
	bool sign, regSA, regSB, rcarry = 0, bitNPlusOne = 0, bitsMore = 0;
	int_fast8_t kA = 0;
	int_fast32_t expA;
	int_fast16_t shiftRight;
	union ui32_p32 uZ;

	sign = signP32UI(uiA);
	if (sign) {
		uiA = -uiA & 0xFFFFFFFF;
		uiB = -uiB & 0xFFFFFFFF;
	}

	if ((int_fast32_t)uiA < (int_fast32_t)uiB) {
		uiA ^= uiB;
		uiB ^= uiA;
		uiA ^= uiB;
	}
	regSA = signregP32UI(uiA);
	regSB = signregP32UI(uiB);

	tmp = (uiA << 2) & 0xFFFFFFFF;
	if (regSA) {
		while (tmp >> 31) {
			kA++;
			tmp = (tmp << 1) & 0xFFFFFFFF;
		}
	}
	else {
		kA = -1;
		while (!(tmp >> 31)) {
			kA--;
			tmp = (tmp << 1) & 0xFFFFFFFF;
		}
		tmp &= 0x7FFFFFFF;
	}

	expA = tmp >> 29; //to get 2 bits
	frac64A = ((0x40000000ULL | tmp << 1) & 0x7FFFFFFFULL) << 32;
	shiftRight = kA;

	tmp = (uiB << 2) & 0xFFFFFFFF;
	if (regSB) {
		while (tmp >> 31) {
			shiftRight--;
			tmp = (tmp << 1) & 0xFFFFFFFF;
		}
	}
	else {
		shiftRight++;
		while (!(tmp >> 31)) {
			shiftRight++;
			tmp = (tmp << 1) & 0xFFFFFFFF;
		}
		tmp &= 0x7FFFFFFF;
	}
	frac64B = ((0x40000000ULL | tmp << 1) & 0x7FFFFFFFULL) << 32;
	//This is 4kZ + expZ; (where kZ=kA-kB and expZ=expA-expB)
	shiftRight = (shiftRight << 2) + expA - (tmp >> 29);

	//Manage CLANG (LLVM) compiler when shifting right more than number of bits
	(shiftRight > 63) ? (frac64B = 0) : (frac64B >>= shiftRight); //frac64B >>= shiftRight

	frac64A += frac64B;

	rcarry = 0x8000000000000000 & frac64A; //first left bit
	if (rcarry) {
		expA++;
		if (expA > 3) {
			kA++;
			expA &= 0x3;
		}
		frac64A >>= 1;
	}
	if (kA < 0) {
		regA = -kA;
		regSA = 0;
		regime = 0x40000000 >> regA;
	}
	else {
		regA = kA + 1;
		regSA = 1;
		regime = 0x7FFFFFFF - (0x7FFFFFFF >> regA);
	}

	if (regA > 30) {
		//max or min pos. exp and frac does not matter.
		(regSA) ? (uZ.ui = 0x7FFFFFFF) : (uZ.ui = 0x1);
	}
	else {
		//remove hidden bits
		frac64A = (frac64A & 0x3FFFFFFFFFFFFFFF) >> (regA + 2); // 2 bits exp

		fracA = frac64A >> 32;

		if (regA <= 28) {
			bitNPlusOne |= (0x80000000 & frac64A);
			expA <<= (28 - regA);
		}
		else {
			if (regA == 30) {
				bitNPlusOne = expA & 0x2;
				bitsMore = (expA & 0x1);
				expA = 0;
			}
			else if (regA == 29) {
				bitNPlusOne = expA & 0x1;
				expA >>= 1;
			}
			if (fracA > 0) {
				fracA = 0;
				bitsMore = 1;
			}
		}

		uZ.ui = packToP32UI(regime, expA, fracA);
		//n+1 frac bit is 1. Need to check if another bit is 1 too if not round to even
		if (bitNPlusOne) {
			(0x7FFFFFFF & frac64A) ? (bitsMore = 1) : (bitsMore = 0);
			uZ.ui += (uZ.ui & 1) | bitsMore;
		}
	}
	if (sign) uZ.ui = -uZ.ui & 0xFFFFFFFF;
	return uZ.p;
}

posit p32_add(posit a, posit b) {
	union ui32_p32 uA, uB, uZ;
	uint_fast32_t uiA, uiB;

	uA.p = a;
	uiA = uA.ui;
	uB.p = b;
	uiB = uB.ui;

#ifdef SOFTPOSIT_EXACT
	uZ.ui.exact = (uiA.ui.exact & uiB.ui.exact);
#endif

	//Zero or infinity
	if (uiA == 0 || uiB == 0) { // Not required but put here for speed
#ifdef SOFTPOSIT_EXACT
		uZ.ui.v = uiA | uiB;
		uZ.ui.exact = (uiA.ui.exact & uiB.ui.exact);
#else
		uZ.ui = uiA | uiB;
#endif
		return uZ.p;
	}
	else if (uiA == 0x80000000 || uiB == 0x80000000) {
		//printf("in infinity\n");
#ifdef SOFTPOSIT_EXACT
		uZ.ui.v = 0x80000000;
		uZ.ui.exact = 0;
#else
		uZ.ui = 0x80000000;
#endif
		return uZ.p;
	}

	//different signs
	if ((uiA ^ uiB) >> 31)
		return subMagsP32(uiA, uiB);
	else
		return addMagsP32(uiA, uiB);

}

posit p32_sub(posit a, posit b) {


	union ui32_p32 uA, uB, uZ;
	uint_fast32_t uiA, uiB;

	uA.p = a;
	uiA = uA.ui;
	uB.p = b;
	uiB = uB.ui;

#ifdef SOFTPOSIT_EXACT
	uZ.ui.exact = (uiA.ui.exact & uiB.ui.exact);
#endif

	//infinity
	if (uiA == 0x80000000 || uiB == 0x80000000) {
#ifdef SOFTPOSIT_EXACT
		uZ.ui.v = 0x80000000;
		uZ.ui.exact = 0;
#else
		uZ.ui = 0x80000000;
#endif
		return uZ.p;
	}
	//Zero
	else if (uiA == 0 || uiB == 0) {
#ifdef SOFTPOSIT_EXACT
		uZ.ui.v = (uiA | -uiB);
		uZ.ui.exact = 0;
#else
		uZ.ui = (uiA | -uiB);
#endif
		return uZ.p;
	}

	//different signs
	if ((uiA ^ uiB) >> 31)
		return addMagsP32(uiA, (-uiB & 0xFFFFFFFF));
	else
		return subMagsP32(uiA, (-uiB & 0xFFFFFFFF));



}

posit p32_mul(posit pA, posit pB) {


	union ui32_p32 uA, uB, uZ;
	uint_fast32_t uiA, uiB;
	uint_fast32_t regA, fracA, regime, tmp;
	bool signA, signB, signZ, regSA, regSB, bitNPlusOne = 0, bitsMore = 0, rcarry;
	int_fast32_t expA;
	int_fast8_t kA = 0;
	uint_fast64_t frac64Z;

	uA.p = pA;
	uiA = uA.ui;
	uB.p = pB;
	uiB = uB.ui;

#ifdef SOFTPOSIT_EXACT
	uZ.ui.exact = (uiA.ui.exact & uiB.ui.exact);
#endif
	//NaR or Zero
	if (uiA == 0x80000000 || uiB == 0x80000000) {

#ifdef SOFTPOSIT_EXACT
		uZ.ui.v = 0x80000000;
		uZ.ui.exact = 0;
#else
		uZ.ui = 0x80000000;
#endif
		return uZ.p;
	}
	else if (uiA == 0 || uiB == 0) {
#ifdef SOFTPOSIT_EXACT

		uZ.ui.v = 0;
		if ((uiA == 0 && uiA.ui.exact) || (uiB == 0 && uiB.ui.exact))
			uZ.ui.exact = 1;
		else
			uZ.ui.exact = 0;
#else
		uZ.ui = 0;
#endif
		return uZ.p;
	}

	signA = signP32UI(uiA);
	signB = signP32UI(uiB);
	signZ = signA ^ signB;

	if (signA) uiA = (-uiA & 0xFFFFFFFF);
	if (signB) uiB = (-uiB & 0xFFFFFFFF);

	regSA = signregP32UI(uiA);
	regSB = signregP32UI(uiB);


	tmp = (uiA << 2) & 0xFFFFFFFF;
	if (regSA) {

		while (tmp >> 31) {

			kA++;
			tmp = (tmp << 1) & 0xFFFFFFFF;
		}
	}
	else {
		kA = -1;
		while (!(tmp >> 31)) {
			kA--;
			tmp = (tmp << 1) & 0xFFFFFFFF;
		}
		tmp &= 0x7FFFFFFF;
	}
	expA = tmp >> 29; //to get 2 bits
	fracA = ((tmp << 1) | 0x40000000) & 0x7FFFFFFF;

	tmp = (uiB << 2) & 0xFFFFFFFF;
	if (regSB) {
		while (tmp >> 31) {
			kA++;
			tmp = (tmp << 1) & 0xFFFFFFFF;
		}
	}
	else {
		kA--;
		while (!(tmp >> 31)) {
			kA--;
			tmp = (tmp << 1) & 0xFFFFFFFF;
		}
		tmp &= 0x7FFFFFFF;
	}
	expA += tmp >> 29;
	frac64Z = (uint_fast64_t)fracA * (((tmp << 1) | 0x40000000) & 0x7FFFFFFF);

	if (expA > 3) {
		kA++;
		expA &= 0x3; // -=4
	}

	rcarry = frac64Z >> 61;//3rd bit of frac64Z
	if (rcarry) {
		expA++;
		if (expA > 3) {
			kA++;
			expA &= 0x3;
		}
		frac64Z >>= 1;
	}

	if (kA < 0) {
		regA = -kA;
		regSA = 0;
		regime = 0x40000000 >> regA;
	}
	else {
		regA = kA + 1;
		regSA = 1;
		regime = 0x7FFFFFFF - (0x7FFFFFFF >> regA);
	}



	if (regA > 30) {
		//max or min pos. exp and frac does not matter.
		(regSA) ? (uZ.ui = 0x7FFFFFFF) : (uZ.ui = 0x1);
	}
	else {
		//remove carry and rcarry bits and shift to correct position (2 bits exp, so + 1 than 16 bits)
		frac64Z = (frac64Z & 0xFFFFFFFFFFFFFFF) >> regA;
		fracA = (uint_fast32_t)(frac64Z >> 32);

		if (regA <= 28) {
			bitNPlusOne |= (0x80000000 & frac64Z);
			expA <<= (28 - regA);
		}
		else {
			if (regA == 30) {
				bitNPlusOne = expA & 0x2;
				bitsMore = (expA & 0x1);
				expA = 0;
			}
			else if (regA == 29) {
				bitNPlusOne = expA & 0x1;
				expA >>= 1; //taken care of by the pack algo
			}
			if (fracA > 0) {
				fracA = 0;
				bitsMore = 1;
			}

		}
		//sign is always zero
		uZ.ui = packToP32UI(regime, expA, fracA);
		//n+1 frac bit is 1. Need to check if another bit is 1 too if not round to even
		if (bitNPlusOne) {
			(0x7FFFFFFF & frac64Z) ? (bitsMore = 1) : (bitsMore = 0);
			uZ.ui += (uZ.ui & 1) | bitsMore;
		}
	}

	if (signZ) uZ.ui = -uZ.ui & 0xFFFFFFFF;
	return uZ.p;

}

double convertP32ToDouble(posit pA) {
	union ui32_p32 uA;
	union ui64_double uZ;
	uint_fast32_t uiA, tmp = 0;
	uint_fast64_t expA = 0, uiZ, fracA = 0;
	bool signA = 0, regSA;
	int_fast32_t scale, kA = 0;

	uA.p = pA;
	uiA = uA.ui;

	if (uA.ui == 0)
		return 0;
	else if (uA.ui == 0x80000000)
		return NAN;

	else {
		signA = signP32UI(uiA);
		if (signA) uiA = (-uiA & 0xFFFFFFFF);
		regSA = signregP32UI(uiA);
		tmp = (uiA << 2) & 0xFFFFFFFF;
		if (regSA) {

			while (tmp >> 31) {
				kA++;
				tmp = (tmp << 1) & 0xFFFFFFFF;
			}
		}
		else {
			kA = -1;
			while (!(tmp >> 31)) {
				kA--;
				tmp = (tmp << 1) & 0xFFFFFFFF;
			}
			tmp &= 0x7FFFFFFF;
		}
		expA = tmp >> 29; //to get 2 bits

		fracA = (((uint64_t)tmp << 3) & 0xFFFFFFFF) << 20;

		expA = (((kA << 2) + expA) + 1023) << 52;
		uiZ = expA + fracA + (((uint64_t)signA & 0x1) << 63);

		uZ.ui = uiZ;
		return uZ.d;
	}
}

posit convertDoubleToP32(double f32) {

	union ui32_p32 uZ;
	bool sign, regS;
	uint_fast32_t reg, frac = 0;
	int_fast32_t exp = 0;
	bool bitNPlusOne = 0, bitsMore = 0;

	(f32 >= 0) ? (sign = 0) : (sign = 1);

	if (f32 == 0) {
		uZ.ui = 0;
		return uZ.p;
	}
	else if (f32 == INFINITY || f32 == -INFINITY || f32 == NAN) {
		uZ.ui = 0x80000000;
		return uZ.p;
	}
	else if (f32 == 1) {
		uZ.ui = 0x40000000;
		return uZ.p;
	}
	else if (f32 == -1) {
		uZ.ui = 0xC0000000;
		return uZ.p;
	}
	else if (f32 >= 1.329227995784916e+36) {
		//maxpos
		uZ.ui = 0x7FFFFFFF;
		return uZ.p;
	}
	else if (f32 <= -1.329227995784916e+36) {
		// -maxpos
		uZ.ui = 0x80000001;
		return uZ.p;
	}
	else if (f32 <= 7.52316384526264e-37 && !sign) {
		//minpos
		uZ.ui = 0x1;
		return uZ.p;
	}
	else if (f32 >= -7.52316384526264e-37 && sign) {
		//-minpos
		uZ.ui = 0xFFFFFFFF;
		return uZ.p;
	}
	else if (f32 > 1 || f32 < -1) {
		if (sign) {
			//Make negative numbers positive for easier computation
			f32 = -f32;
		}

		regS = 1;
		reg = 1; //because k = m-1; so need to add back 1
		// minpos
		if (f32 <= 7.52316384526264e-37) {
			uZ.ui = 1;
		}
		else {
			//regime
			while (f32 >= 16) {
				f32 *= 0.0625;  // f32/=16;
				reg++;
			}
			while (f32 >= 2) {
				f32 *= 0.5;
				exp++;
			}

			int8_t fracLength = 28 - reg;

			if (fracLength < 0) {
				//in both cases, reg=29 and 30, e is n+1 bit and frac are sticky bits
				if (reg == 29) {
					bitNPlusOne = exp & 0x1;
					exp >>= 1; //taken care of by the pack algo
				}
				else {//reg=30
					bitNPlusOne = exp >> 1;
					bitsMore = exp & 0x1;
					exp = 0;
				}
				if (f32 != 1) {//because of hidden bit
					bitsMore = 1;
					frac = 0;
				}
			}
			else
				frac = convertFractionP32(f32, fracLength, &bitNPlusOne, &bitsMore);


			if (reg > 30) {
				(regS) ? (uZ.ui = 0x7FFFFFFF) : (uZ.ui = 0x1);
			}
			//rounding off fraction bits
			else {

				uint_fast32_t regime = 1;
				if (regS) regime = ((1 << reg) - 1) << 1;
				if (reg <= 28)  exp <<= (28 - reg);
				uZ.ui = ((uint32_t)(regime) << (30 - reg)) + ((uint32_t)exp) + ((uint32_t)(frac));
				uZ.ui += (bitNPlusOne & (uZ.ui & 1)) | (bitNPlusOne & bitsMore);
			}
			if (sign) uZ.ui = -uZ.ui & 0xFFFFFFFF;

		}
	}
	else if (f32 < 1 || f32 > -1) {
		if (sign) {
			//Make negative numbers positive for easier computation
			f32 = -f32;
		}
		regS = 0;
		reg = 0;

		//regime
		while (f32 < 1) {
			f32 *= 16;
			reg++;
		}

		while (f32 >= 2) {
			f32 *= 0.5;
			exp++;
		}


		//only possible combination for reg=15 to reach here is 7FFF (maxpos) and FFFF (-minpos)
		//but since it should be caught on top, so no need to handle
		int_fast8_t fracLength = 28 - reg;
		if (fracLength < 0) {
			//in both cases, reg=29 and 30, e is n+1 bit and frac are sticky bits
			if (reg == 29) {
				bitNPlusOne = exp & 0x1;
				exp >>= 1; //taken care of by the pack algo
			}
			else {//reg=30
				bitNPlusOne = exp >> 1;
				bitsMore = exp & 0x1;
				exp = 0;
			}
			if (f32 != 1) {//because of hidden bit
				bitsMore = 1;
				frac = 0;
			}
		}
		else
			frac = convertFractionP32(f32, fracLength, &bitNPlusOne, &bitsMore);


		if (reg > 30) {
			(regS) ? (uZ.ui = 0x7FFFFFFF) : (uZ.ui = 0x1);
		}
		//rounding off fraction bits
		else {

			uint_fast32_t regime = 1;
			if (regS) regime = ((1 << reg) - 1) << 1;
			if (reg <= 28)  exp <<= (28 - reg);
			uZ.ui = ((uint32_t)(regime) << (30 - reg)) + ((uint32_t)exp) + ((uint32_t)(frac));
			uZ.ui += (bitNPlusOne & (uZ.ui & 1)) | (bitNPlusOne & bitsMore);
		}
		if (sign) uZ.ui = -uZ.ui & 0xFFFFFFFF;

	}
	else {
		//NaR - for NaN, INF and all other combinations
		uZ.ui = 0x80000000;
	}
	return uZ.p;
}

posit power(posit num, posit p) {

	return convertDoubleToP32(pow(convertP32ToDouble(num), convertP32ToDouble(p)));
}


int test_function(void)
{
	return 228;
}

