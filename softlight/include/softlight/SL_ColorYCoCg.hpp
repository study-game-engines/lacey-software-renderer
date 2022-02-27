
#ifndef SL_COLOR_YCOCG_HPP
#define SL_COLOR_YCOCG_HPP

#include "softlight/SL_Color.hpp"



/*-----------------------------------------------------------------------------
 * YCoCg Types
-----------------------------------------------------------------------------*/
/**
 * @brief Generic YcoCg Color Structure
 */
template<typename color_t>
struct alignas(sizeof(color_t)) SL_ColorTypeYCoCg
{
    color_t y;
    color_t co;
    color_t cg;
};

// Specializations
typedef SL_ColorTypeYCoCg<int8_t>   SL_ColorYCoCg8;
typedef SL_ColorTypeYCoCg<uint8_t>  SL_ColorYCoCg8u;
typedef SL_ColorTypeYCoCg<int16_t>  SL_ColorYCoCg16;
typedef SL_ColorTypeYCoCg<uint16_t> SL_ColorYCoCg16u;
typedef SL_ColorTypeYCoCg<int32_t>  SL_ColorYCoCg32;
typedef SL_ColorTypeYCoCg<uint32_t> SL_ColorYCoCg32u;
typedef SL_ColorTypeYCoCg<int64_t>  SL_ColorYCoCg64;
typedef SL_ColorTypeYCoCg<uint64_t> SL_ColorYCoCg64u;
typedef SL_ColorTypeYCoCg<float>    SL_ColorYCoCgf;
typedef SL_ColorTypeYCoCg<double>   SL_ColorYCoCgd;

typedef SL_ColorYCoCgf SL_ColorYCoCg;



/**
 * @brief YcoCg Color Structure with Alpha
 */
template<typename color_t>
struct alignas(sizeof(color_t)*4) SL_ColorTypeYCoCgA
{
    color_t y;
    color_t co;
    color_t cg;
    color_t a;
};

// Specializations
typedef SL_ColorTypeYCoCgA<int8_t>   SL_ColorYCoCgA8;
typedef SL_ColorTypeYCoCgA<uint8_t>  SL_ColorYCoCgA8u;
typedef SL_ColorTypeYCoCgA<int16_t>  SL_ColorYCoCgA16;
typedef SL_ColorTypeYCoCgA<uint16_t> SL_ColorYCoCgA16u;
typedef SL_ColorTypeYCoCgA<int32_t>  SL_ColorYCoCgA32;
typedef SL_ColorTypeYCoCgA<uint32_t> SL_ColorYCoCgA32u;
typedef SL_ColorTypeYCoCgA<int64_t>  SL_ColorYCoCgA64;
typedef SL_ColorTypeYCoCgA<uint64_t> SL_ColorYCoCgA64u;
typedef SL_ColorTypeYCoCgA<float>    SL_ColorYCoCgAf;
typedef SL_ColorTypeYCoCgA<double>   SL_ColorYCoCgAd;

typedef SL_ColorYCoCgAf SL_ColorYCoCgA;



/*-----------------------------------------------------------------------------
 * YCoCg & RGB Casting
-----------------------------------------------------------------------------*/
/*-------------------------------------
 * RGB to YCoCg
-------------------------------------*/
template <typename T>
constexpr SL_ColorTypeYCoCg<T> ycocg_cast(const SL_ColorRGBType<T>& p) noexcept
{
    return SL_ColorTypeYCoCg<T>{
        (T)((p[0]/T{4}) + (p[1]/T{2}) + (p[2]/T{4})),
        (T)((p[0]/T{2})               + (p[2]/T{2})),
        (T)((p[1]/T{2}) - (p[0]/T{4}) - (p[2]/T{4}))
    };
}



/*-------------------------------------
 * YCoCg to RGB
-------------------------------------*/
template <typename T>
constexpr SL_ColorRGBType<T> rgb_cast(const SL_ColorTypeYCoCg<T>& p) noexcept
{
    return SL_ColorRGBType<T>{
        (T)(p.y + p.co - p.cg),
        (T)(p.y        + p.cg),
        (T)(p.y - p.co - p.cg),
    };
}



/*-----------------------------------------------------------------------------
 * YCoCg & RGB Casting
-----------------------------------------------------------------------------*/
/*-------------------------------------
 * RGBA to YCoCg
-------------------------------------*/
template <typename T>
constexpr SL_ColorTypeYCoCgA<T> ycocg_cast(const SL_ColorRGBAType<T>& p) noexcept
{
    return SL_ColorTypeYCoCgA<T>{
        (T)((p[0]/T{4}) + (p[1]/T{2}) + (p[2]/T{4})),
        (T)((p[0]/T{2})               - (p[2]/T{2})),
        (T)((p[1]/T{2}) - (p[0]/T{4}) - (p[2]/T{4})),
        p[3]
    };
}

#if defined(LS_ARCH_X86)
inline SL_ColorTypeYCoCgA<float> ycocg_cast(const SL_ColorRGBAType<float>& p) noexcept
{
    const __m128 rgb = _mm_loadu_ps(&p);
    const __m128 a = _mm_set_ps(1.f,  0.25f, 0.5f,  0.25f);
    const __m128 b = _mm_set_ps(0.f, -0.5f,  0.f,   0.5f);
    const __m128 c = _mm_set_ps(0.f, -0.25f, 0.5f, -0.25f);
    const __m128 ycocg = _mm_fmadd_ps(rgb, c, _mm_fmadd_ps(rgb, b, _mm_mul_ps(rgb, a)));

    SL_ColorTypeYCoCgA<float> ret;
    _mm_store_ps(&ret.y, ycocg);
    return ret;
}

#elif defined(LS_ARM_NEON)
inline SL_ColorTypeYCoCgA<float> ycocg_cast(const SL_ColorRGBAType<float>& p) noexcept
{
    constexpr float32x4_t a{ 0.25f, 0.5f,  0.25f, 1.f};
    constexpr float32x4_t b{ 0.5f,  0.f,  -0.5f,  0.f};
    constexpr float32x4_t c{-0.25f, 0.5f, -0.25f, 0.f};

    const float32x4_t ycocg = vmlaq_f32(vmlaq_f32(vmulq_f32(p.simd, a), p.simd, b), p.simd, c);

    SL_ColorTypeYCoCgA<float> ret;
    vst1q_f32(&ret.y, ycocg);
    return ret;
}

#endif



/*-------------------------------------
 * YCoCgA to RGBA
-------------------------------------*/
template <typename T>
constexpr SL_ColorRGBAType<T> rgb_cast(const SL_ColorTypeYCoCgA<T>& p) noexcept
{
    return SL_ColorRGBAType<T>{
        (T)(p.y + p.co - p.cg),
        (T)(p.y        + p.cg),
        (T)(p.y - p.co - p.cg),
        p.a
    };
}



#endif /* SL_COLOR_YCOCG_HPP */

