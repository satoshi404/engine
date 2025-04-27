#include "event.hpp"
#include <X11/keysym.h>
#include <X11/Xutil.h>
#include <iostream>

namespace platform {

    Event::Event(const Window& window)
        : kind_(EventKind::NONE),
          dpy_(window.get_display()),
          wd_(window.get_window()),
          x_(0),
          y_(0),
          keysym_(0) {
        std::cout << "Initialized event handler" << std::endl;
    }

    bool Event::poll(Renderer& renderer) {
        if (!XPending(dpy_)) {
            return false;
        }

        XNextEvent(dpy_, &event_);
        kind_ = EventKind::NONE;

        if (event_.type == Expose) {
            kind_ = EventKind::EXPOSE;
            std::cout << "Event: EXPOSE" << std::endl;
            return true;
        } else if (event_.type == KeyPress) {
            keysym_ = XLookupKeysym(&event_.xkey, 0);
            if (keysym_ == XK_space) {
                kind_ = EventKind::KEY_SPACE;
                std::cout << "Event: KEY_SPACE" << std::endl;
                return true;
            } else if (keysym_ == XK_Escape) {
                kind_ = EventKind::KEY_ESC;
                std::cout << "Event: KEY_ESC" << std::endl;
                return true;
            }
        } else if (event_.type == ClientMessage) {
            kind_ = EventKind::EXIT;
            std::cout << "Event: EXIT" << std::endl;
            return true;
        }

        return false;
    }

    EventKind Event::kind() const {
        return kind_;
    }

} // namespace platform