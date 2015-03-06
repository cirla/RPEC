/**
 *
 */

#ifndef RPEC__EVENTS__SDL_EVENTS__H_
#define RPEC__EVENTS__SDL_EVENTS__H_

#include <SDL2/SDL.h>

#include "events.hpp"

namespace rpec {
namespace events {

class SdlEvents : public Events {
public:
    SdlEvents(const rxcpp::observable<std::chrono::milliseconds> &elapsedMs)
    : Events(elapsedMs) {
        if (0 != SDL_InitSubSystem(SDL_INIT_EVENTS)) {
            throw SDL_GetError();
        }
    }

    ~SdlEvents() {
        SDL_QuitSubSystem(SDL_INIT_EVENTS);
    }

    void poll() override;
};

} // end namespace events
} // end namespace rpec

#endif // RPEC__EVENTS__H_

