#include "pch.h"

#include "Context/Context.h"

#include <chrono>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <unordered_map>

#include "Context/Surface.h"

#include "SwapChain/SwapChain.h"

#include "Light/Light.h"
#include "Shaders/ShaderStage.h"
#include "Windowing/Window.h"
#include "MeshImport/Mesh.h"
#include "DrawableObject/DrawableObject.h"
#include "Material/TextureManager.h"

#include "Tools/Initializers.h"
#include "ImGUI/imgui.h"
#include "Log/Logger.h"

#include "RenderPass/OffScreenRenderPass.h"

#include "Camera/CameraManager.h"
#include "DrawableObject/DrawableObjectsManager.h"
#include "UI/FontsManager.h"


namespace NIX::Rendering
{
    Context::Context(Window& window, const std::string& appName, const Version& appVersion, const std::string& engineName, const Version& engineVersion, const std::vector<const char*>& requiredExtension)
        : m_requiredExtension(requiredExtension)
    {
    	
        InitContext(appName, appVersion, engineName, engineVersion);

        m_debugInstance.SetupDebugMessenger(m_instance.GetVkInstance());

        m_surface.Initialize(window, m_instance.GetVkInstance());

        m_device.Initialize(m_instance.GetVkInstance(), m_surface, window);

        m_swapChain.Initialize(m_surface);

        m_renderPass.Initialize(m_swapChain);
        m_offScreenRenderPass.Initialize(m_swapChain);

        SetupDefaultPipeline();

        SetupDebugPhysicPipeline();

        m_commandPool.Initialize(m_surface);

        m_swapChain.CreateFrameBuffers(m_renderPass);

        m_textureSampler = new TextureSampler();
        m_textureSampler->Initialize(true);

        m_defaultCamera = new Camera(m_commandPool);
        m_currentCamera = m_defaultCamera;

        m_commandBuffers.Initialize(m_commandPool);

        m_synchronizationObjects.Initialize();

        m_textureManager = new TextureManager(&m_commandPool);
        m_lightsManager = new LightsManager(m_commandPool);
        m_cameraManager = new CameraManager(&m_commandPool);
        m_drawableObjectsManager = new DrawableObjectsManager(m_commandPool);

        m_UIManager.InitImGUI(m_instance.GetVkInstance(), m_swapChain, m_commandPool, m_renderPass, m_textureSampler);
          	
    }

    Context::~Context()
    {
        delete m_cameraManager;
        delete m_lightsManager;
        delete m_textureManager;

        delete m_drawableObjectsManager;

        delete m_textureSampler;

        delete m_defaultCamera;
    }

    void Context::InitContext(const std::string& appName, const Version& appVersion, const std::string& engineName, const Version& engineVersion)
    {
        VkApplicationInfo appInfo = Initializers::ApplicationInfo(appName, appVersion, engineName, engineVersion);

        m_instance.Initialize(appInfo, m_requiredExtension);
    }

    void Context::SetupDefaultPipeline()
    {
        // Setup default graphic pipeline

        m_defaultGraphicsPipeline.AddLayoutBinding(0, 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, ShaderStage::VERTEX | ShaderStage::FRAGMENT); // Camera
        m_defaultGraphicsPipeline.AddLayoutBinding(0, 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, ShaderStage::FRAGMENT); // Lights
        m_defaultGraphicsPipeline.AddLayoutBinding(0, 2, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, ShaderStage::FRAGMENT); // NbLights
        //TODO combine all texture in on material
        m_defaultGraphicsPipeline.AddLayoutBinding(1, 0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, ShaderStage::FRAGMENT); // Texture
        //m_defaultGraphicsPipeline.AddLayoutBinding(1,1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, EShaderStage::FRAGMENT); // Normal

        m_defaultGraphicsPipeline.AddLayoutBinding(2, 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, ShaderStage::VERTEX); // ModelComponent

        m_defaultGraphicsPipeline.InitializeDescriptorSetLayout();

        m_defaultGraphicsPipeline.SetBindingDescription(0, sizeof(GeometricTools::Vertex), VK_VERTEX_INPUT_RATE_VERTEX);


        m_defaultGraphicsPipeline.AddAttributeDescription(0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(GeometricTools::Vertex, position));
        m_defaultGraphicsPipeline.AddAttributeDescription(0, 1, VK_FORMAT_R32G32B32_SFLOAT, offsetof(GeometricTools::Vertex, normal));
        m_defaultGraphicsPipeline.AddAttributeDescription(0, 2, VK_FORMAT_R32G32_SFLOAT, offsetof(GeometricTools::Vertex, texCord));


        m_defaultGraphicsPipeline.SetPrimitiveTopology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
        m_defaultGraphicsPipeline.Initialize("Resources/Shaders/baseVert.spv", "Resources/Shaders/baseFrag.spv", nullptr, m_swapChain, m_offScreenRenderPass.GetVkRenderPass());
    }

    void Context::SetupDebugPhysicPipeline()
    {
        m_debugPhysicGraphicsPipeline.AddLayoutBinding(0, 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, ShaderStage::VERTEX); // Camera
        m_debugPhysicGraphicsPipeline.InitializeDescriptorSetLayout();
    	
        m_debugPhysicGraphicsPipeline.SetBindingDescription(0, sizeof(GeometricTools::Vertex), VK_VERTEX_INPUT_RATE_VERTEX);
    	
        m_debugPhysicGraphicsPipeline.AddAttributeDescription(0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(GeometricTools::Vertex, position));
        m_debugPhysicGraphicsPipeline.AddAttributeDescription(0, 1, VK_FORMAT_R32G32B32_SFLOAT, offsetof(GeometricTools::Vertex, normal));
        m_debugPhysicGraphicsPipeline.AddAttributeDescription(0, 2, VK_FORMAT_R32G32_SFLOAT, offsetof(GeometricTools::Vertex, texCord));

        m_debugPhysicGraphicsPipeline.SetPrimitiveTopology(VK_PRIMITIVE_TOPOLOGY_LINE_LIST);
        m_debugPhysicGraphicsPipeline.Initialize("Resources/Shaders/debugPhysicVert.spv", "Resources/Shaders/debugPhysicFrag.spv", nullptr, m_swapChain, m_offScreenRenderPass.GetVkRenderPass());
    }

    void Context::CleanupSwapChain()
    {
        m_swapChain.Cleanup();
        m_commandBuffers.Cleanup();

        m_defaultGraphicsPipeline.Cleanup();
        m_debugPhysicGraphicsPipeline.Cleanup();

        m_renderPass.Cleanup();

        m_swapChain.CleanupImageViews();

        m_swapChain.Destroy();
    	
        m_currentCamera->Cleanup();

    }

    void Context::StartFrame()
    {
        m_UIManager.PrepareNewFrame();

        vkWaitForFences(Device::GetSingleton()->GetVkLogicalDevice(), 1, &m_synchronizationObjects.GetInFlightFence(static_cast<uint32_t>(m_currentFrame)), VK_TRUE, UINT64_MAX);

        m_imageResult = vkAcquireNextImageKHR(Device::GetSingleton()->GetVkLogicalDevice(), m_swapChain.GetSwapChain(), UINT64_MAX, m_synchronizationObjects.GetImageAvailableSemaphore(static_cast<uint32_t>(m_currentFrame)), nullptr, &m_imageIndex);

    }

    void*& Context::GetFrameImGuiID()
    {
        return m_UIManager.GetTextureIds()[m_imageIndex];
    }

    void Context::DrawFrame()
    {
        ImGui::Render();

        if (m_imageResult == VK_ERROR_OUT_OF_DATE_KHR)
        {
            RecreateSwapChain();
            return;
        }
        else if (m_imageResult != VK_SUCCESS && m_imageResult != VK_SUBOPTIMAL_KHR)
        {
            ASSERT(SOURCE_INFO, "Failed to acquire swap chain image!");
        }

        // Check if a previous frame is using this image (i.e. there is its fence to wait on)
        if (m_synchronizationObjects.GetImageInFlight(m_imageIndex) != nullptr)
        {
            vkWaitForFences(Device::GetSingleton()->GetVkLogicalDevice(), 1, &m_synchronizationObjects.GetImageInFlight(static_cast<uint32_t>(m_imageIndex)), VK_TRUE, UINT64_MAX);
        }

        m_synchronizationObjects.MutateImageInFlight(m_imageIndex) = m_synchronizationObjects.GetInFlightFence(static_cast<uint32_t>(m_currentFrame));

        UpdateFrame(m_imageIndex);

        VkSubmitInfo submitInfo = Initializers::SubmitInfo();

        VkSemaphore waitSemaphores[] = { m_synchronizationObjects.GetImageAvailableSemaphore(static_cast<uint32_t>(m_currentFrame)) };
        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &m_commandBuffers.GetPtrCommandBuffer(m_imageIndex);

        VkSemaphore signalSemaphores[] = { m_synchronizationObjects.GetRenderFinishedSemaphore(static_cast<uint32_t>(m_currentFrame)) };
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        vkResetFences(Device::GetSingleton()->GetVkLogicalDevice(), 1, &m_synchronizationObjects.GetInFlightFence(static_cast<uint32_t>(m_currentFrame)));

        VkResult resultQueue = vkQueueSubmit(Device::GetSingleton()->GetGraphicsQueue(), 1, &submitInfo, m_synchronizationObjects.GetInFlightFence(static_cast<uint32_t>(m_currentFrame)));

        if (resultQueue != VK_SUCCESS)
        {
            ASSERT(SOURCE_INFO, "Failed to submit draw command buffer!");
        }

        VkPresentInfoKHR presentInfo = Initializers::PresentInfo();

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = { m_swapChain.GetSwapChain() };
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &m_imageIndex;

        m_imageResult = vkQueuePresentKHR(Device::GetSingleton()->GetPresentQueue(), &presentInfo);

        if (m_imageResult == VK_ERROR_OUT_OF_DATE_KHR || m_imageResult == VK_SUBOPTIMAL_KHR || Device::GetSingleton()->IsWindowResized())
        {
            Device::GetSingleton()->SetIsWindowResized(false);
            RecreateSwapChain();
        }
        else if (m_imageResult != VK_SUCCESS)
        {
            ASSERT(SOURCE_INFO, "Failed to present swap chain image!");
        }
    	
        m_currentFrame = (m_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

       
    }

    DrawableObject* Context::AddDrawableObject(const GeometricTools::Mesh* mesh, Material* material)
    {
        if (mesh == nullptr)
        {
            return nullptr;
        }

        DrawableObject* drawable = m_drawableObjectsManager->AddDrawableObject(*mesh, material, &m_defaultGraphicsPipeline);

        m_isBufferChanges[0] = true;
        m_isBufferChanges[1] = true;
        m_isBufferChanges[2] = true;

        return drawable;
    }

    void Context::DestroyDrawableObject(DrawableObject* drawableObject)
    {
        m_drawableObjectsManager->DestroyDrawableObject(drawableObject);
    }

    DrawableObject* Context::AddDebugDrawableObject(const std::vector<LibMath::Vector3>& points)
    {
        DrawableObject* drawable = m_drawableObjectsManager->AddDrawableObject(points, &m_debugPhysicGraphicsPipeline);
    	
        m_isBufferChanges[0] = true;
        m_isBufferChanges[1] = true;
        m_isBufferChanges[2] = true;
    	
        return drawable;
    }

    LightsManager* Context::GetLightsManager()
    {
        return m_lightsManager;
    }

    CameraManager* Context::GetCameraManager()
    {
        return m_cameraManager;
    }


    Camera* Context::GetCurrentCamera()
    {
        return m_currentCamera;
    }

    void Context::SetCurrentCamera(Camera* camera)
    {
        m_currentCamera = camera;
    }

    void Context::Shutdown()
    {
        vkDeviceWaitIdle(Device::GetSingleton()->GetVkLogicalDevice());
    }

    const TextureSampler& Context::GetTextureSampler()
    {
        return *m_textureSampler;
    }

    TextureManager* Context::GetTextureManager()
    {    	
        return m_textureManager;
    }

    void Context::RecreateSwapChain()
	{

        vkDeviceWaitIdle(Device::GetSingleton()->GetVkLogicalDevice());

        CleanupSwapChain();

        m_swapChain.Initialize(m_surface);

        m_renderPass.Initialize(m_swapChain);

        m_defaultGraphicsPipeline.Initialize("Resources/Shaders/baseVert.spv", "Resources/Shaders/baseFrag.spv", nullptr, m_swapChain, m_offScreenRenderPass.GetVkRenderPass());
        m_debugPhysicGraphicsPipeline.Initialize("Resources/Shaders/debugPhysicVert.spv", "Resources/Shaders/debugPhysicFrag.spv", nullptr, m_swapChain, m_offScreenRenderPass.GetVkRenderPass());
        m_swapChain.CreateFrameBuffers(m_renderPass);
        m_currentCamera->CreateCameraBuffer(m_commandPool);
        m_commandBuffers.Initialize(m_commandPool);
    }

    void Context::UpdateOffscreenRenderSceneWindowSize(const float width, const float height)
    {
        m_offScreenRenderPass.UpdateSceneWindowSize(width, height);
    }

    void Context::UpdateFrame(uint32_t currentImage)
    {
        m_lightsManager->UpdateLights(currentImage);

        m_defaultGraphicsPipeline.GetDescriptorSets().SetIsUpdate(currentImage, false);
        m_debugPhysicGraphicsPipeline.GetDescriptorSets().SetIsUpdate(currentImage, false);
         	
        if (m_isBufferChanges[currentImage])
        {
            m_commandBuffers.StartUpdate(currentImage);

            m_offScreenRenderPass.Update(currentImage, m_commandBuffers.GetVkCommandBuffer(m_imageIndex), m_UIManager, m_drawableObjectsManager->GetDrawableObjects(), *m_currentCamera,
                m_lightsManager->GetLightsBuffers(), m_lightsManager->GetNbOfLightsBuffers(), *m_textureSampler);

            m_renderPass.Update(currentImage, m_commandBuffers.GetVkCommandBuffer(m_imageIndex), m_swapChain, m_defaultGraphicsPipeline, m_UIManager);

            m_commandBuffers.EndUpdate(currentImage);
        }

    }

}
