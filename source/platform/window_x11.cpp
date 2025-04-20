#include "platform.hpp"
#include <iostream>
#include <chrono>
#include <thread>

#if defined(LINUX_PLATFORM)

using namespace std::chrono_literals;

// =============================================================

platform::PlatformWindow platform::NewPlatformWindow() {
    PlatformWindow pwindow = {}; // Zero-initialize struct

    // Open X11 display
    pwindow.display = XOpenDisplay(nullptr);
    if (!pwindow.display) {
        throw std::runtime_error("Failed to open X11 display");
    }

    // Get default screen
    pwindow.screen = DefaultScreen(pwindow.display);

    // Create a simple window
    pwindow.window = XCreateSimpleWindow(
        pwindow.display,
        RootWindow(pwindow.display, pwindow.screen),
        0, 0,           // Position (x, y)
        600, 600,       // Width, height
        0,              // Border width
        BlackPixel(pwindow.display, pwindow.screen), // Border color
        WhitePixel(pwindow.display, pwindow.screen)  // Background color
    );

    if (!pwindow.window) {
        XCloseDisplay(pwindow.display);
        throw std::runtime_error("Failed to create X11 window");
    }

    // Set window title
    XStoreName(pwindow.display, pwindow.window, "Hello, World!");

    // Log success (optional, could be replaced with a logging framework)
    std::printf("Created new X11 window\n");

    return pwindow;
}
void platform::ShowPlatformWindow(PlatformWindow* pwindow) {
    
     // Map the window to make it visible
     XMapWindow(pwindow->display, pwindow->window);
     XFlush(pwindow->display);
}


// =============================================================

void platform::SleepPlatformWindow(PlatformWindow* pwindow) {

    std::this_thread::sleep_for(5000ms);
}

// =============================================================

void platform::DestroyPlatformWindow(PlatformWindow* pwindow) {
    XDestroyWindow(pwindow->display, pwindow->window);
    XCloseDisplay(pwindow->display);
    std::printf("Destroy Window\n");
}

// =============================================================
#endif // === window_x11.cpp ===
// =============================================================
