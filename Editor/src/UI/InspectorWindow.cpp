#include "pch.h"
#include "UI/InspectorWindow.h"

#include <iostream>

#include "UI/Components/UINodeLight.h"
#include "UI/Components/UINodeModel.h"
#include "UI/Components/UINodeCamera.h"
#include "UI/Components/UINodeSoundEmitter.h"
#include "UI/Components/UINodeScript.h"
#include "UI/Components/UINodeText.h"
#include "UI/Components/UINodeImage.h"
#include "UI/Components/UINodeRigidBody.h"
#include "UI/Components/UINodeRigidStatic.h"
#include "UI/Components/UINodeCollisionBox.h"
#include "UI/Components/UINodeCollisionSphere.h"
#include "UI/Components/UINodeCollisionCapsule.h"
#include "UI/Components/UINodeCollisionConvex.h"
#include "UI/UI.h"
#include "GameObject.h"
#include "PhysicSimulation.h"
#include "SceneManager.h"
#include "Log/Logger.h"
#include "MeshImport/Mesh.h"
#include "DrawableObject/DrawableObject.h"
#include "Light/ELightType.h"
#include "Components/CollisionBox.h"
#include "Components/CollisionCapsule.h"
#include "Components/CollisionMesh.h"
#include "Components/CollisionSphere.h"
#include "Components/RigidBody.h"
#include "Components/RigidStatic.h"
#include "Components/Lights/Light.h"
#include "Components/Model.h"
#include "Components/HUD/Image.h"
#include "Components/HUD/Text.h"
#include "Components/Sound/SoundEmitter.h"
#include "Components/Scripting/Script.h"

namespace NIX::Editor
{
	int InspectorWindow::GetPosTag(std::string_view tag)
	{
		int pos = -1;

		for (size_t i = 0; i < m_tags.size(); i++)
		{
			if (m_tags[i] == tag)
			{
				pos = (int)i;
				break;
			}
		}

		return pos;
	}

	void InspectorWindow::UpdateTagWindow()
	{
		bool isOpen;
		UI::GetUI()->SetNextWindowPos(UI::GetUI()->GetIOWidth() * .5f - 200.f, UI::GetUI()->GetIOHeight() * .5f - 350.f);
		UI::GetUI()->SetNextWindowSize(400.f, 470.f);
		UI::GetUI()->NewWindow("Tag Settings", false, &isOpen, false);

		UI::GetUI()->SetCursorPosY(UI::GetUI()->GetCursorPosY() + 20.f);
		UI::GetUI()->AddText("Tags");
		UI::GetUI()->NewChild("##ListOfTags", 0.f, 300.f, true);

		UI::GetUI()->SetCursorPosY(UI::GetUI()->GetCursorPosY() + 2.f);
		UI::GetUI()->AddText(m_tags[0]);
		UI::GetUI()->SetCursorPosY(UI::GetUI()->GetCursorPosY() + 7.f);
		UI::GetUI()->AddText(m_tags[1]);
		UI::GetUI()->SetCursorPosY(UI::GetUI()->GetCursorPosY() + 7.f);
		UI::GetUI()->AddText(m_tags[2]);
		UI::GetUI()->SetCursorPosY(UI::GetUI()->GetCursorPosY() + 2.f);

		for (size_t i = 3; i < m_tags.size() - 1; i++)
		{
			UI::GetUI()->SetCursorPosY(UI::GetUI()->GetCursorPosY() + 5.f);

			UI::GetUI()->AddText(m_tags[i]);
			UI::GetUI()->SameLine();

			UI::GetUI()->SetCursorPos(UI::GetUI()->GetCursorPosX() + UI::GetUI()->GetContentRegionAvailX() - 20.f, UI::GetUI()->GetCursorPosY() - 5.f);

			if (UI::GetUI()->AddButton("X##DeleteTag" + std::to_string(i), 20.f, 20.f))
			{
				while (!m_gameObjectsTagged[i].empty())
				{
					m_gameObjectsTagged[i][0]->SetTag("Untagged");
				}

				m_tags.erase(m_tags.begin() + i);
				m_gameObjectsTagged.erase(m_gameObjectsTagged.begin() + i);

				UpdateGlobalProperties();
			}
		}

		UI::GetUI()->EndChild();

		UI::GetUI()->SetCursorPosY(UI::GetUI()->GetCursorPosY() + 20.f);
		UI::GetUI()->AddInputText("Create new Tag", "Type here", m_tagCreated);

		if (UI::GetUI()->AddButton("Add", 100.f, 20.f))
		{
			if (m_tagCreated.empty())
			{
				m_tagErr = "Couldn't add an empty Tag.";
			}
			else if (GetPosTag(m_tagCreated) >= 0)
			{
				m_tagErr = "This Tag already exists.";
			}
			else
			{
				m_tags.insert(m_tags.end() - 1, m_tagCreated);
				m_gameObjectsTagged.resize(m_tags.size() - 1);
				m_tagCreated = "";
				m_tagErr = "";
			}
		}
		UI::GetUI()->AddText(m_tagErr);

		UI::GetUI()->EndWindow();

		if (!isOpen)
			m_isTagWindowOpen = false;
	}

	void InspectorWindow::UpdateGlobalProperties()
	{
		m_isActive = m_obj->IsActive();
		m_name = m_obj->GetName();
		m_tag = m_obj->GetTag();
	}

	void InspectorWindow::UpdateInspectorComponents()
	{
		m_uiNodeTransform.UpdateComponent();
		m_uiNodeRigidBody.UpdateComponent();
		m_uiNodeRigidStatic.UpdateComponent();
		m_uiNodeCollisionBox.UpdateComponent();
		m_uiNodeCollisionSphere.UpdateComponent();
		m_uiNodeCollisionCapsule.UpdateComponent();
		m_uiNodeCollisionConvex.UpdateComponent();
		m_uiNodeLight.UpdateComponent();
		m_uiNodeModel.UpdateComponent();
		m_uiNodeImage.UpdateComponent();
		m_uiNodeText.UpdateComponent();
		m_uiNodeCamera.UpdateComponent();
		m_uiNodeSoundEmitter.UpdateComponent();
		m_uiNodeScript.UpdateComponent();
	}

	void NIX::Editor::InspectorWindow::AddComponent()
	{
		static bool openAddComp = false;
		static std::string strAddComp = "Add Component";

		if (UI::GetUI()->AddButton(strAddComp, UI::GetUI()->GetContentRegionAvailX(), 20.f))
		{
			openAddComp = !openAddComp;
		}

		if (!openAddComp)
		{
			strAddComp = "Add Component";
			return;
		}

		strAddComp = "Close";
		UI::GetUI()->SetCursorPosY(UI::GetUI()->GetCursorPosY() - 5.f);

		static std::vector<std::string> listCompsPrint = m_listComps;
		static std::string filter = "";

		UI::GetUI()->NewChild("Add Component", 0.f, 200.f, true);

		if (UI::GetUI()->AddInputText(" Filter##AddComponent", "Filter", filter))
		{
			listCompsPrint.clear();

			std::string data = filter;
			for (size_t i = 0; i < data.size(); i++)
				data[i] = (char)std::tolower(data[i]);

			for (auto t : m_listComps)
			{
				std::string data2 = t.data();
				for (size_t i = 0; i < data2.size(); i++)
					data2[i] = (char)std::tolower(data2[i]);

				if (data2.find(data) != std::string::npos)
					listCompsPrint.push_back(t.data());
			}
		}

		UI::GetUI()->NewChild("Add Component2", 0.f, 160.f, true);

		int nbr = 0;
		for (auto t : listCompsPrint)
		{
			while (t != m_listComps[nbr])
				nbr++;

			if (UI::GetUI()->AddSelectable(t + "##ListCompAddInspector", UI::GetUI()->GetContentRegionAvailX(), 15.f))
			{
				bool result = AddComponentSwitch(nbr);
				m_isErrorAddWindowOpen = !result;
				m_errMessageIntro = "Couldn't add " + m_listComps[nbr] + ".";
				openAddComp = !result;
			}
		}

		UI::GetUI()->EndChild();
		UI::GetUI()->EndChild();

		if (m_isErrorAddWindowOpen)
		{
			bool isOpen;
			UI::GetUI()->SetNextWindowPos(UI::GetUI()->GetIOWidth() * .5f - 300.f, UI::GetUI()->GetIOHeight() * .5f - 100.f);
			UI::GetUI()->SetNextWindowSize(600.f, 100.f);
			UI::GetUI()->NewWindow("Failed to add new component", false, &isOpen, false);
			UI::GetUI()->AddText(m_errMessageIntro);
			UI::GetUI()->AddText(m_errMessage);
			UI::GetUI()->EndWindow();

			if (!isOpen)
				m_isErrorAddWindowOpen = false;
		}
	}

	bool NIX::Editor::InspectorWindow::AddComponentSwitch(int nbr)
	{
		ERROR_ADD_COMPONENT err = ERROR_ADD_COMPONENT::NO_ERROR;

		switch (nbr)
		{
		case 0:
			if (!m_obj->GetComponent<Model>())
				m_obj->AddComponent(new Model(m_obj, "Cube"));
			else
				err = ERROR_ADD_COMPONENT::MODEL_ALREADY_EXISTING;
			break;
		case 1:
			if (!m_obj->GetComponent<CollisionShapeComponent>())
				m_obj->AddComponent(new CollisionBox(m_obj, {1, 1, 1}));
			else
				err = ERROR_ADD_COMPONENT::COLLISION_SHAPE_ALREADY_EXISTING;
			break;
		case 2:
			if (!m_obj->GetComponent<CollisionShapeComponent>())
				m_obj->AddComponent(new CollisionCapsule(m_obj, 1, 2));
			else
				err = ERROR_ADD_COMPONENT::COLLISION_SHAPE_ALREADY_EXISTING;
			break;
		case 3:
			if (!m_obj->GetComponent<CollisionShapeComponent>())
				m_obj->AddComponent(new CollisionSphere(m_obj, 1.0f));
			else
				err = ERROR_ADD_COMPONENT::COLLISION_SHAPE_ALREADY_EXISTING;
			break;
		case 4:
			if (!m_obj->GetComponent<Model>())
				err = ERROR_ADD_COMPONENT::NO_MODEL_EXISTING;
			else if (!m_obj->GetComponent<CollisionShapeComponent>())
			{
				std::vector<GeometricTools::Vertex> vertex = m_obj->GetComponent<Model>()->GetDrawableObject()->GetRenderingMesh()->vertices;

				std::vector<LibMath::Vector3> points;
				for (GeometricTools::Vertex v : vertex)
				{
					points.emplace_back(v.position[0], v.position[1], v.position[2]);
				}

				m_obj->AddComponent(new CollisionMesh(m_obj, points, m_obj->GetTransform()->GetWorldScale()));
			}
			else
				err = ERROR_ADD_COMPONENT::COLLISION_SHAPE_ALREADY_EXISTING;
			break;
		case 5:
			if (!m_obj->GetComponent<CollisionShapeComponent>())
				m_obj->AddComponent(new CollisionBox(m_obj, m_obj->GetTransform()->GetWorldScale()));
			if (!m_obj->GetComponent<RigidBody>() && !m_obj->GetComponent<RigidStatic>())
				m_obj->AddComponent(new RigidBody(m_obj, m_obj->GetTransform()->GetWorldPosition(), m_obj->GetComponent<CollisionShapeComponent>()->GetCollisionShape()));
			else
				err = ERROR_ADD_COMPONENT::RB_OR_RS_ALREADY_EXISTING;
			break;
		case 6:
			if (!m_obj->GetComponent<CollisionShapeComponent>())
				m_obj->AddComponent(new CollisionBox(m_obj, m_obj->GetTransform()->GetWorldScale()));
			if (!m_obj->GetComponent<RigidBody>() && !m_obj->GetComponent<RigidStatic>())
				m_obj->AddComponent(new RigidStatic(m_obj, m_obj->GetTransform()->GetWorldPosition(), m_obj->GetComponent<CollisionShapeComponent>()->GetCollisionShape()));
			else
				err = ERROR_ADD_COMPONENT::RB_OR_RS_ALREADY_EXISTING;
			break;
		case 7:
			if (!m_obj->GetComponent<Light>())
			{
				m_obj->AddComponent(new Light(m_obj, Rendering::ELightType::POINT, *m_obj->GetTransform()));
				m_obj->GetComponent<Light>()->UpdateAmbient({ 0.3f, 0.3f, 0.3f });
				m_obj->GetComponent<Light>()->UpdateDiffuse({ 1.0f, 1.0f, 1.0f });
				m_obj->GetComponent<Light>()->UpdateSpecular({ 1.0f, 1.0f, 1.0f });
				m_obj->GetComponent<Light>()->UpdateConstantAttenuation(1.0f);
				m_obj->GetComponent<Light>()->UpdateLinearAttenuation(0.09f);
				m_obj->GetComponent<Light>()->UpdateQuadraticAttenuation(0.032f);
				m_uiNodeLight.UpdateComponentProperties(m_obj);
			}
			else
				err = ERROR_ADD_COMPONENT::LIGHT_ALREADY_EXISTING;
			break;
		case 8:
			if (!m_obj->GetComponent<Image>())
				m_obj->AddComponent(new Image(m_obj));
			else
				err = ERROR_ADD_COMPONENT::IMAGE_ALREADY_EXISTING;
			break;
		case 9:
			if (!m_obj->GetComponent<Text>())
				m_obj->AddComponent(new Text(m_obj));
			else
				err = ERROR_ADD_COMPONENT::TEXT_ALREADY_EXISTING;
			break;
		case 10:
			if (!m_obj->GetComponent<Camera>())
				m_obj->AddComponent(new Camera(m_obj));
			else
				err = ERROR_ADD_COMPONENT::CAMERA_ALREADY_EXISTING;
			break;
		case 11:
			if (!m_obj->GetComponent<SoundEmitter>())
				m_obj->AddComponent(new SoundEmitter(m_obj));
			else
				err = ERROR_ADD_COMPONENT::SOUND_EMITTER_ALREADY_EXISTING;
			break;
		case 12:
			if (!m_obj->GetComponent<Script>())
				m_obj->AddComponent(new Script(m_obj));
			else
				err = ERROR_ADD_COMPONENT::SCRIPT_ALREADY_EXISTING;
			break;
		default:
			break;
		}

		m_errMessage = "Reason: ";

		switch (err)
		{
		case ERROR_ADD_COMPONENT::NO_ERROR:
			return true;
		case ERROR_ADD_COMPONENT::MODEL_ALREADY_EXISTING:
			m_errMessage += "This GameObject already contain a 'Model'.";
			break;
		case ERROR_ADD_COMPONENT::COLLISION_SHAPE_ALREADY_EXISTING:
			m_errMessage += "This GameObject already contain a 'Collision Shape' component.";
			break;
		case ERROR_ADD_COMPONENT::NO_MODEL_EXISTING:
			m_errMessage += "This GameObject doesn't contain a 'Model'.";
			break;
		case ERROR_ADD_COMPONENT::RB_OR_RS_ALREADY_EXISTING:
			m_errMessage += "This GameObject already contain a 'RigidBody' or a 'RigidStatic'.";
			break;
		case ERROR_ADD_COMPONENT::LIGHT_ALREADY_EXISTING:
			m_errMessage += "This GameObject already contain a 'Light'.";
			break;
		case ERROR_ADD_COMPONENT::IMAGE_ALREADY_EXISTING:
			m_errMessage += "This GameObject already contain an 'Image'.";
			break;
		case ERROR_ADD_COMPONENT::TEXT_ALREADY_EXISTING:
			m_errMessage += "This GameObject already contain a 'Text'.";
			break;
		case ERROR_ADD_COMPONENT::CAMERA_ALREADY_EXISTING:
			m_errMessage += "This GameObject already contain a 'Camera'.";
			break;
		case ERROR_ADD_COMPONENT::SOUND_EMITTER_ALREADY_EXISTING:
			m_errMessage += "This GameObject already contain a 'Sound Emitter'.";
			break;
		case ERROR_ADD_COMPONENT::SCRIPT_ALREADY_EXISTING:
			m_errMessage += "This GameObject already contain a 'Script'.\n\nTips: You can put as many scripts as you want in one Component Script.";
			break;
		default:
			m_errMessage += "Unknown error.";
			break;
		}

		return false;
	}

	bool InspectorWindow::InitObjectsTagged()
	{
		if (m_gameObjectsTagged[0].empty())
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_ERROR, "Root of the scene not found.");
			return false;
		}

		m_gameObjectsTagged[0].erase(m_gameObjectsTagged[0].begin());

		if (m_gameObjectsTagged[2].empty())
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_ERROR, "There is no main camera. Please tag a GameObject as 'MainCamera'.");
			return false;
		}

		Camera* cam = m_gameObjectsTagged[2][0]->GetComponent<Camera>();

		if (!cam)
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_ERROR, "GameObject '%s' is tagged as 'MainCamera' but doesn't have a 'Camera' component.", m_gameObjectsTagged[2][0]->GetName().c_str());
			return false;
		}

		SceneManager::SetMainCamera(cam->GetCamera());
		return true;
	}

	void InspectorWindow::ClearObjectsTagged()
	{
		for (auto& vec : m_gameObjectsTagged)
		{
			vec.clear();
		}
	}

	void InspectorWindow::RemoveObjFromTags(GameObject* obj)
	{
		int pos = GetPosTag(obj->GetTag());

		if (pos < 0)
		{
			//Impossible
			ASSERT(SOURCE_INFO, "Couldn't remove '%s' from '%s', this tag doesn't exist.", obj->GetTag().c_str(), obj->GetName().c_str());
			return;
		}

		auto it = std::find(m_gameObjectsTagged[pos].begin(), m_gameObjectsTagged[pos].end(), obj);

		if (it != m_gameObjectsTagged[pos].end())
			m_gameObjectsTagged[pos].erase(it);
	}

	bool InspectorWindow::AddObjectTag(GameObject* obj, std::string_view tag)
	{
		int pos = GetPosTag(tag);

		if (pos < 0)
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_WARNING, "Tag '%s' doesn't exist.", tag.data());
			return false;
		}

		RemoveObjFromTags(obj);
		m_gameObjectsTagged[pos].emplace_back(obj);
		return true;
	}

	const std::vector<GameObject*>& InspectorWindow::GetObjectsWithTag(std::string_view tag)
	{
		int pos = GetPosTag(tag);

		if (pos < 0)
		{
			return m_gameObjectsTaggedEmpty;
		}

		return m_gameObjectsTagged[pos];
	}

	bool NIX::Editor::InspectorWindow::RightClickComp(Component* comp)
	{
		if (UI::GetUI()->IsClicked(1))
		{
			UI::GetUI()->OpenPopup("##RightClickedComponent");
		}
		if (UI::GetUI()->AddPopup("##RightClickedComponent", m_rightClickComp) == 0)
		{
			m_obj->RemoveComponent(comp);

			
			
			return false;
		}

		return true;
	}

	void NIX::Editor::InspectorWindow::SetLeftText(std::string_view text)
	{
		UI::GetUI()->TableNextRow();
		UI::GetUI()->TableNextColumn();
		UI::GetUI()->AddText(text);
		UI::GetUI()->TableNextColumn();
		UI::GetUI()->SetNextItemWitdh(UI::GetUI()->GetContentRegionAvailX());
	}

	void InspectorWindow::SetGameObject(GameObject* obj)
	{
		m_obj = obj;

		if (!m_obj) { return; }

		UpdateGlobalProperties();
		m_uiNodeTransform.UpdateComponentProperties(obj);
		m_uiNodeRigidBody.UpdateComponentProperties(obj);
		m_uiNodeRigidStatic.UpdateComponentProperties(obj);
		m_uiNodeCollisionBox.UpdateComponentProperties(obj);
		m_uiNodeCollisionSphere.UpdateComponentProperties(obj);
		m_uiNodeCollisionCapsule.UpdateComponentProperties(obj);
		m_uiNodeCollisionConvex.UpdateComponentProperties(obj);
		m_uiNodeLight.UpdateComponentProperties(obj);
		m_uiNodeModel.UpdateComponentProperties(obj);
		m_uiNodeImage.UpdateComponentProperties(obj);
		m_uiNodeText.UpdateComponentProperties(obj);
		m_uiNodeCamera.UpdateComponentProperties(obj);
		m_uiNodeSoundEmitter.UpdateComponentProperties(obj);
		m_uiNodeScript.UpdateComponentProperties(obj);
	}

	void InspectorWindow::UpdateInspectorWindow()
	{
		if (!UI::GetUI()->NewWindow("Inspector") || m_obj == nullptr)
		{
			UI::GetUI()->EndWindow();
			return;
		}

		if (UI::GetUI()->AddCheckbox("##isActiveGlobalProperties", m_isActive))
		{
			m_obj->IsActive(m_isActive);
		}

		UI::GetUI()->SameLine();

		if (UI::GetUI()->AddInputText(" Name##GlobalProperties", "GameObject Name", m_name))
		{
			if (m_name.empty())
			{
				m_name = m_obj->GetName();
			}
			else
			{
				m_obj->SetName(m_name);
			}
		}

		if (UI::GetUI()->AddComboAdder(" Tag##GlobalProperties", m_tags, m_tag))
		{
			if (m_tag == m_tags[m_tags.size() - 1])
			{
				m_tag = m_obj->GetTag();
				m_isTagWindowOpen = true;
			}
			else
			{
				m_obj->SetTag(m_tag);
			}
		}

		UpdateInspectorComponents();
		AddComponent();

		if (m_isTagWindowOpen) UpdateTagWindow();

		UI::GetUI()->EndWindow();
	}

	const std::vector<std::string>& InspectorWindow::GetTagsList()
	{
		return m_tags;
	}

	void InspectorWindow::AddTag(const char* tag)
	{		
		m_tags.emplace_back(tag);
	}

	void InspectorWindow::SetTagsNb()
	{
		if (m_tags.size() > 0)
		{
			m_gameObjectsTagged.resize(m_tags.size() - 1);
		}
	}

	bool InspectorWindow::OpenComponentNode(bool& isNodeOpen, std::string_view tableName)
	{
		if (!isNodeOpen)
		{
			isNodeOpen = true;
			return false;
		}

		UI::GetUI()->SetCursorPosY(UI::GetUI()->GetCursorPosY() + 3.f);

		const float w = UI::GetUI()->GetContentRegionAvailX();
		if (w < 0.1f)
		{
			return false;
		}

		if (UI::GetUI()->AddTable(tableName, 2))
		{
			UI::GetUI()->TableSetUpColumn("", false, w * .4f);
			UI::GetUI()->TableSetUpColumn("", false, w * .6f);
		}

		return true;
	}

	void InspectorWindow::CloseComponentNode()
	{
		UI::GetUI()->CloseTable();
	}

	UINodeModel& InspectorWindow::GetUINodeModel()
	{
		return m_uiNodeModel;
	}

	UINodeImage& InspectorWindow::GetUINodeImage()
	{
		return m_uiNodeImage;
	}

	UINodeScript& InspectorWindow::GetUINodeScript()
	{
		return m_uiNodeScript;
	}

	UINodeText& InspectorWindow::GetUINodeText()
	{
		return m_uiNodeText;
	}

	UINodeSoundEmitter& InspectorWindow::GetUINodeSoundEmitter()
	{
		return m_uiNodeSoundEmitter;
	}
}
