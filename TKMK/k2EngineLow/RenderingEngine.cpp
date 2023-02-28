#include "k2EngineLowPreCompile.h"
#include "RenderingEngine.h"

void nsK2EngineLow::RenderingEngine::ModelRendering(RenderContext& rc)
{
	for (auto& modelObj : m_modelList) {
		modelObj->OnRenderModel(rc);
	}
}

void nsK2EngineLow::RenderingEngine::SpriteRendering(RenderContext& rc)
{
	for (auto& spriteObj : m_spriteList)
	{
		spriteObj->OnRenderSprite(rc);
	}
}

void nsK2EngineLow::RenderingEngine::Execute(RenderContext& rc)
{
	ModelRendering(rc);
	SpriteRendering(rc);

	m_modelList.clear();
	m_spriteList.clear();
}
