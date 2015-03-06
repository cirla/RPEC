/**
 *
 */

#ifndef RPEC__EVENTS__EVENTS__H_
#define RPEC__EVENTS__EVENTS__H_

#include <chrono>

#include <boost/variant.hpp>
#include <rxcpp/rx.hpp>

#include "../graphics/renderer.hpp"

namespace rpec {
namespace events {

struct QuitEvent { };

struct KeyboardEvent {
    enum class Type {
        KEY_DOWN,
        KEY_UP
    };

    enum class Key {
        UNKNOWN,
        KEY_0, KEY_1, KEY_2, KEY_3, KEY_4,
        KEY_5, KEY_6, KEY_7, KEY_8, KEY_9,
        KEY_A, KEY_B, KEY_C, KEY_D, KEY_E,
        KEY_F, KEY_G, KEY_H, KEY_I, KEY_J,
        KEY_K, KEY_L, KEY_M, KEY_N, KEY_O,
        KEY_P, KEY_Q, KEY_R, KEY_S, KEY_T,
        KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z,
        KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT,
        KEY_ESCAPE
    };

    KeyboardEvent(Type type,
                  Key key)
    : type(type)
    , key(key)
    { }

    Type type;
    Key key;
};

struct MouseEvent {
    enum class Type {
        BUTTON_DOWN,
        BUTTON_UP,
    };

    enum class Button {
        BUTTON_LEFT,
        BUTTON_MIDDLE,
        BUTTON_RIGHT,
        BUTTON_X1,
        BUTTON_X2,
    };

    typedef std::pair<int32_t, int32_t> Position;

    MouseEvent(Type type,
               Button button,
               int32_t x,
               int32_t y)
    : type(type)
    , button(button)
    , position(std::make_pair(x, y))
    { }

    Type type;
    Button button;
    Position position;
};

struct RenderEvent {
    RenderEvent(graphics::Renderer *r)
    : renderer(r)
    { }

    graphics::Renderer *renderer;
};

typedef boost::variant<boost::blank,
        QuitEvent,
        KeyboardEvent,
        MouseEvent,
        RenderEvent>
    Event;

class EventHandler : public boost::static_visitor<void> {
public:
    virtual ~EventHandler() { }
    inline void handle(const Event &e) {
        boost::apply_visitor(*this, e);
    }
    inline void operator()(const boost::blank &e) { }

    // override these handlers
    virtual void operator()(const QuitEvent &e) { }
    virtual void operator()(const KeyboardEvent &e) { }
    virtual void operator()(const MouseEvent &e) { }
    virtual void operator()(const RenderEvent &e) { }
};

class Events {
public:
    Events(const rxcpp::observable<std::chrono::milliseconds> &elapsedMs)
    : m_events()
    , m_keyboardEvents()
    , m_mouseEvents()
    , m_elapsedMs() {
        elapsedMs.subscribe([this](auto elapsedMs) {
            m_elapsedMs.get_subscriber().on_next(elapsedMs);
        });

        auto subscribe = [this](auto e) {
            m_events.get_subscriber().on_next(e);
        };

        quitEvents().subscribe(subscribe);
        keyboardEvents().subscribe(subscribe);
        mouseEvents().subscribe(subscribe);
    }

    virtual ~Events() { }

    virtual void poll() = 0;

    void render(graphics::Renderer *renderer) {
        renderer->clear();
        m_events.get_subscriber().on_next(std::move(RenderEvent(renderer)));
        renderer->present();
    }

    auto events() {
        static auto ret = m_events.get_observable();
        return ret;
    }

    rxcpp::observable<const QuitEvent> & quitEvents() {
        static auto ret = m_quitEvents.get_observable();
        return ret;
    }

    rxcpp::observable<const KeyboardEvent> & keyboardEvents() {
        static auto ret = m_keyboardEvents.get_observable();
        return ret;
    }

    rxcpp::observable<const MouseEvent> & mouseEvents() {
        static auto ret = m_mouseEvents.get_observable();
        return ret;
    }

    auto totalElapsedMs() {
        static auto ret = m_elapsedMs.get_observable();
        return ret;
    }

    auto elapsedMs() {
        static auto ret = totalElapsedMs().pairwise().map([](auto e) {
            return std::get<1>(e) - std::get<0>(e);
        });
        return ret;
    }

protected:
    rxcpp::subjects::subject<const Event> m_events;
    rxcpp::subjects::subject<const QuitEvent> m_quitEvents;
    rxcpp::subjects::subject<const KeyboardEvent> m_keyboardEvents;
    rxcpp::subjects::subject<const MouseEvent> m_mouseEvents;

private:
    rxcpp::subjects::subject<std::chrono::milliseconds> m_elapsedMs;
};

} // end namespace events
} // end namespace rpec

#endif // RPEC__EVENTS__EVENTS__H_

