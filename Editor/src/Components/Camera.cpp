#include "Components/Camera.h"

#include "EditorApp.h"
#include "Log/Logger.h"

#include "Camera/CameraManager.h"

#include "Scripting/ScriptManager.h"

namespace NIX::Editor
{
	Camera::Camera(GameObject* gameObject)
		: Component(gameObject)
	{
		m_camera = EditorApp::GetRenderingContext().GetCameraManager()->AddCamera();
	}

	Camera::Camera(GameObject* gameObject, const Camera& other)
		: Component(gameObject)
	{
		m_camera = EditorApp::GetRenderingContext().GetCameraManager()->AddCamera(other.GetCamera());

		SetIsActive(gameObject->IsActive());
	}

	Camera::~Camera()
	{
		EditorApp::GetRenderingContext().GetCameraManager()->DestroyCamera(m_camera);
	}

	Rendering::Camera* Camera::GetCamera() const
	{
		return m_camera;
	}

	void Camera::Serialize(Core::SaveElement& parent)
	{
		if (m_camera == nullptr)
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_WARNING, "Could not save camera component, camera was nullptr");
			return;
		}
		
		Core::SaveElement modelElem(parent, "camera");

		modelElem.AddFloatAttribute(CAMERA_FAR_ATTRIBUTE, m_camera->GetFar());
		modelElem.AddFloatAttribute(CAMERA_FOV_ATTRIBUTE, m_camera->GetFov());
		modelElem.AddFloatAttribute(CAMERA_SPEED_ATTRIBUTE, m_camera->GetCameraSpeed());
		modelElem.AddFloatAttribute(CAMERA_ZOOM_SENSITIVITY_ATTRIBUTE, m_camera->GetZoomSensitivity());
		modelElem.AddFloatAttribute(CAMERA_YAW_ATTRIBUTE, m_camera->GetYaw());
		modelElem.AddFloatAttribute(CAMERA_PITCH_ATTRIBUTE, m_camera->GetPitch());
	}

	void Camera::Deserialize(const Core::SaveElement& data)
	{
		if (m_camera == nullptr)
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_WARNING, "Could not load camera component, camera was nullptr");
			return;
		}
		
		m_camera->SetFar(data.GetFloatAttribute(CAMERA_FAR_ATTRIBUTE));
		m_camera->SetFov(data.GetFloatAttribute(CAMERA_FOV_ATTRIBUTE));
		m_camera->SetCameraSpeed(data.GetFloatAttribute(CAMERA_SPEED_ATTRIBUTE));
		m_camera->SetZoomSensitivity(data.GetFloatAttribute(CAMERA_ZOOM_SENSITIVITY_ATTRIBUTE));
		m_camera->SetYaw(data.GetFloatAttribute(CAMERA_YAW_ATTRIBUTE));
		m_camera->SetPitch(data.GetFloatAttribute(CAMERA_PITCH_ATTRIBUTE));
	}

	void Camera::SetIsActive(bool value)
	{
		m_isActive = value;
	}

	void Camera::SetPosRot(LibMath::Vector3 position, LibMath::Vector3 front)
	{
		m_camera->SetPosRot(position, front);
	}

	void Camera::SetFov(float fov)
	{
		if (m_camera == nullptr)
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_ERROR, "Camera is nullptr in Camera component");
			return;
		}
		
		m_camera->SetFov(fov);
	}

	void Camera::SetFar(float far)
	{
		if (m_camera == nullptr)
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_ERROR, "Camera is nullptr in Camera component");
			return;
		}

		m_camera->UpdateCameraFar(far);
	}

	void Camera::LuaInit()
	{
		ScriptManager::GetSingleton()->AddClass<Camera, GameObject*>("Camera");

		ScriptManager::GetSingleton()->AddClassProperty<Camera>("Camera", "IsActive", &Camera::GetIsActive, &Camera::SetIsActive);
		ScriptManager::GetSingleton()->AddClassData<Camera>("Camera", "GameObject", &Camera::m_gameObject, false);
		ScriptManager::GetSingleton()->AddClassFunction<Camera>("Camera", "GetGameObject", &Camera::GetParent);

		ScriptManager::GetSingleton()->AddClassProperty<Camera>("Camera", "Fov", &Camera::GetFov, &Camera::SetFov);
		ScriptManager::GetSingleton()->AddClassProperty<Camera>("Camera", "Far", &Camera::GetFar, &Camera::SetFar);
	}

	float Camera::GetFov() const
	{
		return GetCamera()->GetFov();
	}

	float Camera::GetFar() const
	{
		return GetCamera()->GetFar();
	}
}
