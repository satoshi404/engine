#include "window.hpp"
#include <stdexcept>
#include <iostream>

namespace platform {

    Window::Window(const WindowConfig& config) {
        dpy_ = XOpenDisplay(nullptr);
        if (!dpy_) {
            throw std::runtime_error("ERROR: Failed to open X11 display");
        }

        scr_ = XDefaultScreen(dpy_);
        wd_ = XCreateSimpleWindow(dpy_, XRootWindow(dpy_, scr_),
                                  config.x, config.y, config.width, config.height,
                                  0, 0, config.background_color);
        if (!wd_) {
            XCloseDisplay(dpy_);
            throw std::runtime_error("ERROR: Failed to create X11 window");
        }

        Atom del_window = XInternAtom(dpy_, "WM_DELETE_WINDOW", 0);
        XSetWMProtocols(dpy_, wd_, &del_window, 1);

        XSelectInput(dpy_, wd_, ExposureMask | KeyPressMask | ButtonPressMask);

        XStoreName(dpy_, wd_, config.title);

        std::cout << "Created window on screen " << scr_ << " with size "
                  << config.width << "x" << config.height << std::endl;
    }

    Window::~Window() {
        if (wd_ && dpy_) {
            XDestroyWindow(dpy_, wd_);
        }
        if (dpy_) {
            XCloseDisplay(dpy_);
        }
        std::cout << "Destroyed window" << std::endl;
    }

    void Window::show() {
        if (!dpy_ || !wd_) {
            throw std::runtime_error("ERROR: Cannot show invalid window");
        }
        XMapWindow(dpy_, wd_);
        XFlush(dpy_);
        std::cout << "Mapped window to display" << std::endl;

        // Force expose event
        XExposeEvent ev = { Expose, 0, 1, dpy_, wd_, 0, 0, 800, 600, 0 };
        XSendEvent(dpy_, wd_, False, ExposureMask, (XEvent*)&ev);
        XFlush(dpy_);
    }

    State Window::should_run() const {
        return (dpy_ && wd_) ? State::RUNNING : State::CLOSE;
    }

} // namespace platform