#include "pch.h"

#include "Camera.h"


#include "Context/Context.h"
#include "SwapChain/SwapChain.h"
#include "Memory/EBufferType.h"

#include "Vector/Vector3.h"
#include "Core/Angle.h"
#include "Material/TextureManager.h"

#include "Matrix/Matrix4.h"

namespace NIX::Rendering
{
	Camera::Camera(const CommandPool& commandPool)
	{
        CreateCameraBuffer(commandPool);
	}

	void Camera::MoveCameraForward(float deltaTime)
	{
        m_cameraPos += deltaTime * m_cameraSpeed * m_cameraFront;
        SetAllIsCameraChange(true);
	}

	void Camera::MoveCameraBackward(float deltaTime)
	{
        m_cameraPos -= deltaTime * m_cameraSpeed * m_cameraFront;
        SetAllIsCameraChange(true);
	}

	void Camera::MoveCameraLeft(float deltaTime)
	{
        m_cameraPos -= deltaTime * m_cameraSpeed * m_cameraUp.Cross(m_cameraFront).Normalize();
        SetAllIsCameraChange(true);
	}

	void Camera::MoveCameraRight(float deltaTime)
	{
        m_cameraPos += deltaTime * m_cameraSpeed * m_cameraUp.Cross(m_cameraFront).Normalize();
        SetAllIsCameraChange(true);
	}

	void Camera::UpdateDirection(float xPos, float yPos)
	{
        if (m_firstMouse)
        {
            m_lastXPos = xPos;
            m_lastYPos = yPos;
            m_firstMouse = false;
        }
		
        float xoffset = m_lastXPos - xPos;
        float yoffset = m_lastYPos - yPos;

        m_lastXPos = xPos;
        m_lastYPos = yPos;

        xoffset *= m_sensitivity;
        yoffset *= m_sensitivity;

        m_yaw += xoffset;
        m_pitch += yoffset;

        if (m_pitch > 89.0f)
        {
            m_pitch = 89.0f;
        }
            
        if (m_pitch < -89.0f)
        {
            m_pitch = -89.0f;
        }

        LibMath::Vector3 direction;
        direction.x = LibMath::cos(LibMath::Deg(m_yaw)) * LibMath::cos(LibMath::Deg(m_pitch));
        direction.y = LibMath::sin(LibMath::Deg(m_pitch));
        direction.z = LibMath::sin(LibMath::Deg(m_yaw)) * LibMath::cos(LibMath::Deg(m_pitch));
        m_cameraFront = direction.Normalize();

        SetAllIsCameraChange(true);
	}

	void Camera::UpdateFov(float yOffset)
	{
        m_fov -= yOffset * m_zoomSensitivity;
		
        if (m_fov < 1.0f)
        {
            m_fov = 1.0f;
        }
           
        if (m_fov > 45.0f)
        {
            m_fov = 45.0f;
        }

        SetAllIsCameraChange(true);
	}

	void Camera::UpdateCameraFar(float pCameraFar)
	{
        m_cameraFar = pCameraFar;
        SetAllIsCameraChange(true);
	}

	void Camera::SetPosRot(LibMath::Vector3 pos, LibMath::Vector3 front)
	{
		m_cameraPos = pos;
		m_cameraFront = front;
	}

	void Camera::SetCameraSpeed(float speed)
	{
        m_cameraSpeed = speed;
	}

	void Camera::SetFar(float cameraFar)
	{
        m_cameraFar = cameraFar;
	}

	void Camera::SetFov(float fov)
	{
        m_fov = fov;
	}

	void Camera::SetSpeed(float speed)
	{
        m_cameraSpeed = speed;
	}

	void Camera::SetZoomSensitivity(float zoomSensitivity)
	{
		m_sensitivity = zoomSensitivity;
	}

	void Camera::SetYaw(float yaw)
	{
		m_yaw = yaw;
	}

	void Camera::SetPitch(float pitch)
	{
		m_pitch = pitch;
	}

	void Camera::SetFirstMouse(bool value)
	{
        m_firstMouse = value;
	}

	float Camera::GetFar() const
	{
        return m_cameraFar;
	}

	float Camera::GetFov() const
	{
        return m_fov;
	}

	float Camera::GetCameraSpeed() const
	{
        return m_cameraSpeed;
	}

	float Camera::GetZoomSensitivity() const
	{
        return m_sensitivity;
	}

	float Camera::GetYaw() const
	{
        return m_yaw;
	}

	float Camera::GetPitch() const
	{
        return m_pitch;
	}

	void Camera::Cleanup()
	{
		for (auto& buffer : m_cameraBuffers)
		{
            buffer.Cleanup();
		}
	}

	void Camera::CreateCameraBuffer(const CommandPool& commandPool)
    {
        const VkDeviceSize  cameraBufferSize = sizeof(CameraUBO);
    	
        m_cameraBuffers.resize(MIN_IMAGE_COUNT);
        m_isCameraChange.resize(MIN_IMAGE_COUNT, true);
		
        for (size_t i = 0; i < MIN_IMAGE_COUNT; i++)
        {
            m_cameraBuffers[i].Initialize(commandPool, cameraBufferSize, m_cameraBuffers.data(), EBufferType::UNIFORM);
        }
    }

    void Camera::UpdateCameraBuffer(uint32_t imageIndex, float width, float height)
    {
		if (!m_isCameraChange[imageIndex])
		{
            return;
		}
		
        m_cameraUBO.view = LibMath::Matrix4::LookAtLh(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);

        m_cameraUBO.proj = LibMath::Matrix4::Perspective(LibMath::Degree(m_fov), width / height, 0.1f, m_cameraFar);

        m_cameraUBO.proj[1][1] *= -1;

        m_cameraUBO.position = m_cameraPos;

        m_cameraBuffers[imageIndex].Update(sizeof(m_cameraUBO), &m_cameraUBO);
    }

	const Buffer& Camera::GetBuffer(uint32_t imageIndex)
	{
        return m_cameraBuffers[imageIndex];
	}

	void Camera::SetAllIsCameraChange(bool value)
	{
		for (bool&& change : m_isCameraChange)
		{
            change = value;
		}
	}
}
