#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "../source/platform/window.hpp"
#include "../source/platform/event.hpp"
#include "../source/platform/renderer.hpp"

namespace py = pybind11;

PYBIND11_MODULE(platform_engine, m) {
    m.doc() = "Python bindings for the platform game engine (non-refactored)";

    // WindowConfig
    py::class_<platform::WindowConfig>(m, "WindowConfig")
        .def(py::init([](const std::string& title, int x, int y, int width, int height) {
            platform::WindowConfig config;
            config.title = title.c_str(); // Assume title is std::string; adjust if needed
            config.x = x;
            config.y = y;
            config.width = width;
            config.height = height;
            return config;
        }))
        .def_readwrite("title", &platform::WindowConfig::title)
        .def_readwrite("x", &platform::WindowConfig::x)
        .def_readwrite("y", &platform::WindowConfig::y)
        .def_readwrite("width", &platform::WindowConfig::width)
        .def_readwrite("height", &platform::WindowConfig::height)
        .def_readwrite("background_color", &platform::WindowConfig::background_color);

    // Color
    py::class_<platform::Color>(m, "Color")
        .def(py::init<unsigned char, unsigned char, unsigned char, unsigned char>())
        .def_readwrite("r", &platform::Color::r)
        .def_readwrite("g", &platform::Color::g)
        .def_readwrite("b", &platform::Color::b)
        .def_readwrite("a", &platform::Color::a);

    // Point
    py::class_<platform::Point>(m, "Point")
        .def(py::init<int, int, platform::Color, int>())
        .def_readwrite("x", &platform::Point::x)
        .def_readwrite("y", &platform::Point::y)
        .def_readwrite("color", &platform::Point::color)
        .def_readwrite("id", &platform::Point::id);

    // Line
    py::class_<platform::Line>(m, "Line")
        .def(py::init<int, int, int, int, platform::Color, int>())
        .def_readwrite("x1", &platform::Line::x1)
        .def_readwrite("y1", &platform::Line::y1)
        .def_readwrite("x2", &platform::Line::x2)
        .def_readwrite("y2", &platform::Line::y2)
        .def_readwrite("color", &platform::Line::color)
        .def_readwrite("id", &platform::Line::id);

    // Rectangle
    py::class_<platform::Rectangle>(m, "Rectangle")
        .def(py::init<int, int, int, int, platform::Color, bool, int>())
        .def_readwrite("x", &platform::Rectangle::x)
        .def_readwrite("y", &platform::Rectangle::y)
        .def_readwrite("width", &platform::Rectangle::width)
        .def_readwrite("height", &platform::Rectangle::height)
        .def_readwrite("color", &platform::Rectangle::color)
        .def_readwrite("filled", &platform::Rectangle::filled)
        .def_readwrite("id", &platform::Rectangle::id);

    // Window
    py::class_<platform::Window>(m, "Window")
        .def(py::init<platform::WindowConfig>())
        .def("show", &platform::Window::show)
        .def("should_run", &platform::Window::should_run);

    // EventKind enum
    py::enum_<platform::EventKind>(m, "EventKind")
        .value("NONE", platform::EventKind::NONE)
        .value("EXPOSE", platform::EventKind::EXPOSE)
        .value("KEY_SPACE", platform::EventKind::KEY_SPACE)
        .value("KEY_ESC", platform::EventKind::KEY_ESC)
        .value("EXIT", platform::EventKind::EXIT)
        .export_values();

    // Event
    py::class_<platform::Event>(m, "Event")
        .def(py::init<const platform::Window&>())
        .def("poll", &platform::Event::poll)
        .def("kind", &platform::Event::kind);

    // Renderer
    py::class_<platform::Renderer>(m, "Renderer")
        .def(py::init<const platform::Window&>())
        .def("set_draw_color", &platform::Renderer::set_draw_color)
        .def("clear", &platform::Renderer::clear)
        .def("draw_point", &platform::Renderer::draw_point)
        .def("draw_line", &platform::Renderer::draw_line)
        .def("draw_rect", &platform::Renderer::draw_rect)
        .def("remove_shape_by_id", &platform::Renderer::remove_shape_by_id)
        .def("present", &platform::Renderer::present);
}