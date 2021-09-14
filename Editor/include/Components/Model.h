#pragma once

#include "Component.h"
#include "Matrix/Matrix4.h"

#include <string>

#define DEFAULT_MATERIAL "Resources\\Materials\\defaultMaterial.mat"
#define FAILED_MATERIAL "Resources\\Materials\\failedMaterial.mat"

#define DEFAULT_MESH "Resources\\Meshes\\Cube.obj"

namespace NIX {
	namespace GeometricTools {
		class Mesh;
	}
}

namespace NIX {
	namespace Rendering {
		class DrawableObject;
		class Context;
		struct Material;
	}
}

namespace NIX::Editor
{
	class GameObject;

	class Model : public Component
	{
	private:
		GeometricTools::Mesh* m_mesh = nullptr;
		Rendering::Material* m_material = nullptr;
		Rendering::DrawableObject* m_drawableObject = nullptr;
		
	public:
		Model(GameObject* gameObject);
		Model(GameObject* gameObject, const char* meshPath, const char* materialPath = "Default Material");
		Model(GameObject* gameObject, const Model& other);
		~Model() override;

		void Serialize(Core::SaveElement& parent) override;
		void Deserialize(const Core::SaveElement& data) override;

		void SetIsActive(bool value) override;

		void UpdateMesh(const char* meshPath);
		void UpdateMaterial(const char* material);
		void UpdateDrawableObject();

		void SetMeshPath(std::string_view meshPath);
		void SetMaterialPath(std::string_view materialPath);
		
		void UpdateDrawableTransform(const LibMath::Matrix4& transform);

		[[nodiscard]] const GeometricTools::Mesh* GetMesh() const;
		[[nodiscard]] const Rendering::Material* GetMaterial() const;

		[[nodiscard]] std::string_view GetMeshPath() const;
		[[nodiscard]] std::string_view GetMaterialPath() const;

		[[nodiscard]] const Rendering::DrawableObject* GetDrawableObject() const;

		static void LuaInit();

	private:

		[[nodiscard]] std::string GetLuaMeshPath() const;
		[[nodiscard]] std::string GetLuaMaterialPath() const;
		void SetLuaMeshPath(std::string meshPath);
		void SetLuaMaterialPath(std::string materialPath);


	};
	
}
