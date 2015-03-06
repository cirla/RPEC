/**
 *
 */

#ifndef RPEC__EVENTS__CONTROLS__H_
#define RPEC__EVENTS__CONTROLS__H_

#include <unordered_map>

#include <boost/serialization/strong_typedef.hpp>
#include <boost/variant.hpp>
#include <rxcpp/rx.hpp>

#include "events.hpp"

namespace rpec {
namespace events {

BOOST_STRONG_TYPEDEF(boost::blank, MoveForward);
BOOST_STRONG_TYPEDEF(boost::blank, MoveBackward);
BOOST_STRONG_TYPEDEF(boost::blank, MoveLeft);
BOOST_STRONG_TYPEDEF(boost::blank, MoveRight);
BOOST_STRONG_TYPEDEF(MouseEvent::Position, MoveToward);

typedef boost::variant<
        MoveForward,
        MoveBackward,
        MoveLeft,
        MoveRight,
        MoveToward>
    Control;

class ControlHandler : public boost::static_visitor<void> {
public:
    virtual ~ControlHandler() { }
    inline void handle(const Control &c) {
        boost::apply_visitor(*this, c);
    }

    // override these handlers
    virtual void operator()(const MoveForward &c) { }
    virtual void operator()(const MoveBackward &c) { }
    virtual void operator()(const MoveLeft &c) { }
    virtual void operator()(const MoveRight &c) { }
    virtual void operator()(const MoveToward &c) { }
};

class Controls : public events::EventHandler {
public:
    Controls(events::Events *events)
    : events::EventHandler()
    , m_controls()
    , m_keyDownMap({
            { KeyboardEvent::Key::KEY_UP, MoveForward() },
            { KeyboardEvent::Key::KEY_DOWN, MoveBackward() },
            { KeyboardEvent::Key::KEY_LEFT, MoveLeft() },
            { KeyboardEvent::Key::KEY_RIGHT, MoveRight() },
    })
    {
        events->events().subscribe([this](auto e) {
            handle(e);
        });
    }

    ~Controls()
    { }

    auto controls() {
        static auto ret = m_controls.get_observable();
        return ret;
    }

    void operator()(const events::KeyboardEvent &e) override {
        switch (e.type) {
            case KeyboardEvent::Type::KEY_DOWN: {
                auto result = m_keyDownMap.find(e.key);
                if (result != m_keyDownMap.end()) {
                    m_controls.get_subscriber().on_next(result->second);
                }
            } break;
            case KeyboardEvent::Type::KEY_UP: {
                // TODO: Handle key up
            } break;
        }
    }

    void operator()(const events::MouseEvent &e) override {
        switch (e.type) {
            case MouseEvent::Type::BUTTON_DOWN: {
                if (MouseEvent::Button::BUTTON_LEFT == e.button) {
                    m_controls.get_subscriber().on_next(std::move(MoveToward(e.position)));
                }
            } break;
            case MouseEvent::Type::BUTTON_UP: {
                // TODO: Handle button up
            } break;
        }
    }

private:
    rxcpp::subjects::subject<Control> m_controls;
    std::unordered_map<KeyboardEvent::Key, Control> m_keyDownMap;
};

} // end namespace events
} // end namespace rpec

#endif // RPEC__EVENTS__CONTROLS__H_

