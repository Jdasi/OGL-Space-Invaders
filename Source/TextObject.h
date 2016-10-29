#pragma once
#include <string>
#include <array>

#include <Engine/Colours.h>

class TextObject
{
public:
    TextObject();
    TextObject(const std::string& s, int x, int y, float sz, const float colour[3]);
    ~TextObject() = default;

    void setString(const std::string &s);
    std::string getString() const;

    void setX(int x);
    int getX() const;

    void setY(int y);
    int getY() const;

    void setScale(float f);
    float getScale() const;

    void setColour(const float colour[3]);
    ASGE::Colour getColour() const;

private:
    std::string str;
    std::array<int, 2> pos;
    float scale;
    ASGE::Colour rgb;
};
