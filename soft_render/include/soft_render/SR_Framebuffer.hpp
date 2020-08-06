
#ifndef SR_FRAMEBUFFER_HPP
#define SR_FRAMEBUFFER_HPP

#include "lightsky/setup/Arch.h"

#include "lightsky/math/half.h"

#include "lightsky/utils/Assertions.h"
#include "lightsky/utils/Copy.h" // utils::fast_memset, fast_fill

#include "soft_render/SR_Texture.hpp"
#include "SR_Shader.hpp"




/*-----------------------------------------------------------------------------
 * Forward Declarations
-----------------------------------------------------------------------------*/
namespace ls
{
namespace math
{
template <typename color_type>
union vec4_t;
}
}



/*-----------------------------------------------------------------------------
 * Framebuffer Abstraction
-----------------------------------------------------------------------------*/
class SR_Framebuffer
{
  public:
    typedef void (*PixelPlacementFuncType)(uint16_t, uint16_t, const ls::math::vec4&, SR_Texture* const);
    typedef void (*BlendedPixelPlacementFuncType)(uint16_t, uint16_t, const ls::math::vec4&, SR_Texture* const, const SR_BlendMode);

  private:
    uint64_t mNumColors;

    SR_Texture** mColors;

    SR_Texture* mDepth;

  public:
    ~SR_Framebuffer() noexcept;

    SR_Framebuffer() noexcept;

    SR_Framebuffer(const SR_Framebuffer& f) noexcept;

    SR_Framebuffer(SR_Framebuffer&& f) noexcept;

    SR_Framebuffer& operator=(const SR_Framebuffer& f) noexcept;

    SR_Framebuffer& operator=(SR_Framebuffer&& f) noexcept;

    int reserve_color_buffers(uint64_t numColorBuffers) noexcept;

    int attach_color_buffer(uint64_t index, SR_Texture& t) noexcept;

    SR_Texture* detach_color_buffer(uint64_t index) noexcept;

    const SR_Texture* get_color_buffer(uint64_t index) const noexcept;

    SR_Texture* get_color_buffer(uint64_t index) noexcept;

    uint64_t num_color_buffers() const noexcept;

    template <typename index_type, typename color_type>
    void clear_color_buffer(const index_type index, const color_type& color) noexcept;

    void clear_color_buffers() noexcept;

    int attach_depth_buffer(SR_Texture& d) noexcept;

    SR_Texture* detach_depth_buffer() noexcept;

    const SR_Texture* get_depth_buffer() const noexcept;

    SR_Texture* get_depth_buffer() noexcept;

    template <typename float_type>
    void clear_depth_buffer(const float_type depthVal) noexcept;

    void clear_depth_buffer() noexcept;

    int valid() const noexcept;

    void terminate() noexcept;

    void put_pixel(
        uint64_t targetId,
        uint16_t x,
        uint16_t y,
        const ls::math::vec4_t<float>& rgba) noexcept;

    void put_alpha_pixel(
        uint64_t targetId,
        uint16_t x,
        uint16_t y,
        const ls::math::vec4_t<float>& rgba,
        const SR_BlendMode blendMode) noexcept;

    template <typename data_t>
    void put_depth_pixel(
        uint16_t x,
        uint16_t y,
        data_t depth) noexcept;

    uint16_t width() const noexcept;

    uint16_t height() const noexcept;

    uint16_t depth() const noexcept;

    PixelPlacementFuncType pixel_placement_function(SR_ColorDataType type) const noexcept;

    BlendedPixelPlacementFuncType blended_pixel_placement_function(SR_ColorDataType type) const noexcept;
};



/*-------------------------------------
 * Retrieve an internal color buffer, or NULL if it doesn't exist.
-------------------------------------*/
inline const SR_Texture* SR_Framebuffer::get_color_buffer(uint64_t index) const noexcept
{
    return mColors[index];
}



/*-------------------------------------
 * Retrieve an internal color buffer, or NULL if it doesn't exist.
-------------------------------------*/
inline SR_Texture* SR_Framebuffer::get_color_buffer(uint64_t index) noexcept
{
    return mColors[index];
}



/*-------------------------------------
 * Retrieve the number of active color buffers.
-------------------------------------*/
inline uint64_t SR_Framebuffer::num_color_buffers() const noexcept
{
    return mNumColors;
}



/*-------------------------------------
 * Clear a color buffer
-------------------------------------*/
template <typename index_type, typename color_type>
void SR_Framebuffer::clear_color_buffer(const index_type i, const color_type& c) noexcept
{
    static_assert(std::is_integral<index_type>::value, "Error: Data type cannot be used for indexing.");

    SR_Texture* const pTex = mColors[i];

    if (pTex->data())
    {
        LS_DEBUG_ASSERT(pTex->bpp() == sizeof(color_type)); // insurance
        const size_t numItems = pTex->width() * pTex->height() * pTex->depth();

        if (sizeof(color_type) == sizeof(uint32_t))
        {
            const size_t numBytes = numItems * pTex->bpp();
            ls::utils::fast_memset_4(reinterpret_cast<void*>(pTex->data()), *reinterpret_cast<const uint32_t*>(&c), numBytes);
        }
        else
        {
            ls::utils::fast_fill<color_type>(reinterpret_cast<color_type*>(pTex->data()), c, numItems);
        }
    }
}



/*-------------------------------------
 * Clear the depth buffer
-------------------------------------*/
template <typename float_type>
void SR_Framebuffer::clear_depth_buffer(const float_type depthVal) noexcept
{
    static_assert(std::is_floating_point<float_type>::value, "Error: Data type cannot be used for clearing a depth buffer.");

    if (!mDepth->data())
    {
        return;
    }

    LS_DEBUG_ASSERT(mDepth->bpp() == sizeof(float_type)); // insurance

    if (sizeof(float_type) == sizeof(uint32_t))
    {
        const size_t numBytes = mDepth->width()*mDepth->height()* sizeof(float_type);
        union
        {
            float_type f;
            uint32_t i;
        } outVal{depthVal};
        ls::utils::fast_memset_4(reinterpret_cast<void*>(mDepth->data()), outVal.i, numBytes);
    }
    else if (sizeof(float_type) == sizeof(uint64_t))
    {
        const size_t numBytes = mDepth->width()*mDepth->height()* sizeof(float_type);
        union
        {
            float_type f;
            uint64_t i;
        } outVal{depthVal};
        ls::utils::fast_memset_8(reinterpret_cast<void*>(mDepth->data()), outVal.i, numBytes);
    }
    else
    {
        ls::utils::fast_fill<float_type>(reinterpret_cast<float_type*>(mDepth->data()), depthVal, mDepth->width()*mDepth->height());
    }
}



/*-------------------------------------
 * Clear the depth buffer
-------------------------------------*/
inline void SR_Framebuffer::clear_depth_buffer() noexcept
{
    if (mDepth->data())
    {
        const uint64_t numBytes = mDepth->bpp() * mDepth->width() * mDepth->height() * mDepth->depth();
        ls::utils::fast_memset(mDepth->data(), 0, numBytes);
    }
}



/*-------------------------------------
 * Retrieve the depth buffer
-------------------------------------*/
inline const SR_Texture* SR_Framebuffer::get_depth_buffer() const noexcept
{
    return mDepth;
}



/*-------------------------------------
 * Retrieve the depth buffer
-------------------------------------*/
inline SR_Texture* SR_Framebuffer::get_depth_buffer() noexcept
{
    return mDepth;
}



/*-------------------------------------
 * Place a single pixel onto the depth buffer
-------------------------------------*/
template <>
inline void SR_Framebuffer::put_depth_pixel<ls::math::half>(uint16_t x, uint16_t y, ls::math::half depth) noexcept
{
    mDepth->texel<ls::math::half>(x, y) = depth;
}



template <>
inline void SR_Framebuffer::put_depth_pixel<float>(uint16_t x, uint16_t y, float depth) noexcept
{
    mDepth->texel<float>(x, y) = depth;
}



template <>
inline void SR_Framebuffer::put_depth_pixel<double>(uint16_t x, uint16_t y, double depth) noexcept
{
    mDepth->texel<double>(x, y) = depth;
}



#endif /* SR_FRAMEBUFFER_HPP */
