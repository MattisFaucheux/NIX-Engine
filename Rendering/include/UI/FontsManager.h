#pragma once
#include <string>
#include <vector>


#include "ImGUI/imgui.h"

namespace NIX::Rendering
{
	
	class CommandPool;

	class FontsManager
	{
	private:

		CommandPool* m_commandPool;

		std::vector<std::pair<std::string, int>> m_fontsToLoad;
		std::vector<std::pair<std::pair<std::string, int>, ImFont*>> m_fonts;
		
	public:
		FontsManager() = delete;
		FontsManager(CommandPool& commandPool);

		~FontsManager();

		void LoadDefaultFont(ImGuiIO& io);
		
		void AddFontToLoad(const std::pair<std::string, int>& font);

		void UpdateFontToLoad();

		[[nodiscard]] const std::vector<std::pair<std::pair<std::string, int>, ImFont*>>& GetFonts() const;

	};
}
