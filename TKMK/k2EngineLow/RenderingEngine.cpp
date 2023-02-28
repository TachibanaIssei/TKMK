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

void nsK2EngineLow::RenderingEngine::FontRendering(RenderContext& rc)
{
	for (auto& fontObj : m_fontList)
	{
		fontObj->OnRenderFont(rc);
	}
}

void nsK2EngineLow::RenderingEngine::Execute(RenderContext& rc)
{
	ModelRendering(rc);
	SpriteRendering(rc);
	FontRendering(rc);

	m_modelList.clear();
	m_spriteList.clear();
	m_fontList.clear();
}

void nsK2EngineLow::RenderingEngine::MakeDirectionLight()
{
	//���C�g�̕���
	m_directionLight.lightDirection = m_directionLightDir;
	//���K������
	m_directionLight.lightDirection.Normalize();
	//���C�g�̐F
	m_directionLight.ligColor = m_directionLightColor;
	//���_�̈ʒu��ݒ�
	m_directionLight.CameraEyePos = g_camera3D->GetPosition();
}
