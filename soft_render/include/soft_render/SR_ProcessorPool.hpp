
#ifndef SR_PROCESSORPOOL_HPP
#define SR_PROCESSORPOOL_HPP

#include <atomic>

#include "lightsky/utils/Pointer.h" // Pointer, AlignedPointerDeleter



/*-----------------------------------------------------------------------------
 * Forward Declarations
-----------------------------------------------------------------------------*/
namespace ls
{
namespace utils
{
template <class T>
class Worker;

template <class T>
class WorkerThread;
} // utils namespace

namespace math
{
template <typename T>
union vec4_t;
} // math namespace
} // ls namespace

class SR_Context;
struct SR_FragCoord;
struct SR_FragmentBin;
class SR_Framebuffer;
struct SR_Mesh;
class SR_Shader;
struct SR_ShaderProcessor;
class SR_Texture;



/*-----------------------------------------------------------------------------
 * Wrapper around Shader Processors which allows operations on binned
 * Fragments
-----------------------------------------------------------------------------*/
class SR_ProcessorPool
{
  public:
    typedef ls::utils::Worker<SR_ShaderProcessor> Worker;
    typedef ls::utils::WorkerThread<SR_ShaderProcessor> ThreadedWorker;

  private:
    std::atomic_uint_fast64_t mFragSemaphore;

    std::atomic_uint_fast64_t mShadingSemaphore;

    ls::utils::Pointer<std::atomic_int_fast32_t[], ls::utils::AlignedDeleter> mBinsReady;

    ls::utils::Pointer<uint32_t[], ls::utils::AlignedDeleter> mBinsUsed;

    ls::utils::Pointer<SR_FragmentBin[], ls::utils::AlignedDeleter> mFragBins;

    ls::utils::Pointer<ls::math::vec4_t<float>[], ls::utils::AlignedDeleter> mVaryings;

    ls::utils::Pointer<SR_FragCoord[], ls::utils::AlignedDeleter> mFragQueues;

    ls::utils::Pointer<Worker*[], ls::utils::AlignedDeleter> mThreads;

    unsigned mNumThreads;

  public:
    ~SR_ProcessorPool() noexcept;

    SR_ProcessorPool(unsigned numThreads = 1) noexcept;

    SR_ProcessorPool(const SR_ProcessorPool&) noexcept;

    SR_ProcessorPool(SR_ProcessorPool&&) noexcept;

    SR_ProcessorPool& operator=(const SR_ProcessorPool&) noexcept;

    SR_ProcessorPool& operator=(SR_ProcessorPool&&) noexcept;

    unsigned num_threads() const noexcept;

    unsigned num_threads(unsigned n) noexcept;

    void flush() noexcept;

    void wait() noexcept;

    void execute() noexcept;

    void run_shader_processors(const SR_Context& c, const SR_Mesh& m, size_t numInstances, const SR_Shader& s, SR_Framebuffer& fbo) noexcept;

    void run_shader_processors(const SR_Context& c, const SR_Mesh* meshes, size_t numMeshes, const SR_Shader& s, SR_Framebuffer& fbo) noexcept;

    void clear_fragment_bins() noexcept;

    void run_blit_processors(
        const SR_Texture* inTex,
        SR_Texture* outTex,
        uint16_t srcX0,
        uint16_t srcY0,
        uint16_t srcX1,
        uint16_t srcY1,
        uint16_t dstX0,
        uint16_t dstY0,
        uint16_t dstX1,
        uint16_t dstY1
    ) noexcept;
};



/*--------------------------------------
 * Retrieve the number of threads
--------------------------------------*/
inline unsigned SR_ProcessorPool::num_threads() const noexcept
{
    return mNumThreads;
}



/*-------------------------------------
 * Run the processor threads
-------------------------------------*/
inline void SR_ProcessorPool::execute() noexcept
{
    this->flush();
    this->wait();
}



/*-------------------------------------
 * Remove all bins from potential processing
-------------------------------------*/
inline void SR_ProcessorPool::clear_fragment_bins() noexcept
{
    for (uint16_t t = 0; t < mNumThreads; ++t)
    {
        mBinsReady[t].store(-1, std::memory_order_release);
        mBinsUsed[t] = 0;
    }
}


#endif /* SR_PROCESSORPOOL_HPP */
