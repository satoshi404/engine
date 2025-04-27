#ifndef PLATFORM_RENDERER_H
#define PLATFORM_RENDERER_H

#include "window.hpp"
#include <vector>
#include <variant>
#include <functional>

namespace platform {

    struct Color {
        unsigned char r, g, b, a;
        unsigned long x11_color;
        void allocate(Display* dpy, Colormap cmap);
    };

    struct Point {
        int x, y;
        Color color;
        int id; // Unique identifier
    };

    struct Line {
        int x1, y1, x2, y2;
        Color color;
        int id;
    };

    struct Rectangle {
        int x, y, width, height;
        Color color;
        bool filled;
        int id;
    };

    using Shape = std::variant<Point, Line, Rectangle>;

    class Renderer {
    public:
        Renderer(const Window& window);
        ~Renderer();
        void set_draw_color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);
        void clear();
        void draw_point(int x, int y, int id = 0);
        void draw_line(int x1, int y1, int x2, int y2, int id = 0);
        void draw_rect(int x, int y, int width, int height, bool filled = false, int id = 0);
        void remove_shape_by_id(int id);
        void present();

    private:
        Display* dpy_;
        ::Window wd_;
        Pixmap buffer_;
        int width_, height_;
        Colormap cmap_;
        GC gc_;
        Color draw_color_;
        std::vector<Shape> shapes_;
    };

} // namespace platform

#endif // PLATFORM_RENDERER_H