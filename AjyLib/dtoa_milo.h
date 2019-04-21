#pragma once
// the code is copy & refact from following URL:
// https://github.com/miloyip/itoa-benchmark/tree/master/src

struct DiyFp
{
	DiyFp();
	DiyFp(uint64 f, int32 e);
	DiyFp(double d);

	DiyFp operator-(DiyFp const& rhs) const;
	DiyFp operator*(DiyFp const& rhs) const;

	DiyFp Normalize() const;
	DiyFp NormalizeBoundary() const;
	void NormalizedBoundaries(DiyFp* minus, DiyFp* plus) const;

	static int32 const kDiySignificandSize;
	static int32 const kDpSignificandSize;
	static int32 const kDpExponentBias;
	static int32 const kDpMinExponent;
	static uint64 const kDpExponentMask;
	static uint64 const kDpSignificandMask;
	static uint64 const kDpHiddenBit;

	uint64 f;
	int32 e;

	static DiyFp GetCachedPower(int e, int* K);
	static void GrisuRound(char* buffer, int32 len, uint64 delta, uint64 rest, uint64 ten_kappa, uint64 wp_w);
	static unsigned CountDecimalDigit32(uint n);
	static void DigitGen(DiyFp const& W, DiyFp const& Mp, uint64 delta, char* buffer, int* len, int* K);
	static void Grisu2(double value, char* buffer, int* length, int* K);
	static char const* GetDigitsLut();
	static int WriteExponent(int32 K, char* buffer);
	static int Prettify(char* buffer, int length, int k);
	static int dtoa_milo(double value, char* buffer);
};
