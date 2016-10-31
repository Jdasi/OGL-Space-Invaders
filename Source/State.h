#pragma once
#include <vector>

#include <Engine/Sprite.h>
#include <Engine/Renderer.h>

#include "Commands.h"
#include "TextObject.h"

class InvadersGame;

class StateHandler;

class State
{
friend class StateHandler;

public:
    State(InvadersGame &game)
        : m_game(game)
        , m_handler(nullptr)
    {
    }

    virtual ~State() = default;
    State(const State&) = delete;
    State& operator=(const State&) = delete;

    virtual void onStateEnter() = 0;
    virtual void onStateLeave() = 0;
    virtual void tick() = 0;

    virtual void onCommand(const Command c, const CommandState s) = 0;

protected:
    StateHandler* getHandler() const
    {
        return m_handler;
    }

    InvadersGame& m_game;

    std::vector<std::unique_ptr<ASGE::Sprite>> sprites;
    std::vector<TextObject> textObjects;

private:
    void setHandler(StateHandler* handler)
    {
        m_handler = handler;
    }

    virtual void draw()
    {
    /*
        for (auto& spr : sprites)
        {
            spr->render(game.getRenderer());
        }

        for (auto& txt : textObjects)
        {
            game.getRenderer()->renderText(txt.getString().c_str(), txt.getX(), txt.getY(),
                                   txt.getScale(), txt.getColour());
        }
        */
    }

    StateHandler* m_handler;
};
