#pragma once
#include <Component.h>

#define CAMERA_ELEMENT "camera"
#define CAMERA_FAR_ATTRIBUTE "far"
#define CAMERA_FOV_ATTRIBUTE "fov"
#define CAMERA_SPEED_ATTRIBUTE "speed"
#define CAMERA_ZOOM_SENSITIVITY_ATTRIBUTE "zoom_sensitivity"
#define CAMERA_YAW_ATTRIBUTE "yaw"
#define CAMERA_PITCH_ATTRIBUTE "pitch"

namespace NIX
{
	namespace Rendering
	{
		class Camera;
	}

	namespace Editor
	{
		class Camera : public Component
		{
		private:
			Rendering::Camera* m_camera;

		public:
			Camera(GameObject* gameObject);
			Camera(GameObject* gameObject, const Camera& other);
			~Camera();

			[[nodiscard]] Rendering::Camera* GetCamera() const;

			void Serialize(Core::SaveElement& parent) override;

			void Deserialize(const Core::SaveElement& data) override;

			void SetIsActive(bool value) override;

			void SetPosRot(LibMath::Vector3 pos, LibMath::Vector3 front);

			void SetFov(float fov);

			void SetFar(float far);

			static void LuaInit();

		private:

			[[nodiscard]] float GetFov() const;
			[[nodiscard]] float GetFar() const;
		};
	}
}
