#ifndef PLATFORM_GAME_H
#define PLATFORM_GAME_H

#include "window.hpp"
#include "renderer.hpp"
#include "event.hpp"
#include <memory>

namespace platform {

    class Game {
    public:
        Game(const WindowConfig& config);
        void add_object(std::shared_ptr<GameObject> obj);
        void set_event_callback(Event::EventCallback callback);
        void run();
    protected:
        virtual void update(float delta_time) {}
        std::unique_ptr<Window> window_;
        std::unique_ptr<Renderer> renderer_;
        std::unique_ptr<Event> event_;
        std::vector<std::shared_ptr<GameObject>> objects_;
        bool running_;
    };

} // namespace platform

#endif // PLATFORM_GAME_H