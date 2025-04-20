#ifndef PLATFORM_HPP
#define PLATFORM_HPP

#ifdef __linux__
    #define LINUX_PLATFORM
    #include <X11/Xlib.h>
    #include <X11/Xatom.h>
    #include <X11/Xauth.h>
#else 
    #error "PLATFORM NOT SUPPORTED"
#endif

namespace platform 
{ 
    typedef struct 
    {
        #if defined(LINUX_PLATFORM)
            Display* display;
            Window window;
            int screen;
        #else
            #error "PLATFORM NOT SUPPORTED"
        #endif
    } PlatformWindow;


    PlatformWindow NewPlatformWindow();
    void SleepPlatformWindow(PlatformWindow* pwindow);
    void ShowPlatformWindow(PlatformWindow* pwindow);
    void DestroyPlatformWindow(PlatformWindow* pwindow);
}
// =============================================================
#endif // ==== platform.hpp ====
// =============================================================
