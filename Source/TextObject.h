#pragma once
#include <string>
#include <array>

#include <Engine/Colours.h>

#include "Renderable.h"

class TextObject : public Renderable
{
public:
    TextObject(const std::shared_ptr<ASGE::Renderer>& renderer);
    TextObject(const std::shared_ptr<ASGE::Renderer>& renderer, const std::string& str, 
               const Vector2 position, const float scale, const float colour[3]);
    virtual ~TextObject() = default;

    std::string getString() const;
    void setString(const std::string& str);

    Vector2 getPosition() const override;
    void setPosition(const Vector2 position) override;
    void modifyPosition(int x, int y) override;

    float getScale() const;
    void setScale(float f);

    ASGE::Colour getColour() const;
    void setColour(const float colour[3]);

private:
    void render() override;

    Vector2 m_position;
    std::string m_str;
    float m_scale;
    ASGE::Colour m_rgb;
};
