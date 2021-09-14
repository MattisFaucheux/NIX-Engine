#pragma once
#include <vector>

#include "Components/HUD/HUDComponent.h"

namespace NIX::Editor
{
	class HUDDesigner
	{
		inline static std::vector<HUDComponent*> m_components;

		inline static bool m_isOpen;

		static void Sort(HUDComponent* compChange);

	public:
        template <typename CompType>
        static void Add(CompType* comp)
        {
			Sort(comp);
        }

		static void SetZOrder(int index);

		static void Remove(int nb);

		static void UpdateHUDDesigner(std::string_view label);

		static void IsOpen(bool isOpen);
		static [[nodiscard]] bool& IsOpen();
	};

}
