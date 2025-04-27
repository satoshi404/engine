#ifndef PLATFORM_WINDOW_HPP
#define PLATFORM_WINDOW_HPP

#ifdef __linux__
#include <X11/Xlib.h>
#include <X11/Xauth.h>
#include <X11/Xatom.h>
#else
#error "Platform not supported"
#endif

namespace platform {

    // Configuration for window creation
    struct WindowConfig {
        const char* title = "Hello, World";
        int x = 0;
        int y = 0;
        int width = 700;
        int height = 700;
        unsigned long background_color = 0xffffff; // White
    };

    enum class State {
        RUNNING,
        CLOSE
    };

    class Window {
    public:
        Window(const WindowConfig& config = WindowConfig());
        ~Window();

        void show();
        State should_run() const;
        Display* get_display() const { return dpy_; }
        ::Window get_window() const { return wd_; }

    private:
        Display* dpy_;
        ::Window wd_;
        int scr_;
    };

} // namespace platform

#endif // PLATFORM_WINDOW_HPP
