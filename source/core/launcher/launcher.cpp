#include "launcher.hpp"

void core::Launcher::launcher_run() {
    platform::ShowPlatformWindow(&main_window);
    platform::SleepPlatformWindow(&main_window);
}