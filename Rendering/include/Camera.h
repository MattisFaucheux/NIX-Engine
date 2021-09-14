#pragma once

#include "API/DLLExportRendering.h"

#include "Memory/Buffer.h"

#include "Vector/Vector3.h"
#include "Matrix/Matrix4.h"

#include <vector>

namespace NIX::Rendering
{
	class SwapChain;

	struct CameraUBO
	{
		LibMath::Matrix4 view;
		LibMath::Matrix4 proj;
		LibMath::Vector3 position;
	};

	class Camera
	{
	private:
		std::vector<bool> m_isCameraChange;
		
		LibMath::Vector3 m_cameraPos = { 0.0f, 0.0f, -10.0f };
		LibMath::Vector3 m_cameraUp = { 0.0f, 1.0f, 0.0f };
		LibMath::Vector3 m_cameraFront = { 0.0f, 0.0f, 1.0f };
		float m_cameraFar = 1000.0f;
		float m_fov = 45.0f;

		float m_cameraSpeed = 10.0f;
		float m_sensitivity = 0.1f;

		float m_zoomSensitivity = 3.0f;

		float m_yaw = 90.0f;
		float m_pitch = 0.0f;
		float m_lastXPos = 0;
		float m_lastYPos = 0;
		bool m_firstMouse = true;
		
		std::vector<Buffer> m_cameraBuffers;

	public:
		Camera(const CommandPool& commandPool);
		RENDERING_LIBRARY_API ~Camera() = default;
		
		RENDERING_LIBRARY_API void MoveCameraForward(float deltaTime);
		RENDERING_LIBRARY_API void MoveCameraBackward(float deltaTime);
		RENDERING_LIBRARY_API void MoveCameraLeft(float deltaTime);
		RENDERING_LIBRARY_API void MoveCameraRight(float deltaTime);
		RENDERING_LIBRARY_API void UpdateDirection(float xPos, float yPos);
		RENDERING_LIBRARY_API void UpdateFov(float yOffset);
		RENDERING_LIBRARY_API void UpdateCameraFar(float pCameraFar);

		RENDERING_LIBRARY_API void SetPosRot(LibMath::Vector3 pos, LibMath::Vector3 front);
		RENDERING_LIBRARY_API void SetCameraSpeed(float speed);

		RENDERING_LIBRARY_API void SetFar(float cameraFar);
		RENDERING_LIBRARY_API void SetFov(float fov);
		RENDERING_LIBRARY_API void SetSpeed(float speed);
		RENDERING_LIBRARY_API void SetZoomSensitivity(float zoomSensitivity);
		RENDERING_LIBRARY_API void SetYaw(float yaw);
		RENDERING_LIBRARY_API void SetPitch(float pitch);
		
		RENDERING_LIBRARY_API void SetFirstMouse(bool value);

		RENDERING_LIBRARY_API [[nodiscard]] float GetFar() const;
		RENDERING_LIBRARY_API [[nodiscard]] float GetFov() const;
		RENDERING_LIBRARY_API [[nodiscard]] float GetCameraSpeed() const;
		RENDERING_LIBRARY_API [[nodiscard]] float GetZoomSensitivity() const;
		RENDERING_LIBRARY_API [[nodiscard]] float GetYaw() const;
		RENDERING_LIBRARY_API [[nodiscard]] float GetPitch() const;
		
		void Cleanup();

		void CreateCameraBuffer(const CommandPool& commandPool);
		void UpdateCameraBuffer(uint32_t imageIndex, float width, float height);

		[[nodiscard]] const Buffer& GetBuffer(uint32_t imageIndex);

	private:
		CameraUBO m_cameraUBO;

		void SetAllIsCameraChange(bool value);
	};
}


