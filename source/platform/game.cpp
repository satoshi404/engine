#include "game.hpp"
#include <chrono>
#include <thread>

namespace platform {

    Game::Game(const WindowConfig& config)
        : window_(std::make_unique<Window>(config)),
          renderer_(std::make_unique<Renderer>(*window_)),
          event_(std::make_unique<Event>(*window_)),
          running_(true) {
        window_->show();
    }

    void Game::add_object(std::shared_ptr<GameObject> obj) {
        objects_.push_back(obj);
        renderer_->add_object(obj);
    }

    void Game::set_event_callback(Event::EventCallback callback) {
        event_->set_callback(callback);
    }

    void Game::run() {
        auto last_frame = std::chrono::steady_clock::now();
        while (running_ && window_->should_run() == State::RUNNING) {
            while (event_->poll()) {
                if (event_->kind() == EventKind::EXIT || event_->kind() == EventKind::KEY_ESC) {
                    running_ = false;
                }
            }

            auto now = std::chrono::steady_clock::now();
            auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_frame).count();
            if (delta >= 16) { // ~60 FPS
                float delta_time = delta / 1000.0f;
                update(delta_time);
                for (auto& obj : objects_) {
                    obj->update(delta_time);
                }
                renderer_->render();
                last_frame = now;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(2));
        }
    }

} // namespace platform