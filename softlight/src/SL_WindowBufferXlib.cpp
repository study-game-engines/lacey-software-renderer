
#include <cstdint> // fixed-width types
#include <cstdlib>
#include <utility> // std::move()

#include <unistd.h>
#include <string.h> // strerror()

extern "C"
{
    #include <X11/Xlib.h>
    #include <X11/Xutil.h>

    #if SL_ENABLE_XSHM != 0
        #include <X11/extensions/XShm.h>

        #include <sys/ipc.h> // IPC_CREAT
        #include <sys/shm.h> // shmget
        #include <sys/stat.h> // SL_IRWXU
    #endif /* SL_ENABLE_XSHM */
}

#include "lightsky/utils/Log.h"

#include "softlight/SL_Color.hpp"
#include "softlight/SL_RenderWindowXlib.hpp"
#include "softlight/SL_WindowBufferXlib.hpp"



/*-----------------------------------------------------------------------------
 *
-----------------------------------------------------------------------------*/
#if SL_ENABLE_XSHM != 0
/*-------------------------------------
 *
-------------------------------------*/
SL_WindowBufferXlib::~SL_WindowBufferXlib() noexcept
{
    terminate();
}



/*-------------------------------------
 *
-------------------------------------*/
SL_WindowBufferXlib::SL_WindowBufferXlib() noexcept :
    SL_WindowBuffer{},
    mWindow{nullptr},
    mBuffer{nullptr},
    mShmInfo{nullptr}
{}



/*-------------------------------------
 *
-------------------------------------*/
SL_WindowBufferXlib::SL_WindowBufferXlib(SL_WindowBufferXlib&& wb) noexcept :
    SL_WindowBuffer{std::move(wb)},
    mWindow{wb.mWindow},
    mBuffer{wb.mBuffer},
    mShmInfo{wb.mShmInfo}
{
    wb.mWindow = nullptr;
    wb.mBuffer = nullptr;
    wb.mShmInfo = nullptr;
}




/*-------------------------------------
 *
-------------------------------------*/
SL_WindowBufferXlib& SL_WindowBufferXlib::operator=(SL_WindowBufferXlib&& wb) noexcept
{
    if (this != &wb)
    {
        SL_WindowBuffer::operator=(std::move(wb));

        mWindow = wb.mWindow;
        wb.mWindow = nullptr;

        mBuffer = wb.mBuffer;
        wb.mBuffer = nullptr;

        mShmInfo = wb.mShmInfo;
        wb.mShmInfo = nullptr;
    }

    return *this;
}



/*-------------------------------------
 *
-------------------------------------*/
int SL_WindowBufferXlib::init(SL_RenderWindow& win, unsigned width, unsigned height) noexcept
{
    if (mBuffer)
    {
        return 0;
    }

    SL_RenderWindowXlib* pWin = dynamic_cast<SL_RenderWindowXlib*>(&win);

    if (pWin == nullptr)
    {
        return -1;
    }

    if (!pWin->valid())
    {
        return -2;
    }

    if (mTexture.init(SL_COLOR_RGBA_8U, width, height, 1) != 0)
    {
        return -3;
    }

    XWindowAttributes attribs;
    XGetWindowAttributes(pWin->mDisplay, pWin->mWindow, &attribs);

    char*            pTexData = reinterpret_cast<char*>(mTexture.data());
    XShmSegmentInfo* pShm     = new XShmSegmentInfo;
    XImage*          pImg     = XShmCreateImage(pWin->mDisplay, attribs.visual, attribs.depth, ZPixmap, pTexData, pShm, width, height);

    if (!pShm || !pImg)
    {
        delete pShm;
        mTexture.terminate();
        return -4;
    }

    // Some POSIX systems require that the user, group, and "other" can all
    // read to and write to the shared memory segment.
    constexpr int permissions =
        0
        | S_IREAD
        | S_IWRITE
        | S_IRGRP
        | S_IWGRP
        | S_IROTH
        | S_IWOTH
        | 0;

    // Textures on POSIX-based systems are page-aligned to ensure we can use
    // the X11-shared memory extension.
    // Hopefully this won't fail...
    pShm->shmid = shmget(IPC_PRIVATE, width*height*sizeof(SL_ColorRGBA8), IPC_CREAT|permissions);

    if (pShm->shmid < 0)
    {
        LS_LOG_ERR("Unable to allocate a shared memory segment: (", errno, ") ", strerror(errno));
        delete pShm;
        mTexture.terminate();
        XDestroyImage(pImg);
        return -5;
    }

    // Ensure we're using the texture's address
    pShm->shmaddr = pImg->data = (char*)shmat(pShm->shmid, pTexData, SHM_REMAP);
    pShm->readOnly = False;

    if ((long long)(pImg->data) < 0)
    {
        LS_LOG_ERR("Unable to bind a shared memory segment: (", errno, "): ", strerror(errno));
        delete pShm;
        mTexture.terminate();
        pImg->data = nullptr;
        XDestroyImage(pImg);
        return -6;
    }

    if (XShmAttach(pWin->mDisplay, pShm) == False)
    {
        shmdt(pShm->shmaddr);
        delete pShm;
        mTexture.terminate();
        pImg->data = nullptr;
        XDestroyImage(pImg);
        return -7;
    }

    mWindow = &win;
    mBuffer = pImg;
    mShmInfo = pShm;

    return 0;
}



/*-------------------------------------
 *
-------------------------------------*/
int SL_WindowBufferXlib::terminate() noexcept
{
    if (mBuffer)
    {
        mTexture.terminate();

        XImage* pImg = reinterpret_cast<XImage*>(mBuffer);
        pImg->data = nullptr; // OSX: Avoid double-free

        XDestroyImage(pImg);

        SL_RenderWindowXlib* pWin = dynamic_cast<SL_RenderWindowXlib*>(mWindow);
        XShmDetach(pWin->mDisplay, reinterpret_cast<XShmSegmentInfo*>(mShmInfo));

        mWindow = nullptr;
        mBuffer = nullptr;

        XShmSegmentInfo* pShm = reinterpret_cast<XShmSegmentInfo*>(mShmInfo);
        shmdt(pShm);
        delete pShm;
        mShmInfo = nullptr;
    }

    return 0;
}



/*-----------------------------------------------------------------------------
 *
-----------------------------------------------------------------------------*/
#else /* SL_ENABLE_XSHM */
/*-------------------------------------
 *
-------------------------------------*/
SL_WindowBufferXlib::~SL_WindowBufferXlib() noexcept
{
    terminate();
}



/*-------------------------------------
 *
-------------------------------------*/
SL_WindowBufferXlib::SL_WindowBufferXlib() noexcept :
    SL_WindowBuffer{},
    mWindow{nullptr},
    mBuffer{nullptr}
{}



/*-------------------------------------
 *
-------------------------------------*/
SL_WindowBufferXlib::SL_WindowBufferXlib(SL_WindowBufferXlib&& wb) noexcept :
    SL_WindowBuffer{std::move(wb)},
    mWindow{wb.mWindow},
    mBuffer{wb.mBuffer}
{
    wb.mWindow = nullptr;
    wb.mBuffer = nullptr;
}




/*-------------------------------------
 *
-------------------------------------*/
SL_WindowBufferXlib& SL_WindowBufferXlib::operator=(SL_WindowBufferXlib&& wb) noexcept
{
    if (this != &wb)
    {
        SL_WindowBuffer::operator=(std::move(wb));

        mWindow = wb.mWindow;
        wb.mWindow = nullptr;

        mBuffer = wb.mBuffer;
        wb.mBuffer = nullptr;
    }

    return *this;
}



/*-------------------------------------
 *
-------------------------------------*/
int SL_WindowBufferXlib::init(SL_RenderWindow& win, unsigned width, unsigned height) noexcept
{
    if (mBuffer)
    {
        return -1;
    }

    SL_RenderWindowXlib* pWin = dynamic_cast<SL_RenderWindowXlib*>(&win);

    if (pWin == nullptr)
    {
        return -2;
    }

    if (!pWin->valid())
    {
        return -3;
    }

    XWindowAttributes attribs;
    XGetWindowAttributes(pWin->mDisplay, pWin->mWindow, &attribs);

    if (mTexture.init(SL_COLOR_RGBA_8U, width, height, 1) != 0)
    {
        return -4;
    }

    char*   pTexData = reinterpret_cast<char*>(mTexture.data());
    XImage* pImg     = XCreateImage(pWin->mDisplay, attribs.visual, attribs.depth, ZPixmap, 0, pTexData, width, height, 32, 0);

    if (!pImg)
    {
        mTexture.terminate();
        return -5;
    }

    mWindow = &win;
    mBuffer = pImg;

    return 0;
}



/*-------------------------------------
 *
-------------------------------------*/
int SL_WindowBufferXlib::terminate() noexcept
{
    if (mBuffer)
    {
        mTexture.terminate();

        XImage* pImg = reinterpret_cast<XImage*>(mBuffer);
        pImg->data = nullptr; // OSX: Avoid double-free

        XDestroyImage(pImg);

        mWindow = nullptr;
        mBuffer = nullptr;
    }

    return 0;
}



#endif /* SL_ENABLE_XSHM */
