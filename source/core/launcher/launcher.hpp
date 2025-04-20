#ifndef LAUNCHER_HPP
#define LAUNCHER_HPP

#include <platform/platform.hpp>

namespace core 
{
    
    class Launcher
    {
        
        // ___PRIVATE___

        platform::PlatformWindow main_window;

        public: 
            Launcher() : main_window(platform::NewPlatformWindow()) {};
            ~Launcher() {
                platform::DestroyPlatformWindow(&main_window);
            }

            void launcher_run();
    };
}

// =============================================================
#endif // ==== launcher.hpp ====
// =============================================================