#pragma once

#include <xmmintrin.h>
#include <smmintrin.h>

#include "core/types.h"
#include "../sse_masks.h"
#include "../common.h"

union Vector4
{
    struct { f32 x, y, z, w; };
    struct { f32 r, b, g, a; };
    struct { f32 s, t, u, v; };
    f32 data[4];
    __m128 _sse;

    Vector4(f32 val = 0.0f)
    :   _sse(_mm_set1_ps(val))
    {
    }

    Vector4(f32 x, f32 y, f32 z, f32 w)
    :   _sse(_mm_setr_ps(x, y, z, w))
    {
    }

    Vector4(__m128 sse)
    :   _sse(sse)
    {
    }

    // Vector Functions
    inline f32 Length() const
    {
        return _mm_cvtss_f32(_mm_sqrt_ps(_mm_dp_ps(_sse, _sse, SSE::DP_MASK_V4)));
    }

    inline f32 SqrLength() const
    {
        return _mm_cvtss_f32(_mm_dp_ps(_sse, _sse, SSE::DP_MASK_V4));
    }

    // Returns normalized vector without changing the original
    inline Vector4 Unit() const
    {
        if (_mm_movemask_ps(_mm_cmpeq_ps(_sse, _mm_setzero_ps())) != 0xF)
            return _mm_div_ps(_sse, _mm_sqrt_ps(_mm_dp_ps(_sse, _sse, 0xFF)));
        
        return *this;
    }

    // Changes the original vector
    inline Vector4& Normalize()
    {
        if (_mm_movemask_ps(_mm_cmpeq_ps(_sse, _mm_setzero_ps())) != 0xF)
            _sse = _mm_div_ps(_sse, _mm_sqrt_ps(_mm_dp_ps(_sse, _sse, 0xFF)));

        return *this;
    }

    // Comparative Operators
    inline bool operator==(const Vector4& rhs) const
    {
        return (_mm_movemask_ps(_mm_cmpeq_ps(_sse, rhs._sse)) == 0xF);
    }

    inline bool operator!=(const Vector4& rhs) const
    {
        return (_mm_movemask_ps(_mm_cmpeq_ps(_sse, rhs._sse)) != 0xF);
    }

    // Unary Operator(s?)
    inline Vector4 operator-() const
    {
        return _mm_xor_ps(_sse, SSE::SIGN_MASK_V4);
    }

    // Arithmetic Operators
    inline Vector4 operator+(const Vector4& rhs) const
    {
        return _mm_add_ps(_sse, rhs._sse);
    }

    inline Vector4 operator-(const Vector4& rhs) const
    {
        return _mm_sub_ps(_sse, rhs._sse);
    }

    inline Vector4 operator*(const Vector4& rhs) const
    {
        return _mm_mul_ps(_sse, rhs._sse);
    }

    inline Vector4 operator/(const Vector4& rhs) const
    {
        return _mm_div_ps(_sse, rhs._sse);
    }

    // op= Operators
    inline Vector4& operator+=(const Vector4& rhs)
    {
        _sse = _mm_add_ps(_sse, rhs._sse);
        return *this;
    }

    inline Vector4& operator-=(const Vector4& rhs)
    {
        _sse = _mm_sub_ps(_sse, rhs._sse);
        return *this;
    }

    inline Vector4& operator*=(const Vector4& rhs)
    {
        _sse = _mm_mul_ps(_sse, rhs._sse);
        return *this;
    }

    inline Vector4& operator/=(const Vector4& rhs)
    {
        _sse = _mm_mul_ps(_sse, rhs._sse);
        return *this;
    }

    // Arithmetic Operators with a Scalar
    inline Vector4 operator*(f32 scalar) const
    {
        return _mm_mul_ps(_sse, _mm_set1_ps(scalar));
    }

    inline Vector4 operator/(f32 scalar) const
    {
        return _mm_div_ps(_sse, _mm_set1_ps(scalar));
    }

    // op= Operators with a Scalar
    inline Vector4& operator*=(f32 scalar)
    {
        _sse = _mm_mul_ps(_sse, _mm_set1_ps(scalar));
        return *this;
    }

    inline Vector4& operator/=(f32 scalar)
    {
        _sse = _mm_div_ps(_sse, _mm_set1_ps(scalar));
        return *this;
    }

    // Conversion Operator
    inline operator __m128() const
    {
        return _sse;
    }

    inline operator __m128()
    {
        return _sse;
    }

    // Indexing Operator
    inline const f32& operator[](int index) const
    {
        return data[Min(index, 3)];
    }

    inline f32& operator[](int index)
    {
        return data[Min(index, 3)];
    }
};

// Arithmetic Operators with Scalar on the left
inline Vector4 operator*(f32 scalar, const Vector4& vec)
{
    return vec * scalar;
}

inline Vector4 operator/(f32 scalar, const Vector4& vec)
{
    return _mm_div_ps(_mm_set1_ps(scalar), vec._sse);
}

// Additional Functions
inline f32 Dot(const Vector4& lhs, const Vector4& rhs)
{
    return _mm_cvtss_f32(_mm_dp_ps(lhs._sse, rhs._sse, SSE::DP_MASK_V4));
}

inline Vector4 Lerp(const Vector4& a, const Vector4& b, f32 t)
{
    return _mm_add_ps(
        _mm_mul_ps(a._sse, _mm_set1_ps(1.0f - t)),
        _mm_mul_ps(b._sse, _mm_set1_ps(t))
    );
}