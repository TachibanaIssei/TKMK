#pragma once

namespace nsK2EngineLow {
	class ModelRender;
	class SpriteRender;
	class FontRender;
	class RenderContext;

	struct DirectionalLight
	{
		Vector3 lightDirection;	//���C�g�̕���
		float pad0;
		Vector3 ligColor;		//���C�g�̃J���[
		float pad1;
		Vector3 CameraEyePos;	//�J�������W
	};

	class RenderingEngine
	{
	public:
		/// <summary>
		/// ���f�������_�[�N���X�����X�g�ɒǉ�����
		/// </summary>
		/// <param name="modelRender">���f�������_�[</param>
		void AddModelList(ModelRender* modelRender)
		{
			m_modelList.push_back(modelRender);
		}
		/// <summary>
		/// �X�v���C�g�����_�[�N���X�����X�g�ɒǉ�����
		/// </summary>
		/// <param name="spriteRender">�X�v���C�g�����_�[</param>
		void AddSpriteList(SpriteRender* spriteRender)
		{
			m_spriteList.push_back(spriteRender);
		}
		/// <summary>
		/// �t�H���g�����_�[�N���X�����X�g�ɒǉ�����
		/// </summary>
		/// <param name="fontRender">�t�H���g�����_�[</param>
		void AddFontList(FontRender* fontRender)
		{
			m_fontList.push_back(fontRender);
		}

		/// <summary>
		/// ���f����`�悷��
		/// </summary>
		/// <param name="rc">�����_�[�R���e�L�X�g</param>
		void ModelRendering(RenderContext& rc);
		/// <summary>
		/// �X�v���C�g��`�悷��
		/// </summary>
		/// <param name="rc">�����_�[�R���e�L�X�g</param>
		void SpriteRendering(RenderContext& rc);
		/// <summary>
		/// �t�H���g��`�悷��
		/// </summary>
		/// <param name="rc">�����_�[�R���e�L�X�g</param>
		void FontRendering(RenderContext& rc);

		/// <summary>
		/// �`�揈�������s
		/// </summary>
		/// <param name="rc">�����_�[�R���e�L�X�g</param>
		void Execute(RenderContext& rc);

		/// <summary>
		/// �f�B���N�V�������C�g���擾����
		/// </summary>
		/// <returns>�f�B���N�V�������C�g</returns>
		DirectionalLight* GetDirectionalLight()
		{
			return &m_directionLight;
		}

		/// <summary>
		/// �f�B���N�V�������C�g��ݒ肷��
		/// </summary>
		/// <param name="dir">���C�g�̕���</param>
		/// <param name="color">���C�g�̐F</param>
		void SetDirectionLight(Vector3 dir,Vector3 color = {0.5f,0.5f,0.5f})
		{
			m_directionLightDir = dir;
			
			m_directionLightColor = color;
		}
		

		void MakeDirectionLight();

	private:
		std::vector<ModelRender*>	m_modelList;											//���f���N���X�̃��X�g
		std::vector<SpriteRender*>	m_spriteList;											//�X�v���C�g�N���X�̃��X�g
		std::vector<FontRender*>	m_fontList;												//�t�H���g�N���X�̃��X�g

		DirectionalLight			m_directionLight;										//�f�B���N�V�������C�g
		Vector3						m_directionLightDir		= Vector3{ 1.0f,-1.0f,-1.0f };	//�f�B���N�V�������C�g�̕���
		Vector3						m_directionLightColor	= Vector3{0.5f,0.5f,0.5f};		//�f�B���N�V�������C�g�̐F
	};
}