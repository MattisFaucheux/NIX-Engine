#pragma once

#include "API/DLLExportRendering.h"

#include <vulkan/vulkan_core.h>
#include <string>
#include <vector>

#include "Camera.h"

#include "CommandBuffers.h"
#include "CommandPool.h"
#include "Debug/Debug.h"
#include "GraphicsPipeline.h"
#include "RenderPass/RenderPass.h"
#include "Tools/Version.h"
#include "Context/Surface.h"
#include "SwapChain/SwapChain.h"
#include "Material/Texture.h"
#include "Material/TextureSampler.h"
#include "SynchronizationObjects.h"
#include "Context/Device/Device.h"
#include "Matrix/Matrix4.h"
#include "DrawableObject/DrawableObject.h"
#include "UI/UIManager.h"
#include "RenderPass/OffScreenRenderPass.h"
#include "Light/LightsManager.h"

#include <array>

#include "Instance.h"

namespace NIX::Rendering
{
	class FontsManager;
	class DrawableObjectsManager;
	class CameraManager;

	struct MatrixModel
	{
		LibMath::Matrix4 model = LibMath::Matrix4(1.0f);
	};

	
}

namespace NIX::Rendering
{	
	class TextureManager;
	class Window;
	
	class Context
	{
	public:
		RENDERING_LIBRARY_API Context(Window& window, const std::string& appName, const Version& appVersion, const std::string& engineName, const Version& engineVersion, const std::vector<const char*>& requiredExtension);
		RENDERING_LIBRARY_API ~Context();

		RENDERING_LIBRARY_API void StartFrame();
		RENDERING_LIBRARY_API void*& GetFrameImGuiID();
		RENDERING_LIBRARY_API void DrawFrame();

		RENDERING_LIBRARY_API DrawableObject* AddDrawableObject(const GeometricTools::Mesh* mesh, Material* material);
		RENDERING_LIBRARY_API void DestroyDrawableObject(DrawableObject* drawableObject);
		RENDERING_LIBRARY_API DrawableObject* AddDebugDrawableObject(const std::vector<LibMath::Vector3>& points);

		RENDERING_LIBRARY_API LightsManager* GetLightsManager();
		RENDERING_LIBRARY_API CameraManager* GetCameraManager();
		
		
		RENDERING_LIBRARY_API Camera* GetCurrentCamera();
		RENDERING_LIBRARY_API void SetCurrentCamera(Camera* camera);

		RENDERING_LIBRARY_API void Shutdown();

		static const TextureSampler& GetTextureSampler();
		static TextureManager* GetTextureManager();

		RENDERING_LIBRARY_API void UpdateOffscreenRenderSceneWindowSize(const float width, const float height);
		
	private:
		void InitContext(const std::string& appName, const Version& appVersion, const std::string& engineName, const Version& engineVersion);

		void SetupDefaultPipeline();

		void SetupDebugPhysicPipeline();
		
		void CleanupSwapChain();
		void RecreateSwapChain();
				
		std::vector<const char*> m_requiredExtension;
		
		Instance m_instance;
		Debug m_debugInstance;
		Surface m_surface;
		Device m_device;
		SwapChain m_swapChain;
		Texture m_depthImage;
		TextureView m_depthImageView;
		RenderPass m_renderPass;
		OffScreenRenderPass m_offScreenRenderPass;
		
	    GraphicsPipeline m_defaultGraphicsPipeline;
		GraphicsPipeline m_debugPhysicGraphicsPipeline;
		
		CommandPool m_commandPool;
		
		inline static TextureSampler* m_textureSampler = nullptr;

		Camera* m_defaultCamera = nullptr;
		Camera* m_currentCamera = nullptr;
		std::array<bool, 3> m_isBufferChanges = { true, true, true };
				
		CommandBuffers  m_commandBuffers;
		SynchronizationObjects m_synchronizationObjects;

		inline static TextureManager* m_textureManager = nullptr;

		inline static DrawableObjectsManager* m_drawableObjectsManager = nullptr;
		
		size_t m_currentFrame = 0;

		UIManager m_UIManager;

		VkResult m_imageResult;
		uint32_t m_imageIndex;

		LightsManager* m_lightsManager = nullptr;
		CameraManager* m_cameraManager = nullptr;

		void UpdateFrame(uint32_t currentImage);
	};
	
}
