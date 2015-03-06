/**
 *
 */

#include <cstdint>
#include <unordered_map>

#include "events.hpp"
#include "sdlevents.hpp"

namespace rpec {
namespace events {

namespace {

std::unordered_map<uint32_t, KeyboardEvent::Type>
KEYBOARD_TYPE_MAP({
        { SDL_KEYDOWN, KeyboardEvent::Type::KEY_DOWN },
        { SDL_KEYUP, KeyboardEvent::Type::KEY_UP }
});

std::unordered_map<SDL_Keycode, KeyboardEvent::Key>
KEYBOARD_KEY_MAP({
        { SDLK_0, KeyboardEvent::Key::KEY_0 },
        { SDLK_1, KeyboardEvent::Key::KEY_1 },
        { SDLK_2, KeyboardEvent::Key::KEY_2 },
        { SDLK_3, KeyboardEvent::Key::KEY_3 },
        { SDLK_4, KeyboardEvent::Key::KEY_4 },
        { SDLK_5, KeyboardEvent::Key::KEY_5 },
        { SDLK_6, KeyboardEvent::Key::KEY_6 },
        { SDLK_7, KeyboardEvent::Key::KEY_7 },
        { SDLK_8, KeyboardEvent::Key::KEY_8 },
        { SDLK_9, KeyboardEvent::Key::KEY_9 },
        { SDLK_a, KeyboardEvent::Key::KEY_A },
        { SDLK_b, KeyboardEvent::Key::KEY_B },
        { SDLK_c, KeyboardEvent::Key::KEY_C },
        { SDLK_d, KeyboardEvent::Key::KEY_D },
        { SDLK_e, KeyboardEvent::Key::KEY_E },
        { SDLK_f, KeyboardEvent::Key::KEY_F },
        { SDLK_g, KeyboardEvent::Key::KEY_G },
        { SDLK_h, KeyboardEvent::Key::KEY_H },
        { SDLK_i, KeyboardEvent::Key::KEY_I },
        { SDLK_j, KeyboardEvent::Key::KEY_J },
        { SDLK_k, KeyboardEvent::Key::KEY_K },
        { SDLK_l, KeyboardEvent::Key::KEY_L },
        { SDLK_m, KeyboardEvent::Key::KEY_M },
        { SDLK_n, KeyboardEvent::Key::KEY_N },
        { SDLK_o, KeyboardEvent::Key::KEY_0 },
        { SDLK_p, KeyboardEvent::Key::KEY_P },
        { SDLK_q, KeyboardEvent::Key::KEY_Q },
        { SDLK_r, KeyboardEvent::Key::KEY_R },
        { SDLK_s, KeyboardEvent::Key::KEY_S },
        { SDLK_t, KeyboardEvent::Key::KEY_T },
        { SDLK_u, KeyboardEvent::Key::KEY_U },
        { SDLK_v, KeyboardEvent::Key::KEY_V },
        { SDLK_w, KeyboardEvent::Key::KEY_W },
        { SDLK_x, KeyboardEvent::Key::KEY_X },
        { SDLK_y, KeyboardEvent::Key::KEY_Y },
        { SDLK_z, KeyboardEvent::Key::KEY_Z },
        { SDLK_ESCAPE, KeyboardEvent::Key::KEY_ESCAPE },
        { SDLK_UP, KeyboardEvent::Key::KEY_UP },
        { SDLK_DOWN, KeyboardEvent::Key::KEY_DOWN },
        { SDLK_LEFT, KeyboardEvent::Key::KEY_LEFT },
        { SDLK_RIGHT, KeyboardEvent::Key::KEY_RIGHT }
});

KeyboardEvent convert(SDL_KeyboardEvent const &event) {
    return KeyboardEvent(
            KEYBOARD_TYPE_MAP[event.type],
            KEYBOARD_KEY_MAP[event.keysym.sym]);
}

std::unordered_map<uint32_t, MouseEvent::Type>
MOUSE_BUTTON_TYPE_MAP({
        { SDL_MOUSEBUTTONDOWN, MouseEvent::Type::BUTTON_DOWN },
        { SDL_MOUSEBUTTONUP, MouseEvent::Type::BUTTON_UP }
});

std::unordered_map<uint8_t, MouseEvent::Button>
MOUSE_BUTTON_MAP({
        { SDL_BUTTON_LEFT, MouseEvent::Button::BUTTON_LEFT },
        { SDL_BUTTON_MIDDLE, MouseEvent::Button::BUTTON_MIDDLE },
        { SDL_BUTTON_RIGHT, MouseEvent::Button::BUTTON_RIGHT },
        { SDL_BUTTON_X1, MouseEvent::Button::BUTTON_X1 },
        { SDL_BUTTON_X2, MouseEvent::Button::BUTTON_X2 },
});

MouseEvent convert(SDL_MouseButtonEvent const &event) {
    return MouseEvent(
            MOUSE_BUTTON_TYPE_MAP[event.type],
            MOUSE_BUTTON_MAP[event.button],
            event.x,
            event.y);
}

} // end anonymous namespace

void SdlEvents::poll() {
    SDL_Event sdlEvent;
    while (SDL_PollEvent(&sdlEvent))
    {
        switch (sdlEvent.type) {
            case SDL_KEYDOWN:
            case SDL_KEYUP: {
                m_keyboardEvents.get_subscriber().on_next(convert(sdlEvent.key));
            } break;
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP: {
                m_mouseEvents.get_subscriber().on_next(convert(sdlEvent.button));
            } break;
            case SDL_QUIT: {
                m_quitEvents.get_subscriber().on_next(QuitEvent());
            } break;
            default: break;
        }
    }
}

} // end namespace events
} // end namespace rpec

