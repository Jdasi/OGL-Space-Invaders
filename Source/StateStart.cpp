#include "StateStart.h"

#include <iostream>

StateStart::StateStart(std::shared_ptr<ASGE::Renderer>& r)
    : State(r)
{
}

StateStart::~StateStart()
{
}

void StateStart::onStateEnter()
{
    // Load space invader sprite.
    sprites.push_back(m_renderer->createSprite());
    sprites[0]->position[0] = 700;
    sprites[0]->position[1] = 250;

    if (!sprites[0]->loadTexture("..\\..\\Resources\\Textures\\Invader.jpg"))
    {
        throw std::runtime_error("Error in StateStart::onStateEnter()");
    }

    TextObject obj("Test", 375, 325, 1.0f, ASGE::COLOURS::DARKORANGE);
    textObjects.push_back(obj);
}

void StateStart::onStateLeave()
{
}

void StateStart::tick()
{
}

void StateStart::onCommand(const Command c, const CommandState s)
{
    std::cout << "command: " << static_cast<int>(c) << " " << static_cast<int>(s) << std::endl;
}
