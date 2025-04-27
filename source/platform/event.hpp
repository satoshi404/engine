#ifndef PLATFORM_EVENT_H
#define PLATFORM_EVENT_H

#include "window.hpp"
#include "renderer.hpp"

namespace platform {

    enum class EventKind {
        EXIT,
        NONE,
        LEFT_CLICK,
        RIGHT_CLICK,
        MIDDLE_CLICK,
        KEY_A,
        KEY_ESC,
        KEY_UP,
        KEY_DOWN,
        KEY_LEFT,
        KEY_RIGHT,
        KEY_SPACE,
        EXPOSE
    };

    class Event {
    public:
        Event(const Window& window);
        bool poll(Renderer& renderer);
        void draw() const;
        void wait() const;
        EventKind kind() const;


        int x() const { return x_; }
        int y() const { return y_; }
        KeySym keysym() const { return keysym_; }

    private:
        EventKind kind_;
        Display* dpy_;
        ::Window wd_;
        XEvent event_;
        int x_;
        int y_;
        KeySym keysym_;
    };

} // namespace platform

#endif // PLATFORM_EVENT_H
