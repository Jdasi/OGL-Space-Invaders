#pragma once
#include <string>
#include <array>

#include <Engine/Colours.h>

#include "Renderable.h"

class TextObject : public Renderable
{
public:
    TextObject(const std::shared_ptr<ASGE::Renderer>& renderer);
    TextObject(const std::shared_ptr<ASGE::Renderer>& renderer, const std::string& s, 
               const Position position, const float size, const float colour[3]);
    virtual ~TextObject() = default;

    void setString(const std::string &s);
    std::string getString() const;

    void setScale(float f);
    float getScale() const;

    void setColour(const float colour[3]);
    ASGE::Colour getColour() const;

    void setPosition(const Position position) override;
    Position getPosition() const override;

private:
    void render() override;

    Position m_position;
    std::string m_str;
    std::array<int, 2> m_pos;
    float m_scale;
    ASGE::Colour m_rgb;
};
