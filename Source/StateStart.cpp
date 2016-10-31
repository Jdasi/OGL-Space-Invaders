#include <iostream>

#include "StateStart.h"
#include "Game.h"

StateStart::StateStart(ObjectFactory& factory)
    : State(factory)
{
}



StateStart::~StateStart()
{
}



void StateStart::onStateEnter()
{
    m_invader = getObjectFactory().createSprite("..\\..\\Resources\\Textures\\Invader.jpg", { 700, 250 });
    m_text = getObjectFactory().createText("Test", { 375, 325 }, 1.0f, ASGE::COLOURS::DARKORANGE);
}



void StateStart::onStateLeave()
{
}



void StateStart::tick()
{
    static int blah = 0;
    if (++blah > 500)
    {
        m_invader = nullptr;
    }
}



void StateStart::onCommand(const Command c, const CommandState s)
{
    std::cout << "command: " << static_cast<int>(c) << " " << static_cast<int>(s) << std::endl;
}


