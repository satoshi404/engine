#include "renderer.hpp"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <algorithm>
#include <iostream>

namespace platform {

void Color::allocate(Display* dpy, Colormap cmap) {
    XColor xcolor;
    xcolor.red = r << 8;
    xcolor.green = g << 8;
    xcolor.blue = b << 8;
    xcolor.flags = DoRed | DoGreen | DoBlue;
    if (XAllocColor(dpy, cmap, &xcolor)) {
        x11_color = xcolor.pixel;
        std::cout << "Allocated color RGB(" << (int)r << "," << (int)g << "," << (int)b << ") = " << x11_color << std::endl;
    } else {
        std::cerr << "Failed to allocate color RGB(" << (int)r << "," << (int)g << "," << (int)b << ")" << std::endl;
        x11_color = WhitePixel(dpy, DefaultScreen(dpy));
    }
}

Renderer::Renderer(const Window& window)
    : dpy_(window.get_display()),
      wd_(window.get_window()),
      width_(800),
      height_(600),
      cmap_(DefaultColormap(dpy_, DefaultScreen(dpy_))),
      gc_(DefaultGC(dpy_, DefaultScreen(dpy_))),
      draw_color_{255, 255, 255, 255, 0} {
    XSetErrorHandler([](Display* dpy, XErrorEvent* e) {
        char msg[256];
        XGetErrorText(dpy, e->error_code, msg, sizeof(msg));
        std::cerr << "X11 Error: " << msg << " (code: " << (int)e->error_code << ")" << std::endl;
        return 0;
    });
    draw_color_.allocate(dpy_, cmap_);
    buffer_ = XCreatePixmap(dpy_, wd_, width_, height_, DefaultDepth(dpy_, DefaultScreen(dpy_)));
    if (!buffer_) {
        std::cerr << "Failed to create pixmap" << std::endl;
    }
    XSetForeground(dpy_, gc_, draw_color_.x11_color);
    XFillRectangle(dpy_, buffer_, gc_, 0, 0, width_, height_);
    std::cout << "Initialized renderer with buffer size " << width_ << "x" << height_ << std::endl;
}

Renderer::~Renderer() {
    if (buffer_) {
        XFreePixmap(dpy_, buffer_);
    }
}

void Renderer::set_draw_color(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
    draw_color_ = {r, g, b, a, 0};
    draw_color_.allocate(dpy_, cmap_);
}

void Renderer::clear() {
    shapes_.clear();
    XSetForeground(dpy_, gc_, draw_color_.x11_color);
    XFillRectangle(dpy_, buffer_, gc_, 0, 0, width_, height_);
    std::cout << "Cleared shapes" << std::endl;
}

void Renderer::draw_point(int x, int y, int id) {
    shapes_.push_back(Point{x, y, draw_color_, id});
    XSetForeground(dpy_, gc_, draw_color_.x11_color);
    XDrawPoint(dpy_, buffer_, gc_, x, y);
    std::cout << "Drew point at (" << x << "," << y << ") with id " << id << std::endl;
}

void Renderer::draw_line(int x1, int y1, int x2, int y2, int id) {
    shapes_.push_back(Line{x1, y1, x2, y2, draw_color_, id});
    XSetForeground(dpy_, gc_, draw_color_.x11_color);
    XDrawLine(dpy_, buffer_, gc_, x1, y1, x2, y2);
    std::cout << "Drew line from (" << x1 << "," << y1 << ") to (" << x2 << "," << y2 << ") with id " << id << std::endl;
}

void Renderer::draw_rect(int x, int y, int width, int height, bool filled, int id) {
    shapes_.push_back(Rectangle{x, y, width, height, draw_color_, filled, id});
    XSetForeground(dpy_, gc_, draw_color_.x11_color);
    if (filled) {
        XFillRectangle(dpy_, buffer_, gc_, x, y, width, height);
    } else {
        XDrawRectangle(dpy_, buffer_, gc_, x, y, width, height);
    }
    std::cout << "Drew rectangle at (" << x << "," << y << ") size (" << width << "," << height << ") with id " << id << std::endl;
}

void Renderer::remove_shape_by_id(int id) {
    shapes_.erase(std::remove_if(shapes_.begin(), shapes_.end(),
        [id](const Shape& shape) {
            return std::visit([id](const auto& s) { return s.id == id; }, shape);
        }), shapes_.end());
    std::cout << "Removed shape with id " << id << std::endl;
}

void Renderer::present() {
    std::cout << "Rendering " << shapes_.size() << " shapes" << std::endl;
    XSetForeground(dpy_, gc_, draw_color_.x11_color);
    XFillRectangle(dpy_, buffer_, gc_, 0, 0, width_, height_);
    for (const auto& shape : shapes_) {
        std::visit([&](const auto& s) {
            XSetForeground(dpy_, gc_, s.color.x11_color);
            if constexpr (std::is_same_v<std::decay_t<decltype(s)>, Point>) {
                XDrawPoint(dpy_, buffer_, gc_, s.x, s.y);
            } else if constexpr (std::is_same_v<std::decay_t<decltype(s)>, Line>) {
                XDrawLine(dpy_, buffer_, gc_, s.x1, s.y1, s.x2, s.y2);
            } else if constexpr (std::is_same_v<std::decay_t<decltype(s)>, Rectangle>) {
                if (s.filled) {
                    XFillRectangle(dpy_, buffer_, gc_, s.x, s.y, s.width, s.height);
                } else {
                    XDrawRectangle(dpy_, buffer_, gc_, s.x, s.y, s.width, s.height);
                }
            }
        }, shape);
    }
    XSetForeground(dpy_, gc_, WhitePixel(dpy_, DefaultScreen(dpy_)));
    XFillRectangle(dpy_, wd_, gc_, 100, 100, 200, 200); // Direct draw test
    XCopyArea(dpy_, buffer_, wd_, gc_, 0, 0, width_, height_, 0, 0);
    XFlush(dpy_);
}

} // namespace platform