#include "event.hpp"
#include <iostream>
#if __linux__
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/Xutil.h> // For XDrawArc
#endif

namespace platform {

  Event::Event(const Window& window)
  : kind_(EventKind::NONE),
  dpy_(window.get_display()),
  wd_(window.get_window()),
  x_(0),
  y_(0),
  keysym_(0) {
  }

  bool Event::poll(Renderer& renderer) {
    if (!XPending(dpy_)) {
      return false; // No events available
    }

    XNextEvent(dpy_, &event_);
    kind_ = EventKind::NONE;

    if (event_.type == Expose) {
      kind_ = EventKind::EXPOSE;
      return true;
    }
    else if (event_.type == KeyPress) {
      keysym_ = XLookupKeysym(&event_.xkey, 0);
      switch (keysym_) {
        case XK_a:
        case XK_A:
          kind_ = EventKind::KEY_A;
          std::cout << "Key 'A' pressed" << std::endl;
          return true;
        case XK_Escape:
          kind_ = EventKind::KEY_ESC;
          std::cout << "Key 'Escape' pressed" << std::endl;
          return true;
        case XK_Up:
          kind_ = EventKind::KEY_UP;
          std::cout << "Key 'Up' pressed" << std::endl;
          return true;
        case XK_Down:
          kind_ = EventKind::KEY_DOWN;
          std::cout << "Key 'Down' pressed" << std::endl;
          return true;
        case XK_Left:
          kind_ = EventKind::KEY_LEFT;
          std::cout << "Key 'Left' pressed" << std::endl;
          return true;
        case XK_Right:
          kind_ = EventKind::KEY_RIGHT;
          std::cout << "Key 'Right' pressed" << std::endl;
          return true;
        case XK_space:
          kind_ = EventKind::KEY_SPACE;
          std::cout << "Key 'Space' pressed" << std::endl;
          return true;
        default:
          std::cout << "Unhandled key pressed (keysym: " << keysym_ << ")" << std::endl;
          return false;
      }
    }
    else if (event_.type == ButtonPress) {
      x_ = event_.xbutton.x;
      y_ = event_.xbutton.y;
      switch (event_.xbutton.button) {
        case 1:
          kind_ = EventKind::LEFT_CLICK;
          std::cout << "Left click at (" << x_ << ", " << y_ << ")" << std::endl;
          return true;
        case 2:
          kind_ = EventKind::MIDDLE_CLICK;
          std::cout << "Middle click at (" << x_ << ", " << y_ << ")" << std::endl;
          return true;
        case 3:
          kind_ = EventKind::RIGHT_CLICK;
          std::cout << "Right click at (" << x_ << ", " << y_ << ")" << std::endl;
          return true;
        default:
          return false;
      }
    }
    else if (event_.type == ClientMessage) {
      kind_ = EventKind::EXIT;
      std::cout << "Window close requested" << std::endl;
      return true;
    }

    return false;
  }

  void Event::draw() const {
    if (kind_ != EventKind::LEFT_CLICK && kind_ != EventKind::RIGHT_CLICK && kind_ != EventKind::MIDDLE_CLICK) {
      return; // Only draw clicks
    }

    GC gc = DefaultGC(dpy_, DefaultScreen(dpy_));
    XSetForeground(dpy_, gc, 0x000000); // Black color
    XDrawArc(dpy_, wd_, gc, x_ - 5, y_ - 5, 10, 10, 0, 360 * 64); // Draw 10x10 circle
    XFlush(dpy_);
  }

} // namespace platform
