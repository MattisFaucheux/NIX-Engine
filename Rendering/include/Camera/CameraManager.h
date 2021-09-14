#pragma once
#include "Camera.h"

#include "API/DLLExportRendering.h"

namespace NIX::Rendering
{
	class CameraManager
	{
	private:
		std::vector<Camera*> m_cameras;

		CommandPool* m_commandPool = nullptr;
	public:
		CameraManager() = delete;
		explicit CameraManager(CommandPool* commandPool);
		
		~CameraManager();

		RENDERING_LIBRARY_API Camera* AddCamera();
		RENDERING_LIBRARY_API Camera* AddCamera(Camera* other);
		RENDERING_LIBRARY_API void DestroyCamera(Camera* camera);
	};
}
