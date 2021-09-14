#include "Components/Model.h"

#include "EditorApp.h"
#include "Components/CollisionMesh.h"
#include "Context/Context.h"
#include "MeshImport/Mesh.h"
#include "GameObject.h"

#include "ResourceManager/MeshManager.h"
#include "ResourceManager/MaterialManager.h"

#include "Scripting/ScriptManager.h"


namespace NIX::Editor
{
	class CollisionMesh;

	Model::Model(GameObject* gameObject)
		: Component(gameObject), m_mesh(nullptr), m_material(nullptr)
	{}

	Model::Model(GameObject* gameObject, const char* meshPath, const char* materialPath)
		: Component(gameObject), m_mesh(MainResourceManager::GetMeshManager()->Find(meshPath)), m_material(MainResourceManager::GetMaterialManager()->Find(materialPath))
	{
		if (m_mesh == nullptr)
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_WARNING, "Failed to find mesh : %s", meshPath);
		}

		if (m_material == nullptr)
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_WARNING, "Failed to find material : %s", materialPath);
		}
		
		m_drawableObject = EditorApp::GetRenderingContext().AddDrawableObject(m_mesh, m_material);
	}

	Model::Model(GameObject* gameObject, const Model& other)
		: Component(gameObject)
	{
		m_mesh = other.m_mesh;
		m_material = other.m_material;
		m_drawableObject = EditorApp::GetRenderingContext().AddDrawableObject(m_mesh, m_material);

		SetIsActive(gameObject->IsActive());
	}

	void Model::Serialize(Core::SaveElement& parent)
	{
		Core::SaveElement modelElem(parent, "model");

		modelElem.AddStringAttribute("mesh_path", m_mesh->m_path.c_str());
		modelElem.AddStringAttribute("material_path", m_material->path.c_str());
	}

	void Model::Deserialize(const Core::SaveElement& data)
	{
		const char* meshPath = data.GetStringAttribute("mesh_path");
		const char* materialPath = data.GetStringAttribute("material_path");

		m_mesh = MainResourceManager::GetMeshManager()->Find(meshPath);
		if (m_mesh == nullptr)
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_ERROR, "Failed to load a mesh when deserializing a model component");
			return;
		}

		m_material = MainResourceManager::GetMaterialManager()->Find(materialPath);
		if (m_material == nullptr)
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_ERROR, "Failed to load a material when deserializing a model component");
			return;
		}
		
		if (m_drawableObject == nullptr)
		{
			m_drawableObject = EditorApp::GetRenderingContext().AddDrawableObject(m_mesh, m_material);
		}
		else
		{
			UpdateDrawableObject();
		}

		m_mesh->m_path = meshPath;
		m_material->path = materialPath;
	}

	void Model::SetIsActive(bool value)
	{
		m_isActive = value;
		if (m_drawableObject != nullptr)
		{
			m_drawableObject->SetIsActive(m_isActive);
		}
	}

	Model::~Model()
	{
		EditorApp::GetRenderingContext().DestroyDrawableObject(m_drawableObject);

		CollisionMesh* collisionMesh = m_gameObject->GetComponent<CollisionMesh>();
		if (collisionMesh != nullptr)
		{
			m_gameObject->RemoveComponent(collisionMesh);
		}
	}

	void Model::UpdateMesh(const char* meshPath)
	{
		if (meshPath == nullptr)
		{
			return;
		}

		m_mesh = MainResourceManager::GetMeshManager()->Find(meshPath);

		if (m_mesh == nullptr)
		{
			m_mesh = MainResourceManager::GetMeshManager()->Find(DEFAULT_MESH);
			m_mesh->m_path = DEFAULT_MESH;
		}
		else
		{
			m_mesh->m_path = meshPath;
		}

		if (m_drawableObject == nullptr)
		{
			return;
		}
		
		m_drawableObject->UpdateRenderingMesh(m_mesh);

		if (m_gameObject != nullptr)
		{
			auto* comp = m_gameObject->GetComponent<CollisionMesh>();

			if (comp != nullptr)
			{
				comp->GetCollisionMesh()->SetGeometry(m_drawableObject->GetRenderingMesh()->points, m_gameObject->GetTransform()->GetWorldScale());
			}
		}
	}

	void Model::UpdateMaterial(const char* materialPath)
	{
		if (m_material == nullptr || std::strcmp(m_material->path.c_str(), materialPath) == 0)
		{
			return;
		}

		
		m_material = MainResourceManager::GetMaterialManager()->Find(materialPath);

		if (m_material == nullptr)
		{
			m_material = MainResourceManager::GetMaterialManager()->Find(FAILED_MATERIAL);
			m_material->path = FAILED_MATERIAL;
			
		}
		else
		{
			m_material->path = materialPath;
		}
		
		m_drawableObject->UpdateMaterial(m_material);
		
	}

	void Model::UpdateDrawableObject()
	{
		if (m_drawableObject == nullptr)
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_WARNING, "Failed to update drawable object, drawable object is nullptr");
		}
		
		m_drawableObject->UpdateRenderingMesh(m_mesh);
		m_drawableObject->UpdateMaterial(m_material);

		if (m_gameObject != nullptr)
		{
			auto* comp = m_gameObject->GetComponent<CollisionMesh>();

			if (comp != nullptr)
			{
				comp->GetCollisionMesh()->SetGeometry(m_drawableObject->GetRenderingMesh()->points, m_gameObject->GetTransform()->GetWorldScale());
			}
		}
	}

	void Model::SetMeshPath(std::string_view meshPath)
	{
		m_mesh->m_path = meshPath;
	}

	void Model::SetMaterialPath(std::string_view materialPath)
	{
		m_material->path = materialPath;
	}

	void Model::UpdateDrawableTransform(const LibMath::Matrix4& transform)
	{
		if (m_drawableObject == nullptr)
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_ERROR, "Failed to update drawable transform, drawable object is null");
		}
		
		m_drawableObject->UpdateTransformation(transform);
	}

	const GeometricTools::Mesh* Model::GetMesh() const
	{
		return m_mesh;
	}

	const Rendering::Material* Model::GetMaterial() const
	{
		return m_material;
	}

	std::string_view Model::GetMeshPath() const
	{
		return m_mesh->m_path;
	}

	std::string_view Model::GetMaterialPath() const
	{
		return m_material->path;
	}

	const Rendering::DrawableObject* Model::GetDrawableObject() const
	{
		return m_drawableObject;
	}

	void Model::LuaInit()
	{
		ScriptManager::GetSingleton()->AddClass<Model, GameObject*>("Model");

		ScriptManager::GetSingleton()->AddClassProperty<Model>("Model", "IsActive", &Model::GetIsActive, &Model::SetIsActive);
		ScriptManager::GetSingleton()->AddClassData<Model>("Model", "GameObject", &Model::m_gameObject, false);
		ScriptManager::GetSingleton()->AddClassFunction<Model>("Model", "GetGameObject", &Model::GetParent);

		ScriptManager::GetSingleton()->AddClassProperty<Model>("Model", "MeshPath", &Model::GetLuaMeshPath, &Model::SetLuaMeshPath);
		ScriptManager::GetSingleton()->AddClassProperty<Model>("Model", "MaterialPath", &Model::GetLuaMaterialPath, &Model::SetLuaMaterialPath);
	}

	std::string Model::GetLuaMeshPath() const
	{
		return std::string(GetMeshPath());
	}

	std::string Model::GetLuaMaterialPath() const
	{
		return std::string(GetMaterialPath());
	}

	void Model::SetLuaMeshPath(std::string meshPath)
	{
		UpdateMesh(meshPath.c_str());
	}

	void Model::SetLuaMaterialPath(std::string materialPath)
	{
		UpdateMaterial(materialPath.c_str());
	}
}
