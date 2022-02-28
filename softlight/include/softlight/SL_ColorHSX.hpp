
#ifndef SL_COLOR_HSX_HPP
#define SL_COLOR_HSX_HPP

#include "lightsky/math/scalar_utils.h"
#include "lightsky/math/vec_utils.h"

#include "softlight/SL_Color.hpp"



/*-----------------------------------------------------------------------------
 * Extended Color Models
-----------------------------------------------------------------------------*/
/**
 * @brief Generic HSV Color Structure
 */
template <typename color_t>
struct alignas(sizeof(color_t)) SL_ColorTypeHSV
{
    static_assert(ls::setup::IsFloat<color_t>::value, "HSV can only be represented by floating-point numbers.");

    typedef color_t value_type;
    static constexpr unsigned num_components() noexcept { return 3; }

    color_t h; // should be between 0-360
    color_t s; // 0-1
    color_t v; // 0-1
};

typedef SL_ColorTypeHSV<ls::math::half> SL_ColorTypeHSVh;
typedef SL_ColorTypeHSV<float> SL_ColorTypeHSVf;
typedef SL_ColorTypeHSV<double> SL_ColorTypeHSVd;



/**
 * @brief Generic HSVA Color Structure
 */
template <typename color_t>
struct alignas(sizeof(color_t)) SL_ColorTypeHSVA
{
    static_assert(ls::setup::IsFloat<color_t>::value, "HSV can only be represented by floating-point numbers.");

    typedef color_t value_type;
    static constexpr unsigned num_components() noexcept { return 4; }

    color_t h; // should be between 0-360
    color_t s; // 0-1
    color_t v; // 0-1
    color_t a; // 0-1
};

typedef SL_ColorTypeHSVA<ls::math::half> SL_ColorTypeHSVAh;
typedef SL_ColorTypeHSVA<float> SL_ColorTypeHSVAf;
typedef SL_ColorTypeHSVA<double> SL_ColorTypeHSVAd;



/**
 * @brief Generic HSL Color Structure
 */
template <typename color_t>
struct alignas(sizeof(color_t)) SL_ColorTypeHSL
{
    static_assert(ls::setup::IsFloat<color_t>::value, "HSL can only be represented by floating-point numbers.");

    typedef color_t value_type;
    static constexpr unsigned num_components() noexcept { return 3; }

    color_t h; // should be between 0-360
    color_t s; // 0-1
    color_t l; // 0-1
};

typedef SL_ColorTypeHSL<ls::math::half> SL_ColorTypeHSLh;
typedef SL_ColorTypeHSL<float> SL_ColorTypeHSLf;
typedef SL_ColorTypeHSL<double> SL_ColorTypeHSLd;



/**
 * @brief Generic HSLA Color Structure
 */
template <typename color_t>
struct alignas(sizeof(color_t)) SL_ColorTypeHSLA
{
    static_assert(ls::setup::IsFloat<color_t>::value, "HSL can only be represented by floating-point numbers.");

    typedef color_t value_type;
    static constexpr unsigned num_components() noexcept { return 4; }

    color_t h; // should be between 0-360
    color_t s; // 0-1
    color_t l; // 0-1
    color_t a; // 0-1
};

typedef SL_ColorTypeHSLA<ls::math::half> SL_ColorTypeHSLAh;
typedef SL_ColorTypeHSLA<float> SL_ColorTypeHSLAf;
typedef SL_ColorTypeHSLA<double> SL_ColorTypeHSLAd;



/*-----------------------------------------------------------------------------
 * Internal limits of color HSV ranges
-----------------------------------------------------------------------------*/
/**
 * @brief Template specialization which allows for internal color calculations
 * to determine the maximum and minimum possible number ranges for certain
 * data types and their color representations.
 *
 * @tparam color_t
 * A basic C/C++ data type such as unsigned char, int, double, etc...
 */
template <typename color_t>
struct SL_ColorLimits<color_t, SL_ColorTypeHSV>
{
    /**
    * @brief Determine the minimum possible value for a color object's internal
    * data types.
    */
    static constexpr SL_ColorTypeHSV<color_t> min() noexcept
    {
        return SL_ColorTypeHSV<color_t>{
            (color_t)0.0, (color_t)0.0, (color_t)0.0
        };
    }

    /**
    * @brief Determine the maximum possible value for a color object's internal
    * data types.
    */
    static constexpr SL_ColorTypeHSV<color_t> max() noexcept
    {
        return SL_ColorTypeHSV<color_t>{
            (color_t)360.0, (color_t)1.0, (color_t)1.0
        };
    }
};



template <>
struct SL_ColorLimits<ls::math::half, SL_ColorTypeHSV>
{
    /**
    * @brief Determine the minimum possible value for a color object's internal
    * data types.
    */
    static constexpr SL_ColorTypeHSV<ls::math::half> min() noexcept
    {
        return SL_ColorTypeHSV<ls::math::half>{
            ls::math::half{0x00u, 0x00u},
            ls::math::half{0x00u, 0x00u},
            ls::math::half{0x00u, 0x00u}
        };
    }

    /**
    * @brief Determine the maximum possible value for a color object's internal
    * data types.
    */
    static constexpr SL_ColorTypeHSV<ls::math::half> max() noexcept
    {
        return SL_ColorTypeHSV<ls::math::half>{
            ls::math::half{0x5Du, 0xA0u},
            ls::math::half{0x3Cu, 0x00u},
            ls::math::half{0x3Cu, 0x00u}
        };
    }
};



/*-----------------------------------------------------------------------------
 * Internal limits of color HSVA ranges
-----------------------------------------------------------------------------*/
/**
 * @brief Template specialization which allows for internal color calculations
 * to determine the maximum and minimum possible number ranges for certain
 * data types and their color representations.
 *
 * @tparam color_t
 * A basic C/C++ data type such as unsigned char, int, double, etc...
 */
template <typename color_t>
struct SL_ColorLimits<color_t, SL_ColorTypeHSVA>
{
    /**
    * @brief Determine the minimum possible value for a color object's internal
    * data types.
    */
    static constexpr SL_ColorTypeHSVA<color_t> min() noexcept
    {
        return SL_ColorTypeHSVA<color_t>{
            (color_t)0.0,
            (color_t)0.0,
            (color_t)0.0,
            (color_t)0.0
        };
    }

    /**
    * @brief Determine the maximum possible value for a color object's internal
    * data types.
    */
    static constexpr SL_ColorTypeHSVA<color_t> max() noexcept
    {
        return SL_ColorTypeHSVA<color_t>{
            (color_t)360.0,
            (color_t)1.0,
            (color_t)1.0,
            (color_t)1.0
        };
    }
};



template <>
struct SL_ColorLimits<ls::math::half, SL_ColorTypeHSVA>
{
    /**
    * @brief Determine the minimum possible value for a color object's internal
    * data types.
    */
    static constexpr SL_ColorTypeHSVA<ls::math::half> min() noexcept
    {
        return SL_ColorTypeHSVA<ls::math::half>{
            ls::math::half{0x00u, 0x00u},
            ls::math::half{0x00u, 0x00u},
            ls::math::half{0x00u, 0x00u},
            ls::math::half{0x00u, 0x00u}
        };
    }

    /**
    * @brief Determine the maximum possible value for a color object's internal
    * data types.
    */
    static constexpr SL_ColorTypeHSVA<ls::math::half> max() noexcept
    {
        return SL_ColorTypeHSVA<ls::math::half>{
            ls::math::half{0x5Du, 0xA0u},
            ls::math::half{0x3Cu, 0x00u},
            ls::math::half{0x3Cu, 0x00u},
            ls::math::half{0x3Cu, 0x00u}
        };
    }
};



/*-----------------------------------------------------------------------------
 * Internal limits of color HSV ranges
-----------------------------------------------------------------------------*/
/**
 * @brief Template specialization which allows for internal color calculations
 * to determine the maximum and minimum possible number ranges for certain
 * data types and their color representations.
 *
 * @tparam color_t
 * A basic C/C++ data type such as unsigned char, int, double, etc...
 */
template <typename color_t>
struct SL_ColorLimits<color_t, SL_ColorTypeHSL>
{
    /**
    * @brief Determine the minimum possible value for a color object's internal
    * data types.
    *
    * @return For integral types, the return value is equivalent to
    * std::numeric_limits<color_t>::min(). Floating-point types will return 0.0.
    */
    static constexpr SL_ColorTypeHSL<color_t> min() noexcept
    {
        return SL_ColorTypeHSL<color_t>{
            (color_t)0.0, (color_t)0.0, (color_t)0.0
        };
    }

    /**
    * @brief Determine the maximum possible value for a color object's internal
    * data types.
    *
    * @return For integral types, the return value is equivalent to
    * std::numeric_limits<color_t>::max(). Floating-point types will return 10.0.
    */
    static constexpr SL_ColorTypeHSL<color_t> max() noexcept
    {
        return SL_ColorTypeHSL<color_t>{
            (color_t)360.0, (color_t)1.0, (color_t)1.0
        };
    }
};



template <>
struct SL_ColorLimits<ls::math::half, SL_ColorTypeHSL>
{
    /**
    * @brief Determine the minimum possible value for a color object's internal
    * data types.
    *
    * @return For integral types, the return value is equivalent to
    * std::numeric_limits<color_t>::min(). Floating-point types will return 0.0.
    */
    static constexpr SL_ColorTypeHSL<ls::math::half> min() noexcept
    {
        return SL_ColorTypeHSL<ls::math::half>{
            ls::math::half{0x00u, 0x00u},
            ls::math::half{0x00u, 0x00u},
            ls::math::half{0x00u, 0x00u}
        };
    }

    /**
    * @brief Determine the maximum possible value for a color object's internal
    * data types.
    *
    * @return For integral types, the return value is equivalent to
    * std::numeric_limits<color_t>::max(). Floating-point types will return 10.0.
    */
    static constexpr SL_ColorTypeHSL<ls::math::half> max() noexcept
    {
        return SL_ColorTypeHSL<ls::math::half>{
            ls::math::half{0x5Du, 0xA0u},
            ls::math::half{0x3Cu, 0x00u},
            ls::math::half{0x3Cu, 0x00u}
        };
    }
};



/*-----------------------------------------------------------------------------
 * Internal limits of color HSVA ranges
-----------------------------------------------------------------------------*/
/**
 * @brief Template specialization which allows for internal color calculations
 * to determine the maximum and minimum possible number ranges for certain
 * data types and their color representations.
 *
 * @tparam color_t
 * A basic C/C++ data type such as unsigned char, int, double, etc...
 */
template <typename color_t>
struct SL_ColorLimits<color_t, SL_ColorTypeHSLA>
{
    /**
    * @brief Determine the minimum possible value for a color object's internal
    * data types.
    *
    * @return For integral types, the return value is equivalent to
    * std::numeric_limits<color_t>::min(). Floating-point types will return 0.0.
    */
    static constexpr SL_ColorTypeHSLA<color_t> min() noexcept
    {
        return SL_ColorTypeHSLA<color_t>{
            (color_t)0.0,
            (color_t)0.0,
            (color_t)0.0,
            (color_t)0.0
        };
    }

    /**
    * @brief Determine the maximum possible value for a color object's internal
    * data types.
    *
    * @return For integral types, the return value is equivalent to
    * std::numeric_limits<color_t>::max(). Floating-point types will return 10.0.
    */
    static constexpr SL_ColorTypeHSLA<color_t> max() noexcept
    {
        return SL_ColorTypeHSLA<color_t>{
            (color_t)360.0,
            (color_t)1.0,
            (color_t)1.0,
            (color_t)1.0
        };
    }
};



template <>
struct SL_ColorLimits<ls::math::half, SL_ColorTypeHSLA>
{
    /**
    * @brief Determine the minimum possible value for a color object's internal
    * data types.
    *
    * @return For integral types, the return value is equivalent to
    * std::numeric_limits<color_t>::min(). Floating-point types will return 0.0.
    */
    static constexpr SL_ColorTypeHSLA<ls::math::half> min() noexcept
    {
        return SL_ColorTypeHSLA<ls::math::half>{
            ls::math::half{0x00u, 0x00u},
            ls::math::half{0x00u, 0x00u},
            ls::math::half{0x00u, 0x00u},
            ls::math::half{0x00u, 0x00u}
        };
    }

    /**
    * @brief Determine the maximum possible value for a color object's internal
    * data types.
    *
    * @return For integral types, the return value is equivalent to
    * std::numeric_limits<color_t>::max(). Floating-point types will return 10.0.
    */
    static constexpr SL_ColorTypeHSLA<ls::math::half> max() noexcept
    {
        return SL_ColorTypeHSLA<ls::math::half>{
            ls::math::half{0x5Du, 0xA0u},
            ls::math::half{0x3Cu, 0x00u},
            ls::math::half{0x3Cu, 0x00u},
            ls::math::half{0x3Cu, 0x00u}
        };
    }
};



/*-----------------------------------------------------------------------------
 * Color Casting Operations
-----------------------------------------------------------------------------*/
/*--------------------------------------
 * Cast from HSV to RGB
--------------------------------------*/
template <typename color_t>
SL_ColorRGBType<color_t> rgb_cast(const typename ls::setup::EnableIf<ls::setup::IsFloat<color_t>::value, SL_ColorTypeHSV<color_t>>::type& inC) noexcept
{
    const color_t c = inC.v * inC.s;
    const color_t x = c * (color_t{1.f} - ls::math::abs(ls::math::fmod(inC.h / color_t{60.f}, color_t{2.f}) - color_t{1.f}));
    const color_t m = inC.v - c;

    color_t tempR;
    color_t tempG;
    color_t tempB;

    if (inC.h <= color_t{60.f})
    {
        tempR = c;
        tempG = x;
        tempB = color_t{0.f};
    }
    else if (inC.h <= color_t{120.f})
    {
        tempR = x;
        tempG = c;
        tempB = color_t{0.f};
    }
    else if (inC.h <= color_t{180.f})
    {
        tempR = color_t{0.f};
        tempG = c;
        tempB = x;
    }
    else if (inC.h <= color_t{240.f})
    {
        tempR = color_t{0.f};
        tempG = x;
        tempB = c;
    }
    else if (inC.h <= color_t{300.f})
    {
        tempR = x;
        tempG = color_t{0.f};
        tempB = c;
    }
    else
    {
        tempR = c;
        tempG = color_t{0.f};
        tempB = x;
    }

    tempR += m;
    tempG += m;
    tempB += m;

    constexpr color_t COLOR_MAX_VAL = SL_ColorLimits<color_t, SL_ColorRType>::max().r;

    return SL_ColorRGBType <color_t> {
        static_cast<color_t>(tempR * COLOR_MAX_VAL),
        static_cast<color_t>(tempG * COLOR_MAX_VAL),
        static_cast<color_t>(tempB * COLOR_MAX_VAL)
    };
}



/*--------------------------------------
 * Cast from HSVA to RGBA
--------------------------------------*/
template <typename color_t>
inline SL_ColorRGBAType<color_t> rgb_cast(const typename ls::setup::EnableIf<ls::setup::IsFloat<color_t>::value, SL_ColorTypeHSVA<color_t>>::type& inC) noexcept
{
    SL_ColorTypeHSV<color_t> hsv{inC.h, inC.s, inC.l};
    const SL_ColorRGBType<color_t>&& outRGB = rgb_cast<color_t>(hsv);
    return ls::math::vec4_cast<color_t>(outRGB, inC.a);
}



/*--------------------------------------
 * Cast from HSL to RGB
--------------------------------------*/
template <typename color_t>
SL_ColorRGBType<color_t> rgb_cast(const typename ls::setup::EnableIf<ls::setup::IsFloat<color_t>::value, SL_ColorTypeHSL<color_t>>::type& inC) noexcept
{
    const color_t c = inC.s * (color_t{1.f} - ls::math::abs(color_t{2.f} * inC.l - color_t{1.f}));
    const color_t x = c * (color_t{1.f} - ls::math::abs(ls::math::fmod(inC.h / color_t{60.f}, color_t{2.f}) - color_t{1.f}));
    const color_t m = inC.l - (c * color_t{0.5f});

    color_t tempR;
    color_t tempG;
    color_t tempB;

    if (inC.h <= color_t{60.f})
    {
        tempR = c;
        tempG = x;
        tempB = color_t{0.f};
    }
    else if (inC.h <= color_t{120.f})
    {
        tempR = x;
        tempG = c;
        tempB = color_t{0.f};
    }
    else if (inC.h <= color_t{180.f})
    {
        tempR = 0.f;
        tempG = c;
        tempB = x;
    }
    else if (inC.h <= color_t{240.f})
    {
        tempR = color_t{0.f};
        tempG = x;
        tempB = c;
    }
    else if (inC.h <= color_t{300.f})
    {
        tempR = x;
        tempG = color_t{0.f};
        tempB = c;
    }
    else
    {
        tempR = c;
        tempG = color_t{0.f};
        tempB = x;
    }

    tempR += m;
    tempG += m;
    tempB += m;

    constexpr color_t COLOR_MAX_VAL = SL_ColorLimits<color_t, SL_ColorRType>::max().r;

    return SL_ColorRGBType<color_t>{
        static_cast<color_t>(tempR * COLOR_MAX_VAL),
        static_cast<color_t>(tempG * COLOR_MAX_VAL),
        static_cast<color_t>(tempB * COLOR_MAX_VAL)
    };
}



/*--------------------------------------
 * Cast from HSLA to RGBA
--------------------------------------*/
template <typename color_t>
inline SL_ColorRGBAType<color_t> rgb_cast(const typename ls::setup::EnableIf<ls::setup::IsFloat<color_t>::value, SL_ColorTypeHSLA<color_t>>::type& inC) noexcept
{
    SL_ColorTypeHSL<color_t> hsl{inC.h, inC.s, inC.l};
    const SL_ColorRGBType<color_t>&& outRGB = rgb_cast<color_t>(hsl);
    return ls::math::vec4_cast<color_t>(outRGB, inC.a);
}



/*--------------------------------------
 * RGB To HSV
--------------------------------------*/
template <typename color_t>
SL_ColorTypeHSV<color_t> hsv_cast(const typename ls::setup::EnableIf<ls::setup::IsFloat<color_t>::value, SL_ColorRGBType<color_t>>::type& c) noexcept
{
    const color_t COLOR_EPSILON = color_t{1.0e-6f};
    const color_t normR = (color_t)(0.5f * (static_cast<float>(c[0]) + 1.f));
    const color_t normG = (color_t)(0.5f * (static_cast<float>(c[1]) + 1.f));
    const color_t normB = (color_t)(0.5f * (static_cast<float>(c[2]) + 1.f));

    // normalize the input values and calculate their deltas
    const color_t maxVal = ls::math::max(normR, ls::math::max(normG, normB));
    const color_t minVal = ls::math::min(normR, ls::math::min(normG, normB));
    const color_t delta = maxVal - minVal;

    // check if we are near 0 (min)
    constexpr color_t COLOR_MIN_VAL = SL_ColorLimits<color_t, SL_ColorRType>::min().r;
    if (ls::math::abs(maxVal) <= COLOR_MIN_VAL)
    {
        return SL_ColorTypeHSV<color_t>{
            static_cast<color_t>(-1.f),
            static_cast<color_t>(NAN), static_cast<color_t> (INFINITY)
        };
    }

    color_t hue = color_t{60.f};

    if (ls::math::abs(maxVal-normR) <= COLOR_EPSILON)
    {
        hue *= ls::math::fmod(normG - normB, color_t{6.f}) / delta;
    }
    else if (ls::math::abs(maxVal-normG) <= COLOR_EPSILON)
    {
        hue *= color_t{2.f} + ((normB - normR) / delta);
    }
    else
    {
        hue *= color_t{4.f} + ((normR - normG) / delta);
    }

    // This part of the conversion requires a data type with more than 2 bytes.
    // Some values may be valid, others may be truncated/undefined.
    if (hue < color_t{360.f})
    {
        hue = color_t{360.f} - ls::math::fmod(ls::math::abs(hue), color_t{360.f});
    }
    else
    {
        hue = ls::math::fmod(hue, color_t{360.f});
    }

    // result
    return SL_ColorTypeHSV<color_t>{hue, delta / maxVal, maxVal};
}



/*--------------------------------------
 * Cast from RGBA to HSVA
--------------------------------------*/
template <typename color_t>
inline SL_ColorTypeHSVA<color_t> hsv_cast(const typename ls::setup::EnableIf<ls::setup::IsFloat<color_t>::value, SL_ColorRGBAType<color_t>>::type& c) noexcept
{
    const SL_ColorTypeHSV<color_t>&& hsv = hsv_cast<color_t>(ls::math::vec3_cast<color_t>(c));
    return SL_ColorTypeHSVA<color_t>{hsv.h, hsv.s, hsv.v, c[3]};
}



/*--------------------------------------
 * HSL To HSV
--------------------------------------*/
template <typename color_t>
inline SL_ColorTypeHSV<color_t> hsv_cast(const SL_ColorTypeHSL<color_t>& c) noexcept
{
    color_t l = color_t{2} * c.l;
    color_t s = c.s * ((l <= color_t{1}) ? l : color_t{2} - l);

    return SL_ColorTypeHSV<color_t>{
        c.h,
        (color_t{2} * s) / (l + s),
        (l + s) / color_t{2}
    };
}



/*--------------------------------------
 * HSLA To HSVA
--------------------------------------*/
template <typename color_t>
inline SL_ColorTypeHSVA<color_t> hsv_cast(const SL_ColorTypeHSLA<color_t>& c) noexcept
{
    color_t l = color_t{2} * c.l;
    color_t s = c.s * ((l <= color_t{1}) ? l : color_t{2} - l);

    return SL_ColorTypeHSVA<color_t>{
        c.h,
        (color_t{2} * s) / (l + s),
        (l + s) / color_t{2},
        c.a
    };
}



/*-------------------------------------
 * RGB to HSL
-------------------------------------*/
template <typename color_t>
SL_ColorTypeHSL<color_t> hsl_cast(const typename ls::setup::EnableIf<ls::setup::IsFloat<color_t>::value, SL_ColorRGBType<color_t>>::type& c) noexcept
{
    const color_t COLOR_EPSILON = color_t{1.0e-6f};
    constexpr color_t COLOR_MIN_VAL = SL_ColorLimits<color_t, SL_ColorRType>::min().r;
    constexpr color_t COLOR_MAX_VAL = SL_ColorLimits<color_t, SL_ColorRType>::max().r;

    const color_t normR = (color_t)(0.5f * (static_cast<float>(c[0]) + 1.f));
    const color_t normG = (color_t)(0.5f * (static_cast<float>(c[1]) + 1.f));
    const color_t normB = (color_t)(0.5f * (static_cast<float>(c[2]) + 1.f));

    // normalize the input values and calculate their deltas
    const color_t maxVal = ls::math::max(normR, ls::math::max(normG, normB));
    const color_t minVal = ls::math::min(normR, ls::math::min(normG, normB));
    const color_t delta = maxVal - minVal;
    const color_t deltaInv = delta != color_t{0.f} ? ls::math::rcp(delta) : color_t{0.f};

    // check if we are near 0
    if (ls::math::abs(maxVal) <= COLOR_MIN_VAL)
    {
        return SL_ColorTypeHSL<color_t>{0.f, 0.f, 0.f};
    }

    color_t hue = color_t{60.f};

    if (ls::math::abs(maxVal - normR) <= COLOR_EPSILON)
    {
        hue *= ls::math::fmod(normG - normB, color_t{6.f}) * deltaInv;
    }
    else if (ls::math::abs(maxVal - normG) <= COLOR_EPSILON)
    {
        hue *= color_t{2.f} + ((normB - normR) * deltaInv);
    }
    else
    {
        hue *= color_t{4.f} + ((normR - normG) * deltaInv);
    }

    // This part of the conversion requires a data type with more than 2 bytes.
    // Some values may be valid, others may be truncated/undefined.
    if (hue < color_t{0.f})
    {
        hue = color_t{360.f} - ls::math::fmod(ls::math::abs(hue), color_t{360.f});
    }

    color_t lightness = color_t{0.5f} * (maxVal + minVal);
    color_t saturation = color_t{0.f};

    if (ls::math::abs(maxVal) > COLOR_MAX_VAL)
    {
        saturation = delta * ls::math::rcp(color_t{1.f} - ls::math::abs(color_t{2.f} * lightness - color_t{1.f}));
    }

    // result
    return SL_ColorTypeHSL<color_t>{hue, saturation, lightness};
}



/*--------------------------------------
 * Cast from RGBA to HSLA
--------------------------------------*/
template <typename color_t>
inline SL_ColorTypeHSLA<color_t> hsl_cast(const typename ls::setup::EnableIf<ls::setup::IsFloat<color_t>::value, SL_ColorRGBAType<color_t>>::type& c) noexcept
{
    const SL_ColorTypeHSL<color_t>&& hsl = hsl_cast<color_t>(ls::math::vec3_cast<color_t>(c));
    return SL_ColorTypeHSLA<color_t>{hsl.h, hsl.s, hsl.l, c[3]};
}



/*-------------------------------------
 * HSV to HSL
-------------------------------------*/
template <typename color_t>
inline SL_ColorTypeHSL<color_t> hsl_cast(const SL_ColorTypeHSV<color_t>& c) noexcept
{
    color_t s = c.s * c.v;
    color_t l = (color_t{2} - c.s) * c.v;

    return SL_ColorTypeHSL<color_t>{
        c.h,
        s / ((l <= color_t{1}) ? l : color_t{2} - l),
        l / color_t{2}
    };
}



/*-------------------------------------
 * HSVA to HSLA
-------------------------------------*/
template <typename color_t>
inline SL_ColorTypeHSLA<color_t> hsl_cast(const SL_ColorTypeHSVA<color_t>& c) noexcept
{
    color_t s = c.s * c.v;
    color_t l = (color_t{2} - c.s) * c.v;

    return SL_ColorTypeHSLA<color_t>{
        c.h,
        s / ((l <= color_t{1}) ? l : color_t{2} - l),
        l / color_t{2},
        c.a
    };
}



#endif /* SL_COLOR_HSX_HPP */

