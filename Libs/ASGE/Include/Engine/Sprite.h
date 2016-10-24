#pragma once
#include <memory>

namespace ASGE {
	class Renderer;
	class Sprite
	{
	public:
		float position[2]{ 0,0 };
		float rotation = 0;
		float size[2]{ 0,0 };

		virtual bool loadTexture(const char*) = 0;
		virtual bool render(std::shared_ptr<Renderer>) = 0;
	};
}