#pragma once
#include <utility>

namespace NIX::Editor
{
	class SceneWindow
	{

	public:
		std::pair<float, float> UpdateSceneWindow(void*& textureID);
	};
}
