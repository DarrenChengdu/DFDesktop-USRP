#ifndef DF_LIB_H
#define DF_LIB_H

#include <cmath>
#include <cassert>

namespace df_lib {

	template<class _Type>
	inline _Type angle(const _Type &deg)
	{
		double rad = deg / 180.0 * 3.1415926;
		double out = atan2(sin(rad), cos(rad));

		return (_Type)(out / 3.1415926 * 180.0);
	}

	template<class _Type>
	inline _Type mod(_Type X, _Type Y)
	{
		int n = floor(X * 1.0 / Y);
		return X - n * Y;
	}

	template<class _Type>
	inline _Type max2(_Type a, _Type b) {
		return ((a > b) ? a : b);
	}

	template<class _Type>
	inline _Type min2(_Type a, _Type b) {
		return ((a < b) ? a : b);
	}

	template<class _Type>
	inline _Type max3(_Type a, _Type b, _Type c) {
		_Type d = ((a > b) ? a : b);
		return ((d > c) ? d : c);
	}

	template<class _Type>
	inline _Type min3(_Type a, _Type b, _Type c) {
		_Type d = ((a < b) ? a : b);
		return ((d < c) ? d : c);
	}

	template<class _Type>
	inline void clamp(_Type &val, _Type min, _Type max) {
		if (val < min) val = min;
		if (val > max) val = max;
	}

	// Interpolation between a -> b
	// p is a 0.0 -> 1.0 value
	template<typename _Type>
	inline _Type lerp(_Type a, _Type b, float p) {
		return a * (1.f - p) + b * p;
	}

	// dB to linear voltage correction
	// Used for path-loss corrections
	inline void db_to_lin(float *srcDst, int len) {
		for (int i = 0; i < len; i++) {
			srcDst[i] = pow(10, srcDst[i] * 0.05);
		}
	}

	// Convert dBm value to mV
	inline void dbm_to_mv(float *srcDst, int len) {
		for (int i = 0; i < len; i++) {
			srcDst[i] = pow(10, (srcDst[i] + 46.9897) * 0.05);
		}
	}

	/*
	* a ^ n
	*/
	inline double power(double a, int n)
	{
		assert(n >= 0);

		if (n == 0)
			return 1.0;

		int r = a;
		for (int i = 0; i < n - 1; i++)
			r *= a;
		return r;
	}

	// Get next multiple of 'factor' after start
	// e.g. factor = 25, start = 38, return 50
	// if start is a multiple of factor, return start
	inline double next_multiple_of(double factor, double start)
	{
		if (fmod(start, factor) == 0.0)
			return start;

		int m = int(start / factor) + 1;
		return m * factor;
	}

	// Return value [0.0, 1.0], represent fraction of
	//   f between [start, stop]
	inline double frac_between(double start, double stop, double f) {
		return (f - start) / (stop - start);
	}

	inline int factorial(int x)
	{
		int t;

		if (x == 0)
		{
			t = 1;
		}
		else
		{
			t = x * factorial(x - 1);
		}

		return t;
	}

	inline int nchoosek(int n, int k)
	{
		return factorial(n) / (factorial(k) * factorial(n - k));
	}

	template<typename _Type>
	inline _Type sind(_Type deg)
	{
		// alpha * 3.1415926 / 180.0)
		return sin(deg * 0.0174533);
	}

	template<typename _Type>
	inline _Type cosd(_Type deg)
	{
		// alpha * 3.1415926 / 180.0)
		return cos(deg * 0.0174533);
	}

}

#endif
