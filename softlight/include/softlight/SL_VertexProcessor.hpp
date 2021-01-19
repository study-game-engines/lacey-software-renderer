
#ifndef SL_VERTEX_PROCESSOR_HPP
#define SL_VERTEX_PROCESSOR_HPP

#include <atomic>

#include "softlight/SL_Mesh.hpp"



/*-----------------------------------------------------------------------------
 * Forward Declarations
-----------------------------------------------------------------------------*/
namespace ls
{
    namespace math
    {
        template<typename>
        union vec4_t;
    } // end math namespace
} // end ls namespace

template <typename data_t>
union SL_BinCounter;

template <typename data_t>
union SL_BinCounterAtomic;

class SL_Context; // SL_Context.hpp
struct SL_FragmentBin; // SL_ShaderProcessor.hpp
struct SL_FragCoord;
class SL_Framebuffer;
class SL_Shader; // SL_Shader.hpp
struct SL_TransformedVert;



/*-----------------------------------------------------------------------------
 * Internal Enums
-----------------------------------------------------------------------------*/
enum SL_ClipStatus
{
    SL_TRIANGLE_NOT_VISIBLE       = 0x00,
    SL_TRIANGLE_PARTIALLY_VISIBLE = 0x01,
    SL_TRIANGLE_FULLY_VISIBLE     = 0x03,
};



/*-----------------------------------------------------------------------------
 * Encapsulation of vertex processing on another thread.
-----------------------------------------------------------------------------*/
struct SL_VertexProcessor
{
    // 32 bits
    uint16_t mThreadId;
    uint16_t mNumThreads;

    // 64-128 bits
    SL_BinCounterAtomic<int_fast64_t>* mFragProcessors;
    SL_BinCounterAtomic<uint_fast64_t>* mBusyProcessors;

    // 96-192 bits
    const SL_Shader*  mShader;
    const SL_Context* mContext;
    SL_Framebuffer*   mFbo;

    // 64-128 bits
    size_t mNumMeshes;
    size_t mNumInstances;

    // 32 bits
    SL_RenderMode mRenderMode;

    // 32-64 bits
    const SL_Mesh* mMeshes;

    // 64-128 bits
    SL_BinCounterAtomic<uint32_t>* mBinsUsed;
    SL_BinCounter<uint32_t>* mBinIds;
    SL_BinCounter<uint32_t>* mTempBinIds;

    // 96-192 bits
    SL_FragmentBin* mFragBins;
    SL_FragCoord* mFragQueues;

    // 448 bits (56 bytes) in 32-bit mode
    // 800 bits (100 bytes) in 64-bit mode
    // Padding not included

    void flush_bins() const noexcept;

    template <int renderMode>
    void push_bin(
        size_t primIndex,
        float fboW,
        float fboH,
        const SL_TransformedVert& a,
        const SL_TransformedVert& b,
        const SL_TransformedVert& c
    ) const noexcept;

    void clip_and_process_tris(
        size_t primIndex,
        float fboW,
        float fboH,
        const SL_TransformedVert& a,
        const SL_TransformedVert& b,
        const SL_TransformedVert& c
    ) noexcept;

    void process_points(const SL_Mesh& m, size_t instanceId) noexcept;

    void process_lines(const SL_Mesh& m, size_t instanceId) noexcept;

    void process_tris(const SL_Mesh& m, size_t instanceId) noexcept;

    void execute() noexcept;
};



extern template
void SL_VertexProcessor::push_bin<RENDER_MODE_POINTS>(
    size_t,
    float,
    float,
    const SL_TransformedVert&,
    const SL_TransformedVert&,
    const SL_TransformedVert&
) const noexcept;

extern template
void SL_VertexProcessor::push_bin<RENDER_MODE_LINES>(
    size_t,
    float,
    float,
    const SL_TransformedVert&,
    const SL_TransformedVert&,
    const SL_TransformedVert&
) const noexcept;

extern template
void SL_VertexProcessor::push_bin<RENDER_MODE_TRIANGLES>(
    size_t,
    float,
    float,
    const SL_TransformedVert&,
    const SL_TransformedVert&,
    const SL_TransformedVert&
) const noexcept;



#endif /* SL_VERTEX_PROCESSOR_HPP */
