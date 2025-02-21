/*
Author: Inan Evin
www.inanevin.com

Copyright 2018 Inan Evin

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, 
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions 
and limitations under the License.

Class: GenericMath
Timestamp: 4/8/2019 9:14:44 PM

*/

#pragma once

#ifndef GenericMath_HPP
#define GenericMath_HPP

#include "cmwc4096.hpp"
#include <cmath>
#include <cstdlib>

#include "Core/SizeDefinitions.hpp"
#include "Core/Common.hpp"
#include "Core/LinaAPI.hpp"
#define MATH_PI 3.1415926535897932f
#define MATH_TWO_PI 6.28318530717959f
#define MATH_HALF_PI 1.57079632679f
#define MATH_R_PI 0.31830988618f
#define MATH_R_TWO_PI 0.159154943091895f
#define MATH_R_HALF_PI 0.636619772367581f

#define MATH_E 2.71828182845904523536f
#define MATH_R_LN_2 1.44269504088896f
#define MATH_RAD_TO_DEG_CONV 57.2957795130823f
#define MATH_DEG_TO_RAD_CONV 0.0174532925199433f

namespace LinaEngine
{
	struct GenericMath
	{
		static constexpr FORCEINLINE int32 TruncToInt(float val)
		{
			return (int32)val;
		}

		static constexpr FORCEINLINE float TruncToFloat(float val)
		{
			return (float)TruncToInt(val);
		}

		static FORCEINLINE int32 FloorToInt(float val)
		{
			return TruncToInt(FloorToFloat(val));
		}

		static FORCEINLINE float FloorToFloat(float val)
		{
			return floorf(val);
		}

		static FORCEINLINE double FloorToDouble(double val)
		{
			return floor(val);
		}

		static FORCEINLINE int32 RoundToInt(float val)
		{
			return CeilToInt(val - 0.5f);
		}

		static FORCEINLINE float RoundToFloat(float val)
		{
			return CeilToFloat(val - 0.5f);
		}

		static FORCEINLINE int32 CeilToInt(float val)
		{
			return TruncToInt(ceilf(val));
		}

		static FORCEINLINE float CeilToFloat(float val)
		{
			return ceilf(val);
		}

		static FORCEINLINE double CeilToDouble(double val)
		{
			return ceil(val);
		}

		static FORCEINLINE float Fractional(float val)
		{
			return val - TruncToFloat(val);
		}

		static FORCEINLINE float Exp(float val) { return expf(val); }
		static FORCEINLINE float Ln(float val) { return logf(val); }
		static FORCEINLINE float Logx(float base, float val) { return Ln(val) / Ln(base); }
		static FORCEINLINE float Log2(float val) { return Ln(val) * MATH_R_LN_2; }

		static FORCEINLINE float Fmod(float num, float den) { return fmodf(num, den); }
		static FORCEINLINE float Sin(float val) { return sinf(val); }
		static FORCEINLINE float Asin(float val) { return asinf(val); }
		static FORCEINLINE float Cos(float val) { return cosf(val); }
		static FORCEINLINE float Acos(float val) { return acosf(val); }
		static FORCEINLINE float Tan(float val) { return tanf(val); }
		static FORCEINLINE float Atan(float val) { return atanf(val); }
		static FORCEINLINE float Atan(float val, float val2) { return atan2f(val, val2); }
		static FORCEINLINE float Pow(float val, float power) { return powf(val, power); }
		static FORCEINLINE float Sqrt(float val) { return sqrtf(val); }

		template<typename T>
		static constexpr FORCEINLINE T Select(const T& cmp,
			const T& valIfGreaterOrEqualToZero,
			const T& valIfLessZero)
		{
			return cmp >= (T)(0) ? valIfGreaterOrEqualToZero : valIfLessZero;
		}

		static FORCEINLINE void SinCos(float* outSin, float* outCos, float angle)
		{
			// If angle is outside range of [0, 2*pi], adjust it so it is.
			// Using fmod gives worse precision than the following code
			if ((angle < 0.0f) || (angle >= MATH_TWO_PI)) {
				angle -= FloorToFloat(angle * MATH_R_TWO_PI) * MATH_TWO_PI;
			}
			//		angle = Math::Fmod(angle, MATH_TWO_PI);

					// This approximation is only accurate with angles in the range
					// [-pi/2, pi/2]. If angle is outside that range, convert it to that
					// range and find the appropriate sign to adjust the result.
			angle = MATH_PI - angle;
			float Sign = -1.0f;
			if (fabsf(angle) >= MATH_HALF_PI) {
				angle = Select(angle, MATH_PI, -MATH_PI) - angle;
				Sign = 1.0f;
			}

			// Sine and cosine are calculated with their respective taylor series
			// (technically Maclaurin series, since no offset is used). The "typical"
			// series is as follows:
			//
			// sin(a) = a/(1!) - a^3/(3!) + a^5/(5!) - a^7/(7!) + ...
			// cos(a) = 1 - a^2/(2!) + a^4/(4!) - a^6/(6!) + a^8/(8!) - ...
			//
			// However, this form is inefficient for computation. Some algebra has 
			// been performed to reduce computational cost.
			//
			// For sine:
			// - The a was factored out
			// sin(a) = a * (1/(1!) - a^2/(3!) + a^4/(5!) - a^6/(7!))
			// - a^2 was factored out
			// sin(a) = a * (1/(1!) + a^2*(-1/(3!) + a^2*(1/(5!) + a^2*(...))))
			// - The factorial constants were precalculated and hardcoded, leaving
			// the final equation used. The equation terminates at the a^9/(9!) term
			// of the original taylor series.
			//
			// For cosine:
			// - a^2 was factored out
			// cos(a) = 1 + a^2*(-1/(2!) + a^2*(1/(4!) + a^2*(-1/(6!) + a^2*(...))))
			// - The factorial constants were precalculated and hardcoded, leaving
			// the final equation used. The equation terminates at the a^10/(10!)
			// term of the original taylor series.
			//
			// Note that in both equations, some of the factorial constants have
			// been modified to generate empirically more accurate results.

			float a2 = angle * angle;
			*outSin = angle * (((((-2.39e-08f * a2 + 2.7526e-06f) * a2 - 1.98409e-04f) *
				a2 + 8.3333315e-03f) * a2 - 1.666666664e-01f) * a2 + 1.0f);
			*outCos = Sign * (((((-2.605e-07f * a2 + 2.47609e-05f) * a2 - 1.3888397e-03f) *
				a2 + 4.16666418e-02f) * a2 - 4.999999963e-01f) * a2 + 1.0f);
		}

		static FORCEINLINE float Reciprocal(float val)
		{
			return 1.0f / val;
		}

		static FORCEINLINE float RSqrt(float val)
		{
			return Reciprocal(sqrtf(val));
		}

		static FORCEINLINE bool IsNaN(float val)
		{
			union {
				float f;
				uint32 i;
			} f;
			f.f = val;
			return (f.i & 0x7FFFFFFF) > 0x7F800000;
		}

		static FORCEINLINE bool IsFinite(float val)
		{
			union {
				float f;
				uint32 i;
			} f;
			f.f = val;
			return (f.i & 0x7F800000) != 0x7F800000;
		}

		static FORCEINLINE int32 Rand() { return ::rand(); }
		static FORCEINLINE void SeedRand(int32 seed) { srand((uint32)seed); }
		static FORCEINLINE float RandF() { return ::rand() / (float)RAND_MAX; }
		static FORCEINLINE float RandF(float Min, float Max) { return Lerp(Min, Max, RandF()); }

		static FORCEINLINE uint32 FloorLog2(uint32 val)
		{
			uint32 pos = 0;
			if (val >= 1 << 16) { val >>= 16; pos += 16; }
			if (val >= 1 << 8) { val >>= 8; pos += 8; }
			if (val >= 1 << 4) { val >>= 4; pos += 4; }
			if (val >= 1 << 2) { val >>= 2; pos += 2; }
			if (val >= 1 << 1) { pos += 1; }
			return (val == 0) ? 0 : pos;
		}

		static FORCEINLINE uint32 GetNumLeadingZeroes(uint32 val)
		{
			if (val == 0) {
				return 32;
			}
			return 31 - FloorLog2(val);
		}

		static FORCEINLINE uint32 CeilLog2(uint32 val)
		{
			if (val <= 1) {
				return 0;
			}
			return FloorLog2(val - 1) + 1;
		}

		static FORCEINLINE uint32 RoundUpToNextPowerOf2(uint32 val)
		{
			return 1 << CeilLog2(val);
		}

		template<typename T>
		static constexpr FORCEINLINE T Abs(const T& val)
		{
			return val >= (T)(0) ? val : -val;
		}

		// WARNING: This behavior is not consistent with Vector sign function!
		//	template<typename T>
		//	static constexpr FORCEINLINE T sign(const T& val)
		//	{
		//		return (T(0) < val) - (val < T(0));
		//	}

		template<typename T>
		static constexpr FORCEINLINE T Min(const T& val1, const T& val2)
		{
			return val1 <= val2 ? val1 : val2;
		}

		template<typename T>
		static constexpr FORCEINLINE T Min3(const T& val1, const T& val2, const T& val3)
		{
			return Min(Min(val1, val2), val3);
		}

		template<typename T>
		static constexpr FORCEINLINE T Max(const T& val1, const T& val2)
		{
			return val1 >= val2 ? val1 : val2;
		}

		template<typename T>
		static constexpr FORCEINLINE T Max3(const T& val1, const T& val2, const T& val3)
		{
			return Max(Max(val1, val2), val3);
		}

		template<typename T>
		static FORCEINLINE T Mad(const T& val1, const T& val2, const T& val3)
		{
			return val1 * val2 + val3;
		}

		template<typename T>
		static FORCEINLINE T ToDegrees(const T& val)
		{
			return val * MATH_RAD_TO_DEG_CONV;
		}

		template<typename T>
		static FORCEINLINE T ToRadians(const T& val)
		{
			return val * MATH_DEG_TO_RAD_CONV;
		}

		template<typename T, typename U>
		static FORCEINLINE T Lerp(const T& val1, const T& val2, const U& amt)
		{
			return (T)(val1 * ((U)(1) - amt) + val2 * amt);
		}

		template<typename T, typename U>
		static FORCEINLINE T CubicLerp(const T& val1, const T& val2, const U& amt)
		{
			// Cubic interp, assuming tangents are 0 at all points.
			return Lerp(val1, val2, 3 * amt*amt - 2 * amt*amt*amt);
		}

		template<typename T, typename U>
		static FORCEINLINE T CubicInterpolation(const T& val0, const T& val1, const T& val2, const T& val3, const U& amt)
		{
			// Using Catmull-Rom tangents
			U amt2 = amt * amt;
			return (
				(val3*(U(1) / U(2)) - val2 * (U(3) / U(2)) - val0 * (U(1) / U(2)) + val1 * (U(3) / U(2)))*amt*amt2
				+ (val0 - val1 * (U(5) / U(2)) + val2 * U(2) - val3 * (U(1) / U(2)))*amt2
				+ (val2*(U(1) / U(2)) - val0 * (U(1) / U(2)))*amt
				+ val1);
		}

		template<typename T, typename U>
		static FORCEINLINE T CubicInterpolationSpecifyTangents(const T& val1, const T& tan1, const T& val2, const T& tan2, const U& amt)
		{
			// Using custom tangents
			U amt2 = amt * amt;
			return (
				(tan2 - val2 * U(2) + tan1 + val1 * (U(2)))*amt*amt2
				+ (tan1*U(2) - val1 * U(3) + val2 * U(3) - tan2 * U(2))*amt2
				+ tan1 * amt
				+ val1);
		}

		template<typename T, typename U>
		static FORCEINLINE T BiLerp(const T& val00, const T& val10,
			const T& val01, const T& val11,
			const U& amtX, const U& amtY)
		{
			return Lerp(
				Lerp(val00, val10, amtX),
				Lerp(val01, val11, amtX),
				amtY);
		}

		template<typename T>
		static FORCEINLINE T Clamp(const T& val, const T& Min, const T& Max)
		{
			if (val > Max) {
				return Max;
			}
			else if (val > Min) {
				return val;
			}
			else {
				return Min;
			}
		}

		template<typename T>
		static FORCEINLINE T Remap(const T& val, const T& fromLow, const T& fromHigh, const T& toLow, const T& toHigh)
		{
			return toLow + (val - fromLow) * (toHigh - toLow) / (fromHigh - fromLow);
		}

		template<typename T>
		static FORCEINLINE T Saturate(const T& val)
		{
			return Clamp(val, (T)(0), (T)(1));
		}

		template<typename T>
		static FORCEINLINE T Square(const T& val)
		{
			return val * val;
		}

		template<typename T>
		static FORCEINLINE T Cube(const T& val)
		{
			return val * val*val;
		}

		template<typename T>
		static FORCEINLINE bool Equals(const T& val1, const T& val2, const T& errorMargin)
		{
			return Abs(val1 - val2) < errorMargin;
		}

		// TODO: Min and max of array
	};
}


#endif