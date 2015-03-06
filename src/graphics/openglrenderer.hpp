/**
 *
 */

#ifndef RPEC__GRAPHICS__OPENGL_RENDERER__H_
#define RPEC__GRAPHICS__OPENGL_RENDERER__H_

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include <GL/glew.h>

#include "renderer.hpp"

namespace rpec {
namespace graphics {

template<class Context>
class OpenGlRenderer : public Renderer {
public:
    OpenGlRenderer()
    : m_context()
    {
        glewExperimental = GL_TRUE;
        auto glewError = glewInit();
        if (GLEW_OK != glewError) {
            throw glewGetErrorString(glewError);
        }

        std::cout << "gl_renderer:" << glGetString(GL_RENDERER) << std::endl;
        std::cout << "gl_context: " << glGetString(GL_VERSION) << std::endl;
        std::cout << "gl_shading_lang_version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    }

    ~OpenGlRenderer() { }

private:
    void clear() override {
        glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void present() override {
        m_context.swap();
    }

private:
    Context m_context;
};

} // end namespace graphics
} // end namespace rpec

#endif // RPEC__GRAPHICS__OPENGL_RENDERER__H_

