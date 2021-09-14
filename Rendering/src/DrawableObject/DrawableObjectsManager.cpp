#include "pch.h"

#include "DrawableObject/DrawableObjectsManager.h"

#include "Context/Device/Device.h"
#include "DrawableObject/DrawableObject.h"
#include "Log/Logger.h"

namespace NIX::Rendering
{
	DrawableObjectsManager::DrawableObjectsManager(CommandPool& commandPool)
		: m_commandPool(&commandPool)
	{}

	DrawableObjectsManager::~DrawableObjectsManager()
	{
		for (auto* drawableObject : m_drawableObjects)
		{
			delete drawableObject;
		}
	}

	DrawableObject* DrawableObjectsManager::AddDrawableObject(const GeometricTools::Mesh& mesh, Material* material, GraphicsPipeline* graphicsPipeline)
	{
		vkDeviceWaitIdle(Device::GetSingleton()->GetVkLogicalDevice());
		
		DrawableObject* drawable = new DrawableObject(*m_commandPool, mesh, material, graphicsPipeline);
		if (drawable == nullptr)
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_ERROR, "Failed to create drawable object");
			return nullptr;
		}
		
		m_drawableObjects.emplace_back(drawable);

		return drawable;
	}

	DrawableObject* DrawableObjectsManager::AddDrawableObject(const std::vector<LibMath::Vector3>& points, GraphicsPipeline* graphicsPipeline)
	{
		vkDeviceWaitIdle(Device::GetSingleton()->GetVkLogicalDevice());
		
		DrawableObject* drawable = new DrawableObject(*m_commandPool, points, graphicsPipeline);
		if (drawable == nullptr)
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_ERROR, "Failed to create drawable object");
			return nullptr;
		}

		m_drawableObjects.emplace_back(drawable);

		return drawable;
	}

	void DrawableObjectsManager::DestroyDrawableObject(DrawableObject* drawableObject)
	{
		m_drawableObjects.erase(std::remove_if(m_drawableObjects.begin(), m_drawableObjects.end(), [&drawableObject](DrawableObject* i) { return i && (i == drawableObject); }));
		delete drawableObject;
	}

	std::vector<DrawableObject*>& DrawableObjectsManager::GetDrawableObjects()
	{
		return m_drawableObjects;
	}
}
