#include <platform/event.hpp>
#include <iostream>
#include <chrono>
#include <thread>

int main() {
    try {
        platform::WindowConfig config;
        config.title = "X11 SDL2-Like Renderer with Easy Rectangles";
        config.width = 800;
        config.height = 600;

        platform::Window window(config);
        platform::Renderer renderer(window);
        window.show();

        // Initial rendering (static shapes)
        renderer.set_draw_color(0, 0, 100, 255); // Dark blue background
        renderer.draw_rect(0, 0, config.width, config.height, true, 1); // ID 1 for background

        // Draw grid of white points (20x20 spacing)
        renderer.set_draw_color(255, 255, 255, 255);
        int point_id = 100; // Start IDs for points
        for (int x = 0; x < config.width; x += 20) {
            for (int y = 0; y < config.height; y += 20) {
                renderer.draw_point(x, y, point_id++);
            }
        }
        renderer.present(); // Show initial scene

        // Animated rectangles
        struct AnimatedRect {
            int x, y, width, height;
            int speed_x, speed_y;
            int id;
            unsigned char r, g, b;
        };
        std::vector<AnimatedRect> rects = {
            {0, 0, 20, 20, 2, 2, 2, 255, 0, 0}, // Red square
            {100, 100, 30, 30, -3, 1, 3, 0, 255, 0}, // Green square
        };

        platform::Event event(window);
        auto last_frame = std::chrono::steady_clock::now();
        while (window.should_run() == platform::State::RUNNING) {
            // Handle events
            if (event.poll(renderer)) {
                if (event.kind() == platform::EventKind::EXIT || event.kind() == platform::EventKind::KEY_ESC) {
                    std::cout << "Exiting" << std::endl;
                    break;
                }
            }

            // Update animation (~60 FPS)
            auto now = std::chrono::steady_clock::now();
            auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_frame).count();
            if (delta >= 16) { // ~60 FPS (1000ms / 60)
                // Update animated rectangles
                for (auto& rect : rects) {
                    rect.x += rect.speed_x;
                    rect.y += rect.speed_y;
                    if (rect.x > config.width - rect.width) rect.x = 0;
                    if (rect.x < 0) rect.x = config.width - rect.width;
                    if (rect.y > config.height - rect.height) rect.y = 0;
                    if (rect.y < 0) rect.y = config.height - rect.height;

                    // Update rectangle with ID
                    renderer.remove_shape_by_id(rect.id);
                    renderer.set_draw_color(rect.r, rect.g, rect.b, 255);
                    renderer.draw_rect(rect.x, rect.y, rect.width, rect.height, true, rect.id);
                }

                // Redraw entire scene
                renderer.present();

                last_frame = now;
            }

            // Stabilize frame rate
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}