/*
 * TODO:
 *
 * - Support window parenting
 *
 * - Integrate window buffers
 */

#ifndef SR_RENDERWINDOW_HPP
#define SR_RENDERWINDOW_HPP

#include <cstddef> // ptrdiff_t

#include "lightsky/utils/Pointer.h"

#include "soft_render/SR_WindowEvent.hpp"



/*-----------------------------------------------------------------------------
 *
-----------------------------------------------------------------------------*/
class SR_WindowBuffer;


/*-----------------------------------------------------------------------------
 * Enumeration to capture the state of a window
-----------------------------------------------------------------------------*/
enum WindowStateInfo
{
    WINDOW_STARTING,
    WINDOW_STARTED,
    WINDOW_RUNNING,
    WINDOW_PAUSED,
    WINDOW_CLOSED,
    WINDOW_CLOSING
};


/*-----------------------------------------------------------------------------
 *
-----------------------------------------------------------------------------*/
class SR_RenderWindow
{
  public:
    static ls::utils::Pointer<SR_RenderWindow> create() noexcept;

  protected:
    WindowStateInfo mCurrentState;

  public:
    virtual ~SR_RenderWindow()  noexcept = 0;

    SR_RenderWindow() noexcept;

    SR_RenderWindow(const SR_RenderWindow&) noexcept;

    SR_RenderWindow(SR_RenderWindow&&) noexcept;

    SR_RenderWindow& operator=(const SR_RenderWindow&) noexcept;

    SR_RenderWindow& operator=(SR_RenderWindow&&) noexcept;

    virtual int set_title(const char* const pName) noexcept = 0;

    virtual int init(unsigned width = 640, unsigned height = 480) noexcept = 0;

    virtual int destroy() noexcept = 0;

    virtual unsigned width() const noexcept = 0;

    virtual unsigned height() const noexcept = 0;

    virtual void get_size(unsigned& width, unsigned& height) const noexcept = 0;

    virtual bool set_size(unsigned width, unsigned height) noexcept = 0;

    virtual int x_position() const noexcept = 0;

    virtual int y_position() const noexcept = 0;

    virtual bool get_position(int& width, int& height) const noexcept = 0;

    virtual bool set_position(int x, int y) noexcept = 0;

    virtual SR_RenderWindow* clone() const noexcept = 0;

    virtual bool valid() const noexcept = 0;

    virtual WindowStateInfo state() const noexcept = 0;

    virtual void update() noexcept = 0;

    virtual bool pause() noexcept = 0;

    virtual bool run() noexcept = 0;

    virtual bool has_event() const noexcept = 0;

    virtual bool peek_event(SR_WindowEvent* const pEvent) noexcept = 0;

    virtual bool pop_event(SR_WindowEvent* const pEvent) noexcept = 0;

    virtual bool set_keys_repeat(bool doKeysRepeat) noexcept = 0;

    virtual bool keys_repeat() const noexcept = 0;

    virtual void render(SR_WindowBuffer& buffer) noexcept = 0;

    virtual void set_mouse_capture(bool isCaptured) noexcept = 0;

    virtual bool is_mouse_captured() const noexcept = 0;

    virtual void* native_handle() noexcept = 0;

    virtual const void* native_handle() const noexcept = 0;
};



#endif /* SR_RENDERWINDOW_HPP */
