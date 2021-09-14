#include "pch.h"

#include "Camera/CameraManager.h"

#include "Log/Logger.h"

namespace NIX::Rendering
{
	CameraManager::CameraManager(CommandPool* commandPool)
	{
		if (commandPool == nullptr)
		{
			ASSERT(SOURCE_INFO, "Command pool sent to Camera manager is null");
		}
		
		m_commandPool = commandPool;
	}

	CameraManager::~CameraManager()
	{
		for (Camera* cam : m_cameras)
		{
			delete cam;
		}
	}

	Camera* CameraManager::AddCamera()
	{
		Camera* cam = new Camera(*m_commandPool);
		if (cam == nullptr)
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_WARNING, "Failed to add camera to camera manager");
		}
		
		m_cameras.emplace_back(cam);

		return cam;
	}

	Camera* CameraManager::AddCamera(Camera* other)
	{
		Camera* cam = new Camera(*m_commandPool);
		if (cam == nullptr)
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_WARNING, "Failed to add camera to camera manager");
		}

		cam->SetFar(other->GetFar());
		cam->SetFov(other->GetFov());
		cam->SetCameraSpeed(other->GetCameraSpeed());
		cam->SetZoomSensitivity(other->GetZoomSensitivity());
		cam->SetYaw(other->GetYaw());
		cam->SetPitch(other->GetPitch());

		m_cameras.emplace_back(cam);
		
		return cam;
	}

	void CameraManager::DestroyCamera(Camera* camera)
	{
		m_cameras.erase(std::remove_if(m_cameras.begin(), m_cameras.end(), [&camera](Camera* i) { return i && (i == camera); }));
		delete camera;
	}
}
