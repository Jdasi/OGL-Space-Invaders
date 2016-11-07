#pragma once
#include <string>

#include <Engine/Colours.h>

#include "Renderable.h"

class TextObject : public Renderable
{
public:
    TextObject(const std::shared_ptr<ASGE::Renderer>& _renderer);
    TextObject(const std::shared_ptr<ASGE::Renderer>& _renderer, const std::string& _str, 
               const Vector2 _pos, const float _scale, const float _colour[3]);
    virtual ~TextObject() = default;

    std::string getString() const;
    void setString(const std::string& _str);

    Vector2 getPosition() const override;
    void setPosition(const Vector2 _pos) override;
    void modifyPosition(int _x, int _y) override;

    float getScale() const;
    void setScale(float _scale);

    ASGE::Colour getColour() const;
    void setColour(const float _colour[3]);

private:
    void render() override;

    Vector2 position;
    std::string str;
    float scale;
    ASGE::Colour rgb;
};
