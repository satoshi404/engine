#include <platform/event.hpp>
#include <iostream>
#include <chrono>
#include <thread>
#include <random>
#include <vector>
#include <algorithm>

struct Pipe {
    int x; // X position of pipe pair
    int gap_y; // Y position of gap center
    int id_top, id_bottom; // IDs for top and bottom pipe rectangles
    bool scored; // Whether bird passed this pipe
};

struct Bird {
    float x, y; // Position
    float velocity; // Vertical velocity
    int id; // ID for rectangle
};

int main() {
    try {
        platform::WindowConfig config;
        config.title = "Flappy Bird with X11 Renderer";
        config.width = 800;
        config.height = 600;

        platform::Window window(config);
        platform::Renderer renderer(window);
        window.show();

        // Initial rendering (static shapes)
        renderer.set_draw_color(0, 0, 100, 255); // Dark blue background
        renderer.draw_rect(0, 0, config.width, config.height, true, 1); // ID 1

        // Ground (green rectangle)
        renderer.set_draw_color(0, 255, 0, 255);
        renderer.draw_rect(0, config.height - 50, config.width, 50, true, 2); // ID 2

        // Grid of white points (20x20 spacing, above ground)
        renderer.set_draw_color(255, 255, 255, 255);
        int point_id = 100;
        for (int x = 0; x < config.width; x += 20) {
            for (int y = 0; y < config.height - 50; y += 20) {
                renderer.draw_point(x, y, point_id++);
            }
        }
        renderer.present();

        // Game state
        Bird bird{200.0f, 300.0f, 0.0f, 3}; // Start at (200, 300), ID 3
        std::vector<Pipe> pipes;
        bool game_over = false;
        int score = 0;
        const float gravity = 0.5f;
        const float flap_velocity = -10.0f;
        const int pipe_width = 50;
        const int gap_size = 150;
        const int pipe_spacing = 200;
        const float pipe_speed = 2.0f;

        // Random number generator for gap positions
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> gap_dist(100, config.height - 150); // Gap center range

        // Spawn initial pipe
        pipes.push_back({config.width, gap_dist(gen), 1000, 1001, false});

        platform::Event event(window);
        auto last_frame = std::chrono::steady_clock::now();
        while (window.should_run() == platform::State::RUNNING) {
            // Handle events
            if (event.poll(renderer)) {
                if (event.kind() == platform::EventKind::EXIT || event.kind() == platform::EventKind::KEY_ESC) {
                    std::cout << "Exiting" << std::endl;
                    break;
                }
                if (event.kind() == platform::EventKind::KEY_SPACE) {
                    if (game_over) {
                        // Restart game
                        bird = {200.0f, 300.0f, 0.0f, 3};
                        for (const auto& pipe : pipes) {
                            renderer.remove_shape_by_id(pipe.id_top);
                            renderer.remove_shape_by_id(pipe.id_bottom);
                        }
                        pipes.clear();
                        pipes.push_back({config.width, gap_dist(gen), 1000, 1001, false});
                        score = 0;
                        game_over = false;
                    } else {
                        // Flap bird
                        bird.velocity = flap_velocity;
                    }
                }
            }

            // Update game (~60 FPS)
            auto now = std::chrono::steady_clock::now();
            auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_frame).count();
            if (delta >= 16) { // ~60 FPS
                if (!game_over) {
                    // Update bird
                    bird.velocity += gravity;
                    bird.y += bird.velocity;
                    renderer.remove_shape_by_id(bird.id);
                    renderer.set_draw_color(255, 255, 0, 255); // Yellow bird
                    renderer.draw_rect(static_cast<int>(bird.x), static_cast<int>(bird.y), 20, 20, true, bird.id);

                    // Update pipes
                    for (auto& pipe : pipes) {
                        pipe.x -= pipe_speed;
                        renderer.remove_shape_by_id(pipe.id_top);
                        renderer.remove_shape_by_id(pipe.id_bottom);
                        renderer.set_draw_color(0, 255, 0, 255); // Green pipes
                        // Top pipe (from top to gap_y - gap_size/2)
                        renderer.draw_rect(pipe.x, 0, pipe_width, pipe.gap_y - gap_size / 2, true, pipe.id_top);
                        // Bottom pipe (from gap_y + gap_size/2 to ground)
                        renderer.draw_rect(pipe.x, pipe.gap_y + gap_size / 2, pipe_width, config.height - 50 - (pipe.gap_y + gap_size / 2), true, pipe.id_bottom);

                        // Score when bird passes pipe
                        if (!pipe.scored && pipe.x + pipe_width < bird.x) {
                            score++;
                            pipe.scored = true;
                            std::cout << "Score: " << score << std::endl;
                        }
                    }

                    // Spawn new pipe
                    if (!pipes.empty() && pipes.back().x <= config.width - pipe_spacing) {
                        int new_id = pipes.back().id_top + 2;
                        pipes.push_back({config.width, gap_dist(gen), new_id, new_id + 1, false});
                    }

                    // Remove off-screen pipes
                    pipes.erase(std::remove_if(pipes.begin(), pipes.end(),
                        [&](const Pipe& pipe) {
                            if (pipe.x + pipe_width < 0) {
                                renderer.remove_shape_by_id(pipe.id_top);
                                renderer.remove_shape_by_id(pipe.id_bottom);
                                return true;
                            }
                            return false;
                        }), pipes.end());

                    // Collision detection
                    for (const auto& pipe : pipes) {
                        // Bird rectangle
                        int bx = static_cast<int>(bird.x);
                        int by = static_cast<int>(bird.y);
                        int bw = 20, bh = 20;
                        // Top pipe
                        int tx = pipe.x, ty = 0, tw = pipe_width, th = pipe.gap_y - gap_size / 2;
                        // Bottom pipe
                        int bx2 = pipe.x, by2 = pipe.gap_y + gap_size / 2, bw2 = pipe_width, bh2 = config.height - 50 - (pipe.gap_y + gap_size / 2);
                        // AABB collision
                        if ((bx < tx + tw && bx + bw > tx && by < ty + th && by + bh > ty) ||
                            (bx < bx2 + bw2 && bx + bw > bx2 && by < by2 + bh2 && by + bh > by2)) {
                            game_over = true;
                            std::cout << "Game Over! Score: " << score << std::endl;
                            // Clear pipes immediately
                            for (const auto& pipe : pipes) {
                                renderer.remove_shape_by_id(pipe.id_top);
                                renderer.remove_shape_by_id(pipe.id_bottom);
                            }
                            pipes.clear();
                        }
                    }
                    // Ground and ceiling collision
                    if (bird.y + 20 > config.height - 50 || bird.y < 0) {
                        game_over = true;
                        std::cout << "Game Over! Score: " << score << std::endl;
                        // Clear pipes immediately
                        for (const auto& pipe : pipes) {
                            renderer.remove_shape_by_id(pipe.id_top);
                            renderer.remove_shape_by_id(pipe.id_bottom);
                        }
                        pipes.clear();
                    }
                }

                renderer.present();
                last_frame = now;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(2));
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}