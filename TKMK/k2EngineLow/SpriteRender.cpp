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

	//Sprite�������I�u�W�F�N�g���g�p���āASprite������������
	m_sprite.Init(initData);

}

void nsK2EngineLow::SpriteRender::Draw(RenderContext& rc)
{
	m_sprite.Draw(rc);
}