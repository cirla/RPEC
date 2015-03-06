/**
 *
 */

#ifndef RPEC__GRAPHICS__SDL_OPENGL_RENDERER__H_
#define RPEC__GRAPHICS__SDL_OPENGL_RENDERER__H_

#include <SDL2/SDL.h>

#include "openglrenderer.hpp"

namespace rpec {
namespace graphics {

class SdlOpenGlContext {
public:
    SdlOpenGlContext()
    : m_window(nullptr)
    , m_context(nullptr) {
        SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "0");

        if (0 != SDL_InitSubSystem(SDL_INIT_VIDEO)) {
            throw SDL_GetError();
        }

        // TODO: OS independent
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

        m_window = SDL_CreateWindow(
                "rpec",
                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                640, 480,
                SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_SHOWN);
        if (nullptr == m_window) {
            throw SDL_GetError();
        }

        m_context = SDL_GL_CreateContext(m_window);
        if (nullptr == m_context) {
            throw SDL_GetError();
        }
    }

    ~SdlOpenGlContext() {
        SDL_GL_DeleteContext(m_context);
        SDL_DestroyWindow(m_window);
        SDL_QuitSubSystem(SDL_INIT_VIDEO);
    }

    inline void swap() {
        SDL_GL_SwapWindow(m_window);
    }

private:
    SDL_Window *m_window;
    SDL_GLContext m_context;
};

typedef OpenGlRenderer<SdlOpenGlContext> SdlOpenGlRenderer;

} // end namespace graphics
} // end namespace rpec

#endif // RPEC__GRAPHICS__SDL_OPENGL_RENDERER__H_

