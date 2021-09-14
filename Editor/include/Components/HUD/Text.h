#pragma once
#include "Components/HUD/HUDComponent.h"

#include "Vector/Vector4.h"

namespace NIX::Editor
{
	class Text : public HUDComponent
	{
	public:
		Text(GameObject* gameObject, std::string_view text = "Text", LibMath::Vector4 color = { 1.f, 1.f, 1.f, 1.f });
		Text(GameObject* gameObject, const Text& other);

		void SetText(std::string_view text);
		[[nodiscard]] std::string_view GetText() const;

		void SetFont(std::string_view path);
		[[nodiscard]] std::string_view GetFont() const;

		void SetSize(int size);
		[[nodiscard]] int GetSize() const;

		void SetColor(LibMath::Vector4 color);
		[[nodiscard]] LibMath::Vector4 GetColor() const;

		void Serialize(Core::SaveElement& parent) override;
		void Deserialize(const  Core::SaveElement& data) override;

		//Internal Functions
		void SetLast(std::string lastFont, int lastSize);
		[[nodiscard]] std::pair<std::string, int> GetLast();

		static void LuaInit();

		void LuaSetText(std::string text);
		std::string LuaGetText() const;
		void LuaSetFont(std::string font);
		std::string LuaGetFont() const;

	private:


		std::string m_text = "";
		std::string m_font = "Default Font";
		int m_size = 16;
		LibMath::Vector4 m_color = { 1.f, 1.f, 1.f, 1.f };

		std::string m_lastFont = "";
		int m_lastSize = 0;
	};
}