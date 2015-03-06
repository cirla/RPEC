/**
 *
 */

#ifndef RPEC__ENTITIES__PLAYER__H_
#define RPEC__ENTITIES__PLAYER__H_

#include <iostream>

#include "../events/controls.hpp"
#include "entity.hpp"

namespace rpec {
namespace entities {

class Player
    : public Entity
    , public events::ControlHandler
{
public:
    Player ()
    { }

    ~Player()
    { }

    virtual void operator()(const events::MoveForward &c) override {
        std::cout << "MoveForward" << std::endl;
    }

    virtual void operator()(const events::MoveBackward &c) override {
        std::cout << "MoveBackward" << std::endl;
    }

    virtual void operator()(const events::MoveLeft &c) override {
        std::cout << "MoveLeft" << std::endl;
    }

    virtual void operator()(const events::MoveRight &c) override {
        std::cout << "MoveRight" << std::endl;
    }

    virtual void operator()(const events::MoveToward &c) override {
        std::cout << "Move Toward(" << c.t.first << ", " << c.t.second << ")" << std::endl;
    }

private:
};

} // end namespace entities
} // end namespace rpec

#endif // RPEC__ENTITIES__PLAYER__H_

