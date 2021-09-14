#pragma once
#include <vector>
#include <Vector/Vector3.h>


namespace NIX {
	namespace GeometricTools {
		class Mesh;
	}
}

namespace NIX::Rendering
{
	struct Material;
	class GraphicsPipeline;
	class CommandPool;
	class DrawableObject;
	
	class DrawableObjectsManager
	{
	private:
		std::vector<DrawableObject*> m_drawableObjects;

		CommandPool* m_commandPool = nullptr;
		
	public:
		DrawableObjectsManager() = delete;
		explicit DrawableObjectsManager(CommandPool& commandPool);
		~DrawableObjectsManager();
		
		[[nodiscard]] DrawableObject* AddDrawableObject(const GeometricTools::Mesh& mesh, Material* material, GraphicsPipeline* graphicsPipeline);
		[[nodiscard]] DrawableObject* AddDrawableObject(const std::vector<LibMath::Vector3>& points, GraphicsPipeline* graphicsPipeline);
		
		void DestroyDrawableObject(DrawableObject* drawableObject);

		[[nodiscard]] std::vector<DrawableObject*>& GetDrawableObjects();
	};
}
