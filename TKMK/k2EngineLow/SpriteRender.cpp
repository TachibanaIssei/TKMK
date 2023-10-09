#include "k2EngineLowPreCompile.h"
#include "SpriteRender.h"

void nsK2EngineLow::SpriteRender::Init(const char* filePath, const float w, const float h, AlphaBlendMode alphaBlendMode)
{
	SpriteInitData initData;
	//DDS�t�@�C��(�摜�f�[�^)�̃t�@�C���p�X���w�肷��
	initData.m_ddsFilePath[0] = filePath;
	//Sprite�\���p�̃V�F�[�_�[�̃t�@�C���p�X���w�肷��
	initData.m_fxFilePath = "Assets/shader/sprite.fx";
	//�X�v���C�g�̕��ƍ������w�肷��
	initData.m_width = static_cast<UINT>(w);
	initData.m_height = static_cast<UINT>(h);
	//�X�v���C�g�̃u�����h���[�h��ݒ�
	initData.m_alphaBlendMode = alphaBlendMode;

	initData.m_expandConstantBuffer = &m_spriteCB;
	initData.m_expandConstantBufferSize = sizeof(m_spriteCB);

	//Sprite�������I�u�W�F�N�g���g�p���āASprite������������
	m_sprite.Init(initData);

	m_isInit = true;
}

void nsK2EngineLow::SpriteRender::Draw(RenderContext& rc, bool drawTiming, int viewportNo)
{
	if (viewportNo == -1)
	{
		if (drawTiming)
		{
			g_renderingEngine->AddSpriteList(this, true);
		}
		else {
			g_renderingEngine->AddSpriteList(this);
		}
	}
	else
	{
		g_renderingEngine->AddSpriteDrawViewportList(this, viewportNo);
	}
}

void nsK2EngineLow::SpriteRender::DrawFront(RenderContext& rc)
{
	g_renderingEngine->AddFrontSpriteList(this);
}
