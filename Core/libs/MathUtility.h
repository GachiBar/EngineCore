#pragma once
#include <cmath>

struct FMath
{
	/** Computes absolute value in a generic way */
	template <class T>
	static constexpr inline T Abs(const T A)
	{
		return (A >= (T)0) ? A : -A;
	}

	/** Returns 1, 0, or -1 depending on relation of T to 0 */
	template <class T>
	static constexpr inline T Sign(const T A)
	{
		return (A > (T)0) ? (T)1 : ((A < (T)0) ? (T)-1 : (T)0);
	}

	/** Returns higher value in a generic way */
	template <class T>
	static constexpr inline T Max(const T A, const T B)
	{
		return (A >= B) ? A : B;
	}

	/** Returns lower value in a generic way */
	template <class T>
	static constexpr inline T Min(const T A, const T B)
	{
		return (A <= B) ? A : B;
	}

	// Allow mixing of float types to promote to highest precision type
	static constexpr inline double Max(const double A, const float B) { return Max<double>(A, B); }
	static constexpr inline double Max(const float A, const double B) { return Max<double>(A, B); }
	static constexpr inline double Min(const double A, const float B) { return Min<double>(A, B); }
	static constexpr inline double Min(const float A, const double B) { return Min<double>(A, B); }


	static inline bool IsNearlyEqual(float A, float B, float ErrorTolerance = std::numeric_limits<float>::epsilon());

	static inline bool IsNearlyEqual(double A, double B, double ErrorTolerance = std::numeric_limits<double>::epsilon());

	static inline bool IsNearlyZero(float Value, float ErrorTolerance = std::numeric_limits<float>::epsilon());

	static inline bool IsNearlyZero(double Value, double ErrorTolerance = std::numeric_limits<double>::epsilon());
};


/** Float specialization */
template <>
inline float FMath::Abs(const float A)
{
	return fabsf(A);
}

template <>
inline double FMath::Abs(const double A)
{
	return fabs(A);
}

inline bool FMath::IsNearlyEqual(float A, float B, float ErrorTolerance)
{
	return Abs<float>(A - B) <= ErrorTolerance;
}

inline bool FMath::IsNearlyEqual(double A, double B, double ErrorTolerance)
{
	return Abs<double>(A - B) <= ErrorTolerance;
}

inline bool FMath::IsNearlyZero(float Value, float ErrorTolerance)
{
	return Abs<float>(Value) <= ErrorTolerance;
}

inline bool FMath::IsNearlyZero(double Value, double ErrorTolerance)
{
	return Abs<double>(Value) <= ErrorTolerance;
}
