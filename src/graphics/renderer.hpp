/**
 *
 */

#ifndef RPEC__GRAPHICS__RENDERER__H_
#define RPEC__GRAPHICS__RENDERER__H_

namespace rpec {
namespace graphics {

class Renderer {
public:
    Renderer() { }
    virtual ~Renderer() { }

    virtual void clear() = 0;
    virtual void present() = 0;
};

} // end namespace graphics
} // end namespace rpec

#endif // RPEC__GRAPHICS__RENDERER__H_

