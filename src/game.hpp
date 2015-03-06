/**
 *
 */

#ifndef RPEC__GAME__H_
#define RPEC__GAME__H_

#include <chrono>
#include <iostream>
#include <memory>
#include <unordered_map>

#include <rxcpp/rx.hpp>
#include <SDL2/SDL.h>

#include "events/controls.hpp"
#include "events/events.hpp"
#include "events/sdlevents.hpp"
#include "entities/entity.hpp"
#include "entities/player.hpp"
#include "graphics/renderer.hpp"
#include "graphics/sdlopenglrenderer.hpp"

namespace rpec {

class Game : public events::EventHandler {
public:
    Game()
    : events::EventHandler()
    , m_running((SDL_Init(0), false))
    , m_startTime()
    , m_elapsedMs()
    , m_renderer(new graphics::SdlOpenGlRenderer())
    , m_events(new events::SdlEvents(m_elapsedMs.get_observable()))
    , m_controls(m_events.get())
    , m_entities()
    { }

    void run() {
        m_running = true;
        m_startTime = std::chrono::high_resolution_clock::now();

        m_events->events().subscribe([this](auto e) {
            handle(e);
        });

        auto player = new entities::Player();
        m_controls.controls().subscribe([player](auto c) {
            player->handle(c);
        });
        std::unique_ptr<entities::Entity> playerEntity(player);
        m_entities.insert(std::make_pair(playerEntity->id(), std::move(playerEntity)));

        while (m_running) {
            auto now = std::chrono::high_resolution_clock::now();
            auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_startTime);

            m_events->poll();
            m_elapsedMs.get_subscriber().on_next(elapsedMs);
            m_events->render(m_renderer.get());
        }
    }

    ~Game() {
        SDL_Quit();
    }

    void operator()(const events::QuitEvent &e) override {
        m_running = false;
    }

    void operator()(const events::KeyboardEvent &e) override {
        if (e.key == events::KeyboardEvent::Key::KEY_ESCAPE) {
            m_running = false;
        }
    }

private:
    bool m_running;
    std::chrono::high_resolution_clock::time_point m_startTime;
    rxcpp::subjects::subject<std::chrono::milliseconds> m_elapsedMs;

    std::unique_ptr<graphics::Renderer> m_renderer;
    std::unique_ptr<events::Events> m_events;
    events::Controls m_controls;

    std::unordered_map<
            entities::Entity::id_type,
            std::unique_ptr<entities::Entity>,
            entities::Entity::id_hash>
        m_entities;
};

} // end namespace rpec

#endif // RPEC__GAME__H_

