#pragma once
#include <string>

#include <Engine/Colours.h>

#include "Renderable.h"

// Derived from Renderable. Convenience class to treat rendered text like an asset.
class TextObject : public Renderable
{
public:
    TextObject(const std::shared_ptr<ASGE::Renderer>& _renderer,
        std::function<void(Renderable*)> _delete_render_object, const std::string& _str,
        const Vector2 _pos, const float _scale, const float _colour[3]);
    virtual ~TextObject() = default;

    std::string getString() const;
    void setString(const std::string& _str);
    void appendString(const std::string& _str);

    float getScale() const;
    void setScale(float _scale);

    ASGE::Colour getColour() const;
    void setColour(const float _colour[3]);

private:
    void render() override;

    std::string str;
    float scale;
    ASGE::Colour rgb;
};
