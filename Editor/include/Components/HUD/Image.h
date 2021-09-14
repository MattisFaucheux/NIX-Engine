#pragma once
#include "Components/HUD/HUDComponent.h"

namespace NIX::Editor
{
	class Image : public HUDComponent
	{
	public:
		Image(GameObject* gameObject, std::string_view path = "Default Texture", float width = 100.f, float height = 100.f);
		Image(GameObject* gameObject, const Image& other);

		void SetImage(std::string_view path);
		[[nodiscard]] std::string_view GetImage() const;

		void SetSize(float width, float height);
		[[nodiscard]] std::pair<float, float> GetSize();

		void SetWidth(float width);
		[[nodiscard]] float GetWidth() const;

		void SetHeight(float height);
		[[nodiscard]] float GetHeight() const;

		void Serialize(Core::SaveElement& parent) override;
		void Deserialize(const  Core::SaveElement& data) override;

		static void LuaInit();

	private:
		std::string m_path = "Default Texture";

		float m_width = 100.f;
		float m_height = 100.f;
	};
}