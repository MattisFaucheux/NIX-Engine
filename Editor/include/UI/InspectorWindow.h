#pragma once

#include <vector>
#include <string>

#include "Components/UINodeCamera.h"
#include "Components/UINodeCollisionBox.h"
#include "Components/UINodeCollisionCapsule.h"
#include "Components/UINodeCollisionConvex.h"
#include "Components/UINodeCollisionSphere.h"
#include "Components/UINodeImage.h"
#include "Components/UINodeLight.h"
#include "Components/UINodeModel.h"
#include "Components/UINodeRigidBody.h"
#include "Components/UINodeRigidStatic.h"
#include "Components/UINodeScript.h"
#include "Components/UINodeSoundEmitter.h"
#include "Components/UINodeText.h"
#include "Components/UINodeTransform.h"

namespace NIX::Editor
{
	class Component;

	class GameObject;

	class InspectorWindow
	{
		GameObject* m_obj = nullptr;

		/// Add Component

		enum class ERROR_ADD_COMPONENT : int
		{
			NO_ERROR,
			MODEL_ALREADY_EXISTING,
			COLLISION_SHAPE_ALREADY_EXISTING,
			NO_MODEL_EXISTING,
			RB_OR_RS_ALREADY_EXISTING,
			LIGHT_ALREADY_EXISTING,
			IMAGE_ALREADY_EXISTING,
			TEXT_ALREADY_EXISTING,
			CAMERA_ALREADY_EXISTING,
			SOUND_EMITTER_ALREADY_EXISTING,
			SCRIPT_ALREADY_EXISTING
		};

		std::vector<std::string> m_listComps = { "Model", "Collision Box", "Collision Capsule", "Collision Sphere", "Collision Convex", "RigidBody", "RigidStatic", "Light", "Image", "Text", "Camera", "Sound Emitter", "Script" };

		std::string m_errMessageIntro;
		std::string m_errMessage;
		bool m_isErrorAddWindowOpen = false;

		/// Right-Click Component

		std::vector<std::string> m_rightClickComp = { "Delete" };

		/// Global Properties

		bool m_isActive;
		std::string m_name;
		std::vector<std::string> m_tags;
		std::string m_tag;

		std::vector<std::vector<GameObject*>> m_gameObjectsTagged;
		std::vector<GameObject*> m_gameObjectsTaggedEmpty;

		/// Tag Settings

		bool m_isTagWindowOpen = false;

		std::string m_tagCreated = "";
		std::string m_tagErr = "";

		int m_idDrag = 0;

		///
		///
		
		UINodeTransform m_uiNodeTransform;
		UINodeCamera m_uiNodeCamera;
		UINodeLight m_uiNodeLight;
		UINodeModel m_uiNodeModel;
		
		UINodeCollisionBox m_uiNodeCollisionBox;
		UINodeCollisionCapsule m_uiNodeCollisionCapsule;
		UINodeCollisionSphere m_uiNodeCollisionSphere;
		UINodeCollisionConvex m_uiNodeCollisionConvex;
		UINodeRigidBody m_uiNodeRigidBody;
		UINodeRigidStatic m_uiNodeRigidStatic;
		
		UINodeImage m_uiNodeImage;
		UINodeText m_uiNodeText;
		
		UINodeScript m_uiNodeScript;
		
		UINodeSoundEmitter m_uiNodeSoundEmitter;
		
		
		

		int GetPosTag(std::string_view tag);

		void UpdateTagWindow();

		void UpdateGlobalProperties();

		void UpdateInspectorComponents();

		void AddComponent();
		bool AddComponentSwitch(int nbr);

	public:
		bool InitObjectsTagged();
		void ClearObjectsTagged();
		void RemoveObjFromTags(GameObject* obj);
		bool AddObjectTag(GameObject* obj, std::string_view tag);
		const std::vector<GameObject*>& GetObjectsWithTag(std::string_view tag);

		bool RightClickComp(Component* comp);

		void SetLeftText(std::string_view text);

		void SetGameObject(GameObject* obj);

		void UpdateInspectorWindow();

		[[nodiscard]] const std::vector<std::string>& GetTagsList();
		void AddTag(const char* tag);

		void SetTagsNb();

		[[nodiscard]] bool OpenComponentNode(bool& isNodeOpen, std::string_view tableName);
		void CloseComponentNode();

		UINodeModel& GetUINodeModel();
		UINodeImage& GetUINodeImage();
		UINodeScript& GetUINodeScript();
		UINodeText& GetUINodeText();
		UINodeSoundEmitter& GetUINodeSoundEmitter();
		
		
	};
}
