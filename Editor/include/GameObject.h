#pragma once

#include <vector>
#include <string>


#include "CollisionShape.h"
#include "Transform.h"
#include "Components/CollisionShapeComponent.h"
#include "Components/Model.h"
#include "Log/Logger.h"

namespace NIX {
	namespace GeometricTools {
		class Mesh;
	}
}

namespace NIX {
	namespace Rendering {
		class Context;
		struct Material;
		class DrawableObject;
	}
}

namespace NIX::Editor {
    class CollisionBox;
    class CollisionCapsule;
    class CollisionSphere;
    class CollisionMesh;
    class RigidBody;
    class RigidStatic;
    class Light;
    class Image;
    class Text;
    class Camera;
    class SoundEmitter;
}

namespace NIX::Editor
{
	class GameObject
    {
    public:
        ~GameObject();

        GameObject(std::string_view name = "GameObject");
		
        GameObject(GameObject* parent, const GameObject& other);
        GameObject(GameObject&& other) noexcept = default;
        GameObject& operator=(GameObject&& other) noexcept = default;

        void Serialize(Core::SaveElement& parent);
        void Deserialize(const Core::SaveElement& data);
		
        [[nodiscard]] std::string& GetName();
        void SetName(std::string_view name);

        [[nodiscard]] std::string& GetTag();
        void SetTag(std::string_view tag);

        [[nodiscard]] bool& IsActive();
        void IsActive(bool isActive, bool isExitingSimulation = false);
        void IsActiveInEditor();
    	
        [[nodiscard]] std::vector<Component*>& GetComponents();

        template<typename T>
        void AddComponent(T* comp)
        {
            m_components.emplace_back(comp);
        }
		
        void RemoveComponent(int pos);
        void RemoveComponent(Component* comp);

        [[nodiscard]] Transform* GetTransform();

        template <typename CompType>
        CompType* GetComponent()
        {
            CompType* result = nullptr;
            for (Component* currComp : m_components)
            {
                result = dynamic_cast<CompType*>(currComp);
                if (result)
                    break;
            }
            return result;
        }

        static const std::vector<GameObject*>& GetObjectsWithTag(std::string_view tag);

        bool operator==(const GameObject& rhs) const;

        void SetParent(GameObject* parent);
        GameObject* GetParent();

        std::vector<GameObject*>& GetChildren();

        void RemoveChild(int pos);

        void Update();

        void SwitchToSimulation();
        void ExitSimulation();
		
        void SimulationStart();
        void SimulationUpdate(float deltaTime);

        static void LuaInit();

        void OnTrigger(Physic::CollisionShape* other, bool triggerEnter);
        void OnContact(Physic::CollisionShape* other, bool contactBegin);

        GameObject* GetCollisionShapeGameObject(Physic::CollisionShape* cs);
		
    private:

        void PhysicUpdate();

        void DeserializeNextComp(const char* compName, const Core::SaveElement& data);

        void DeserializeCollisionMesh(const Core::SaveElement& data);

        template<typename T>
        void Deserialize(const Core::SaveElement& data, bool isUnique)
        {
            Component* comp = GetComponent<T>();
        	
            if (isUnique && comp != nullptr)
            {
                comp->Deserialize(data);
                return;
            }
        	
            comp = new T(this);

            comp->Deserialize(data);

            AddComponent(comp);
        }

        Model* AddComponentModel();
        CollisionBox* AddComponentCollisionBox();
        CollisionCapsule* AddComponentCollisionCapsule();
        CollisionSphere* AddComponentCollisionSphere();
        CollisionMesh* AddComponentCollisionMesh();
        RigidBody* AddComponentRigidBody();
        RigidStatic* AddComponentRigidStatic();
        Light* AddComponentLight();
        Image* AddComponentImage();
        Text* AddComponentText();
        Camera* AddComponentCamera();
        SoundEmitter* AddComponentSoundEmitter();

        std::string m_name;
        std::string m_tag = "Untagged";

        bool m_isActive = true;
        bool m_isActiveInEditor = true;

        std::vector<Component*> m_components;
        GameObject* m_parent = nullptr;
        std::vector<GameObject*> m_children;
    };
}
