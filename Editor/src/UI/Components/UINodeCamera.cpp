#include "UI/Components/UINodeCamera.h"

#include "SceneManager.h"
#include "UI/InspectorWindow.h"
#include "UI/UI.h"
#include "UI/UIEditor.h"

namespace NIX::Editor
{
	void UINodeCamera::UpdateComponentProperties(GameObject* obj)
	{
		if (obj == nullptr)
		{
			return;
		}

		m_obj = obj;

		Camera* cameraComp = m_obj->GetComponent<Camera>();
		if (cameraComp == nullptr)
		{
			return;
		}

		m_far = cameraComp->GetCamera()->GetFar();
		m_fov = cameraComp->GetCamera()->GetFov();
	}

	void UINodeCamera::UpdateComponent()
	{
		if (m_obj == nullptr || m_obj->GetComponent<Camera>() == nullptr)
		{
			return;
		}

		static bool isCameraOpen = true;

		UI::GetUI()->NewChild("Camera", 0.0f, isCameraOpen ? 85.0f : 30.0f, true);
		UI::GetUI()->SetNextItemOpen(isCameraOpen);

		if (UI::GetUI()->AddNode("Camera"))
		{
			if (UIEditor::GetInspectorWindow().RightClickComp(m_obj->GetComponent<Camera>()))
				UpdateComponentNode(isCameraOpen);
			UI::GetUI()->CloseNode();
		}
		else
		{
			UIEditor::GetInspectorWindow().RightClickComp(m_obj->GetComponent<Camera>());
			isCameraOpen = false;
		}

		UI::GetUI()->EndChild();
	}

	void UINodeCamera::UpdateComponentNode(bool& isComponentOpen)
	{
		Camera* camera = m_obj->GetComponent<Camera>();
		if (camera == nullptr)
		{
			return;
		}

		if (!UIEditor::GetInspectorWindow().OpenComponentNode(isComponentOpen, "##CameraTable"))
		{
			return;
		}
		
		UpdateComponentProperties(m_obj);
		
		UpdateFar(camera);
		UpdateFov(camera);
		
		UIEditor::GetInspectorWindow().CloseComponentNode();
	}

	void UINodeCamera::UpdateFov(Camera* camera)
	{
		UIEditor::GetInspectorWindow().SetLeftText("Fov");
		if (UI::GetUI()->AddFloat("##CameraFov", m_fov, true))
		{
			camera->GetCamera()->SetFov(m_fov);
		}
	}

	void UINodeCamera::UpdateFar(Camera* camera)
	{
		UIEditor::GetInspectorWindow().SetLeftText("Far");
		if (UI::GetUI()->AddFloat("##CameraFar", m_far, true))
		{
			camera->GetCamera()->SetFar(m_far);
		}
	}
}
