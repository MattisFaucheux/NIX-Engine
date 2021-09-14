#include "GameObject.h"

#include "SceneManager.h"

#include "DrawableObject/DrawableObject.h"

#include "Context/Context.h"
#include "Components/Model.h"
#include "Components/Lights/Light.h"
#include "Components/RigidBody.h"
#include "Components/RigidStatic.h"
#include "Components/CollisionBox.h"
#include "Components/CollisionCapsule.h"
#include "Components/CollisionSphere.h"
#include "Components/CollisionMesh.h"
#include "Components/HUD/Image.h"
#include "Components/HUD/Text.h"
#include "Components/Sound/SoundEmitter.h"
#include "Components/Scripting/Script.h"


#include <iostream>

#include "EditorApp.h"
#include "EngineState.h"
#include "Components/Camera.h"
#include "Log/Logger.h"

#include "Scripting/ScriptManager.h"
#include "ResourceManager/MeshManager.h"
#include "UI/InspectorWindow.h"
#include "UI/UIEditor.h"
#include "Light/ELightType.h"

namespace NIX::Editor
{
	GameObject::~GameObject()
	{
		if (m_parent)
		{
			m_parent->GetChildren().erase(std::find(m_parent->GetChildren().begin(), m_parent->GetChildren().end(), this));
		}

		while (!m_components.empty())
		{
			delete m_components[0];
			m_components.erase(m_components.begin());
		}

		while (!m_children.empty())
		{
			delete m_children[0];
		}

		UIEditor::GetInspectorWindow().RemoveObjFromTags(this);
	}

	GameObject::GameObject(std::string_view name) :
		m_name(name)
	{
		m_components.push_back(new Transform(this));
		UIEditor::GetInspectorWindow().AddObjectTag(this, "Untagged");
		m_tag = "Untagged";
	}

	GameObject::GameObject(GameObject* parent, const GameObject& other) :
		m_name(other.m_name), m_parent(parent)
	{
		IsActive(other.m_isActive);
		
		if(UIEditor::GetInspectorWindow().AddObjectTag(this, other.m_tag))
			m_tag = other.m_tag;

		//Duplicate Components
		for (Component* otherComp : other.m_components)
		{
			if (Transform* transform = dynamic_cast<Transform*>(otherComp))
			{
				AddComponent(new Transform(this, *transform));
			}
			else if (Model* model = dynamic_cast<Model*>(otherComp))
			{
				AddComponent(new Model(this, *model));
			}
			else if (CollisionBox* cBox = dynamic_cast<CollisionBox*>(otherComp))
			{
				AddComponent(new CollisionBox(this, *cBox));
			}
			else if (CollisionCapsule* cCapsule = dynamic_cast<CollisionCapsule*>(otherComp))
			{
				AddComponent(new CollisionCapsule(this, *cCapsule));
			}
			else if (CollisionSphere* cSphere = dynamic_cast<CollisionSphere*>(otherComp))
			{
				AddComponent(new CollisionSphere(this, *cSphere));
			}
			else if (CollisionMesh* cMesh = dynamic_cast<CollisionMesh*>(otherComp))
			{
				AddComponent(new CollisionMesh(this, *cMesh));
			}
			else if (RigidBody* rb = dynamic_cast<RigidBody*>(otherComp))
			{
				AddComponent(new RigidBody(this, *rb));
			}
			else if (RigidStatic* rs = dynamic_cast<RigidStatic*>(otherComp))
			{
				AddComponent(new RigidStatic(this, *rs));
			}
			else if (Light* lightComp = dynamic_cast<Light*>(otherComp))
			{
				AddComponent(new Light(this, *lightComp));
			}
			else if (Image* image = dynamic_cast<Image*>(otherComp))
			{
				AddComponent(new Image(this, *image));
			}
			else if (Text* text = dynamic_cast<Text*>(otherComp))
			{
				AddComponent(new Text(this, *text));
			}
			else if (Camera* camera = dynamic_cast<Camera*>(otherComp))
			{
				AddComponent(new Camera(this , *camera));
			}
			else if (SoundEmitter* soundComponent = dynamic_cast<SoundEmitter*>(otherComp))
			{
				AddComponent(new SoundEmitter(this, *soundComponent));
			}
			else if (Script* script = dynamic_cast<Script*>(otherComp))
			{
				AddComponent(new Script(this, *script));
			}
			
			//TO DO : add all Component Type
		}

		//Duplicate Children

		for (GameObject* child : other.m_children)
		{
			GameObject* childDuplicate  = new GameObject(this, *child);
			m_children.emplace_back(childDuplicate);
		}

		Update();
		
		
	}

	void GameObject::Serialize(Core::SaveElement& parent)
	{
		Core::SaveElement gameObjectElem(parent, "game_object");

		gameObjectElem.AddStringAttribute("name", m_name.c_str());
		gameObjectElem.AddStringAttribute("tag", m_tag.c_str());
		gameObjectElem.AddBoolAttribute("isActive", m_isActive);
			
		std::vector<Component*>& comps = GetComponents();

		for (auto* comp : comps)
		{
			comp->Serialize(gameObjectElem);
		}

		for (GameObject* child : m_children)
		{
			if (child == nullptr)
			{
				continue;
			}
			
			child->Serialize(gameObjectElem);
		}
	}

	void GameObject::Deserialize(const Core::SaveElement& data)
	{
		std::pair<bool, Core::SaveElement> elem = data.GetFirstChild();

		while (elem.first)
		{
			const char* elemName = elem.second.GetElementName();

			if (std::strcmp(elemName, "game_object") == 0)
			{
				auto* gameObject = new GameObject(elem.second.GetStringAttribute("name"));
				gameObject->SetParent(this);
				gameObject->SetTag(elem.second.GetStringAttribute("tag"));
				gameObject->IsActive(elem.second.GetBoolAttribute("isActive"));

				gameObject->Deserialize(elem.second);
			}
			else
			{
				DeserializeNextComp(elemName, elem.second);
			}
			

			elem = elem.second.GetNextSibling();
		}

		Update();
	}

	std::string& GameObject::GetName()
	{
		return m_name;
	}

	void GameObject::SetName(std::string_view name)
	{
		m_name = name;
	}

	std::string& GameObject::GetTag()
	{
		return m_tag;
	}

	void GameObject::SetTag(std::string_view tag)
	{
		if (UIEditor::GetInspectorWindow().AddObjectTag(this, tag))
			m_tag = tag;
	}

	bool& GameObject::IsActive()
	{
		return m_isActive;
	}

	void GameObject::IsActive(bool isActive, bool isExitingSimulation)
	{
		if (isExitingSimulation)
		{
			m_isActive = m_isActiveInEditor;
		}
		else
		{
			m_isActive = isActive;
		}
		
		std::vector<Component*>& comps = GetComponents();

		for (auto* comp : comps)
		{
			comp->SetIsActive(m_isActive);
		}

		std::vector<GameObject*>& children = GetChildren();
		for (auto* child : children)
		{
			child->IsActive(m_isActive, isExitingSimulation);
		}
	}

	void GameObject::IsActiveInEditor()
	{
		m_isActiveInEditor = m_isActive;

		std::vector<GameObject*>& children = GetChildren();
		for (auto* child : children)
		{
			child->IsActiveInEditor();
		}
	}


	std::vector<Component*>& GameObject::GetComponents()
	{
		return m_components;
	}

	void GameObject::RemoveComponent(int pos)
	{
		if (pos <= 0 || pos >= m_components.size()) return;

		Component* comp = m_components[pos];
		m_components.erase(m_components.begin() + pos);
		delete comp;
	}

	void GameObject::RemoveComponent(Component* comp)
	{
		for (size_t i = 0; i < m_components.size(); i++)
		{
			if (m_components[i] == comp)
			{
				RemoveComponent((int)i);
				break;
			}
		}
	}

	Transform* GameObject::GetTransform()
	{
	    return dynamic_cast<Transform*>(m_components[0]);
	}

	const std::vector<GameObject*>& GameObject::GetObjectsWithTag(std::string_view tag)
	{
		return UIEditor::GetInspectorWindow().GetObjectsWithTag(tag);
	}

	bool GameObject::operator==(const GameObject& rhs) const
	{
	    return this == &rhs;
	}

	void GameObject::SetParent(GameObject* parent)
	{
		if (this == SceneManager::GetActiveScene()->sceneRoot)
		{
			std::cout << "Unable to set the parent of the scene root." << std::endl;
			return;
		}

		GameObject* obj = parent->GetParent();

		while (obj != nullptr)
		{
			if (*obj == *this)
			{
				parent->SetParent(m_parent);
			}

			obj = obj->GetParent();
		}	

	    if (m_parent)
	    {
	        const auto it = std::find(m_parent->GetChildren().begin(), m_parent->GetChildren().end(), this);
	        m_parent->GetChildren().erase(it);
	    }

	    m_parent = parent;
	    parent->m_children.push_back(this);

		GetTransform()->SetWorld(GetTransform()->GetWorldMatrix());
	}

	GameObject* GameObject::GetParent()
	{
	    return m_parent;
	}

	std::vector<GameObject*>& GameObject::GetChildren()
	{
	    return m_children;
	}

	void GameObject::RemoveChild(int pos)
	{
	    if (m_parent->GetParent() != nullptr || !SceneManager::GetActiveScene()->sceneRoot)
	    {
	        m_children[pos]->SetParent(SceneManager::GetActiveScene()->sceneRoot);
	        m_children.erase(m_children.begin() + pos);
	    }
	}

	void GameObject::Update()
	{
		RigidBody* rb = GetComponent<RigidBody>();
		if (rb != nullptr)
		{
			rb->GetRigidBody()->SetPosition(GetTransform()->GetWorldPosition());
			rb->GetRigidBody()->SetRotation(GetTransform()->GetWorldRotation());
		}
		else
		{
			RigidStatic* rs = GetComponent<RigidStatic>();
			if (rs != nullptr)
			{
				rs->GetRigidStatic()->SetPosition(GetTransform()->GetWorldPosition());
				rs->GetRigidStatic()->SetRotation(GetTransform()->GetWorldRotation());
			}
		}

		CollisionBox* cbComp = GetComponent<CollisionBox>();
		if (cbComp != nullptr)
		{
			cbComp->GetCollisionBox()->SetObjectScale(GetTransform()->GetWorldScale());
		}

		CollisionMesh* convexComp = GetComponent<CollisionMesh>();

		if (convexComp != nullptr)
		{
			convexComp->GetCollisionMesh()->SetScale(GetTransform()->GetWorldScale());
		}

		
		Model* modelComp = GetComponent<Model>();

		if(modelComp != nullptr)
		{
			modelComp->UpdateDrawableTransform(GetTransform()->GetWorldMatrix());
		}

		Light* lightComp = GetComponent<Light>();
		
		if (lightComp != nullptr)
		{
			lightComp->UpdatePosition(GetTransform()->GetWorldPosition());
			lightComp->UpdateDirection(GetTransform()->GetWorldForward());
		}
		
		Camera* cameraComp = GetComponent<Camera>();

		if (cameraComp != nullptr)
		{
			cameraComp->SetPosRot(GetTransform()->GetWorldPosition(), GetTransform()->GetWorldForward());
		}
	}

	void GameObject::SwitchToSimulation()
	{
		IsActiveInEditor();
		IsActive(false);
	}

	void GameObject::ExitSimulation()
	{		
		IsActive(m_isActiveInEditor, true);
	}

	void GameObject::SimulationStart()
	{
		Script* script = GetComponent<Script>();
		if (script)
		{
			script->Start();
		}

		SoundEmitter* soundEmitter = GetComponent<SoundEmitter>();
		if (soundEmitter != nullptr)
		{
			soundEmitter->Play();
		}

		for (GameObject* child : m_children)
		{
			child->SimulationStart();
		}
	}

	void GameObject::SimulationUpdate(float deltaTime)
	{
		PhysicUpdate();

		Script* script = GetComponent<Script>();
		if (script)
		{
			script->Update(deltaTime);
		}

		for (GameObject* child : m_children)
		{
			child->SimulationUpdate(deltaTime);
		}

	}

	void GameObject::PhysicUpdate()
	{
		auto* rigidBody = GetComponent<RigidBody>();
		if (rigidBody)
		{
			Physic::RigidBody* physicRigidBody = rigidBody->GetRigidBody();

			LibMath::Quaternion rotation = physicRigidBody->GetRotation();
			//rotation.Inverse();
			
			GetTransform()->SetPosition(physicRigidBody->GetPosition());
			GetTransform()->SetRotation(rotation);
			GetTransform()->SetScale(GetTransform()->GetWorldScale());
		}
	}

	void GameObject::LuaInit()
	{
		Transform::LuaInit();
		RigidBody::LuaInit();
		RigidStatic::LuaInit();
		CollisionShapeComponent::LuaInit();
		CollisionBox::LuaInit();
		CollisionCapsule::LuaInit();
		CollisionSphere::LuaInit(); 
		CollisionMesh::LuaInit();
		Model::LuaInit();
		Light::LuaInit();
		Camera::LuaInit();
		Text::LuaInit();
		Image::LuaInit();
		SoundEmitter::LuaInit();

		ScriptManager::GetSingleton()->AddClass<GameObject, std::string>("GameObject");

		ScriptManager::GetSingleton()->AddClassData<GameObject>("GameObject", "name", &GameObject::m_name, true);
		ScriptManager::GetSingleton()->AddClassData<GameObject>("GameObject", "tag", &GameObject::m_tag, true);
		ScriptManager::GetSingleton()->AddClassData<GameObject>("GameObject", "isActive", &GameObject::m_isActive, true);
		//ScriptManager::GetSingleton()->AddClassProperty<GameObject>("GameObject", "Parent", &GameObject::GetParent, &GameObject::SetParent);

		ScriptManager::GetSingleton()->AddClassFunction<GameObject>("GameObject", "GetParent", &GameObject::GetParent);
		ScriptManager::GetSingleton()->AddClassFunction<GameObject>("GameObject", "SetParent", &GameObject::SetParent);

		ScriptManager::GetSingleton()->AddClassFunction<GameObject>("GameObject", "GetComponentTransform", &GameObject::GetComponent<Transform>);
		ScriptManager::GetSingleton()->AddClassFunction<GameObject>("GameObject", "GetComponentModel", &GameObject::GetComponent<Model>);
		ScriptManager::GetSingleton()->AddClassFunction<GameObject>("GameObject", "GetComponentRigidBody", &GameObject::GetComponent<RigidBody>);
		ScriptManager::GetSingleton()->AddClassFunction<GameObject>("GameObject", "GetComponentRigidStatic", &GameObject::GetComponent<RigidStatic>);
		ScriptManager::GetSingleton()->AddClassFunction<GameObject>("GameObject", "GetComponentCollisionShape", &GameObject::GetComponent<CollisionShapeComponent>);
		ScriptManager::GetSingleton()->AddClassFunction<GameObject>("GameObject", "GetComponentCollisionBox", &GameObject::GetComponent<CollisionBox>);
		ScriptManager::GetSingleton()->AddClassFunction<GameObject>("GameObject", "GetComponentCollisionCapsule", &GameObject::GetComponent<CollisionCapsule>);
		ScriptManager::GetSingleton()->AddClassFunction<GameObject>("GameObject", "GetComponentCollisionSphere", &GameObject::GetComponent<CollisionSphere>);
		ScriptManager::GetSingleton()->AddClassFunction<GameObject>("GameObject", "GetComponentCollisionMesh", &GameObject::GetComponent<CollisionMesh>);
		ScriptManager::GetSingleton()->AddClassFunction<GameObject>("GameObject", "GetComponentLight", &GameObject::GetComponent<Light>);
		ScriptManager::GetSingleton()->AddClassFunction<GameObject>("GameObject", "GetComponentCamera", &GameObject::GetComponent<Camera>);
		ScriptManager::GetSingleton()->AddClassFunction<GameObject>("GameObject", "GetComponentImage", &GameObject::GetComponent<Image>);
		ScriptManager::GetSingleton()->AddClassFunction<GameObject>("GameObject", "GetComponentText", &GameObject::GetComponent<Text>);
		ScriptManager::GetSingleton()->AddClassFunction<GameObject>("GameObject", "GetComponentSoundEmitter", &GameObject::GetComponent<SoundEmitter>);
		
		ScriptManager::GetSingleton()->AddClassFunction<GameObject>("GameObject", "AddComponentModel", &GameObject::AddComponentModel);
		ScriptManager::GetSingleton()->AddClassFunction<GameObject>("GameObject", "AddComponentCollisionBox", &GameObject::AddComponentCollisionBox);
		ScriptManager::GetSingleton()->AddClassFunction<GameObject>("GameObject", "AddComponentCollisionCapsule", &GameObject::AddComponentCollisionCapsule);
		ScriptManager::GetSingleton()->AddClassFunction<GameObject>("GameObject", "AddComponentCollisionSphere", &GameObject::AddComponentCollisionSphere);
		ScriptManager::GetSingleton()->AddClassFunction<GameObject>("GameObject", "AddComponentCollisionMesh", &GameObject::AddComponentCollisionMesh);
		ScriptManager::GetSingleton()->AddClassFunction<GameObject>("GameObject", "AddComponentRigidBody", &GameObject::AddComponentRigidBody);
		ScriptManager::GetSingleton()->AddClassFunction<GameObject>("GameObject", "AddComponentRigidStatic", &GameObject::AddComponentRigidStatic);
		ScriptManager::GetSingleton()->AddClassFunction<GameObject>("GameObject", "AddComponentLight", &GameObject::AddComponentLight);
		ScriptManager::GetSingleton()->AddClassFunction<GameObject>("GameObject", "AddComponentImage", &GameObject::AddComponentImage);
		ScriptManager::GetSingleton()->AddClassFunction<GameObject>("GameObject", "AddComponentText", &GameObject::AddComponentText);
		ScriptManager::GetSingleton()->AddClassFunction<GameObject>("GameObject", "AddComponentCamera", &GameObject::AddComponentCamera);
		ScriptManager::GetSingleton()->AddClassFunction<GameObject>("GameObject", "AddComponentSoundEmitter", &GameObject::AddComponentSoundEmitter);


	}

	void GameObject::OnTrigger(Physic::CollisionShape* other, bool triggerEnter)
	{
		if (EditorApp::GetActualState() != EngineState::PLAY)
		{
			return;
		}

		Script* script = GetComponent<Script>();
		if (script)
		{
			CollisionShapeComponent* collisionShapeCpnt = GetComponent<CollisionShapeComponent>();
			if (collisionShapeCpnt)
			{
				GameObject* go = SceneManager::GetActiveScene()->sceneRoot->GetCollisionShapeGameObject(other);

				if (go)
				{
					script->OnTrigger(go, triggerEnter);
				}
			}
		}
	}

	void GameObject::OnContact(Physic::CollisionShape* other, bool contactBegin)
	{
		if (EditorApp::GetActualState() != EngineState::PLAY)
		{
			return;
		}

		Script* script = GetComponent<Script>();
		if (script)
		{
			CollisionShapeComponent* collisionShapeCpnt = GetComponent<CollisionShapeComponent>();
			if (collisionShapeCpnt)
			{
				GameObject* go = SceneManager::GetActiveScene()->sceneRoot->GetCollisionShapeGameObject(other);

				if (go)
				{
					script->OnContact(go, contactBegin);
				}
			}
		}
	}

	GameObject* GameObject::GetCollisionShapeGameObject(Physic::CollisionShape* cs)
	{
		if (!cs)
		{
			return nullptr;
		}

		CollisionShapeComponent* collisionShapeCpnt = GetComponent<CollisionShapeComponent>();
		if (collisionShapeCpnt && collisionShapeCpnt->IsSameCollisionShape(cs))
		{
			return this;
		}

		for (GameObject* child : m_children)
		{
			GameObject* go = child->GetCollisionShapeGameObject(cs);
			if (go)
			{
				return go;
			}
		}

		return nullptr;
	}

	void GameObject::DeserializeNextComp(const char* compName, const Core::SaveElement& data)
	{
		if (std::strcmp(compName, "transform") == 0)
		{
			Deserialize<Transform>(data, true);
		}
		else if (std::strcmp(compName, "light") == 0)
		{
			Deserialize<Light>(data, true);
		}
		else if (std::strcmp(compName, "model") == 0)
		{
			Deserialize<Model>(data, true);
		}
		else if (std::strcmp(compName, "rigid_static") == 0)
		{
			Deserialize<RigidStatic>(data, true);
		}
		else if (std::strcmp(compName, "rigid_body") == 0)
		{
			Deserialize<RigidBody>(data, true);
		}
		else if (std::strcmp(compName, "collision_sphere") == 0)
		{
			Deserialize<CollisionSphere>(data, true);
		}
		else if (std::strcmp(compName, "collision_shape") == 0)
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_TRACE, "No collision shape component should exist alone !");
		}
		else if (std::strcmp(compName, "collision_mesh") == 0)
		{
			DeserializeCollisionMesh(data);
		}
		else if (std::strcmp(compName, "collision_capsule") == 0)
		{
			Deserialize<CollisionCapsule>(data, true);
		}
		else if (std::strcmp(compName, "collision_box") == 0)
		{
			Deserialize<CollisionBox>(data, true);
		}
		else if (std::strcmp(compName, "text") == 0)
		{
			Deserialize<Text>(data, true);
		}
		else if (std::strcmp(compName, "image") == 0)
		{
			Deserialize<Image>(data, true);
		}
		else if (std::strcmp(compName, "camera") == 0)
		{
			Deserialize<Camera>(data, true);
		}
		else if (std::strcmp(compName, "scripts") == 0)
		{
			Deserialize<Script>(data, false);
		}
		else if(std::strcmp(compName, SOUND_EMITTER_ELEMENT) == 0)
		{
			Deserialize<SoundEmitter>(data, true);
		}
		else
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_INFO, "Component with name %s does not exist.", compName);
		}
	}


	void GameObject::DeserializeCollisionMesh(const Core::SaveElement& data)
	{
		Model* modelComp = GetComponent<Model>();

		if (modelComp == nullptr)
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_TRACE, "No model was load before creating a collision mesh component");
			return;
		}
		
		std::vector<LibMath::Vector3> points;
		points.reserve(modelComp->GetDrawableObject()->GetRenderingMesh()->vertices.size());
		
		for (const GeometricTools::Vertex& v : modelComp->GetDrawableObject()->GetRenderingMesh()->vertices)
		{
			points.emplace_back(v.position[0], v.position[1], v.position[2]);
		}

		auto* collisionMesh = new CollisionMesh(this);

		collisionMesh->Deserialize(data);
		collisionMesh->SetPoints(points);

		AddComponent<CollisionMesh>(collisionMesh);
		
	}

	Model* GameObject::AddComponentModel()
	{
		if (!GetComponent<Model>())
		{
			AddComponent(new Model(this, "Cube"));
		}

		return GetComponent<Model>();
	}

	CollisionBox* GameObject::AddComponentCollisionBox()
	{
		if (!GetComponent<CollisionShapeComponent>())
		{
			AddComponent(new CollisionBox(this, { 1, 1, 1 }));
		}

		return GetComponent<CollisionBox>();
	}

	CollisionCapsule* GameObject::AddComponentCollisionCapsule()
	{
		if (!GetComponent<CollisionShapeComponent>())
		{
			AddComponent(new CollisionCapsule(this, 1, 2));
		}

		return GetComponent<CollisionCapsule>();
	}

	CollisionSphere* GameObject::AddComponentCollisionSphere()
	{
		if (!GetComponent<CollisionShapeComponent>())
		{
			AddComponent(new CollisionSphere(this, 1.0f));
		}

		return GetComponent<CollisionSphere>();
	}

	CollisionMesh* GameObject::AddComponentCollisionMesh()
	{
		if (GetComponent<Model>() && !GetComponent<CollisionShapeComponent>())
		{
			std::vector<GeometricTools::Vertex> vertex = GetComponent<Model>()->GetDrawableObject()->GetRenderingMesh()->vertices;

			std::vector<LibMath::Vector3> points;
			for (GeometricTools::Vertex v : vertex)
			{
				points.emplace_back(v.position[0], v.position[1], v.position[2]);
			}

			AddComponent(new CollisionMesh(this, points, GetTransform()->GetWorldScale()));
		}

		return GetComponent<CollisionMesh>();
	}

	RigidBody* GameObject::AddComponentRigidBody()
	{
		if (!GetComponent<CollisionShapeComponent>())
		{
			AddComponent(new CollisionBox(this, GetTransform()->GetWorldScale()));
		}
		if (!GetComponent<RigidBody>() && !GetComponent<RigidStatic>())
		{
			AddComponent(new RigidBody(this, GetTransform()->GetWorldPosition(), GetComponent<CollisionShapeComponent>()->GetCollisionShape()));
		}

		return GetComponent<RigidBody>();
	}

	RigidStatic* GameObject::AddComponentRigidStatic()
	{
		if (!GetComponent<CollisionShapeComponent>())
		{
			AddComponent(new CollisionBox(this, GetTransform()->GetWorldScale()));
		}
		if (!GetComponent<RigidBody>() && !GetComponent<RigidStatic>())
		{
			AddComponent(new RigidStatic(this, GetTransform()->GetWorldPosition(), GetComponent<CollisionShapeComponent>()->GetCollisionShape()));
		}

		return GetComponent<RigidStatic>();
	}

	Light* GameObject::AddComponentLight()
	{
		if (!GetComponent<Light>())
		{
			AddComponent(new Light(this, Rendering::ELightType::POINT, *GetTransform()));
			GetComponent<Light>()->UpdateAmbient({ 0.3f, 0.3f, 0.3f });
			GetComponent<Light>()->UpdateDiffuse({ 1.f, 1.f, 1.f });
			GetComponent<Light>()->UpdateSpecular({ 1.f, 1.0f, 1.0f });
			GetComponent<Light>()->UpdateConstantAttenuation(1.0f);
			GetComponent<Light>()->UpdateLinearAttenuation(0.09f);
			GetComponent<Light>()->UpdateQuadraticAttenuation(0.032f);
			//UINodeLight::UpdateLightProperties(m_obj);
		}
		
		return GetComponent<Light>();
	}

	Image* GameObject::AddComponentImage()
	{
		if (!GetComponent<Image>())
		{
			AddComponent(new Image(this));
		}

		return GetComponent<Image>();
	}

	Text* GameObject::AddComponentText()
	{
		if (!GetComponent<Text>())
		{
			AddComponent(new Text(this));
		}

		return GetComponent<Text>();
	}

	Camera* GameObject::AddComponentCamera()
	{
		if (!GetComponent<Camera>())
		{
			AddComponent(new Camera(this));
		}

		return GetComponent<Camera>();
	}

	SoundEmitter* GameObject::AddComponentSoundEmitter()
	{
		if (!GetComponent<SoundEmitter>())
		{
			AddComponent(new SoundEmitter(this));
		}

		return GetComponent<SoundEmitter>();
	}
}
