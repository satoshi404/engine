#include <iostream>
#include <core/launcher/launcher.hpp>

auto main(int argc, char** argv) -> int {
    core::Launcher launcher;
    launcher.launcher_run();
    return EXIT_SUCCESS;
}