#pragma once

#include "Light.h"

namespace nsK2EngineLow {
	class ModelRender;
	class SpriteRender;
	class FontRender;
	class RenderContext;

	class RenderingEngine
	{
	public:
		void Init();

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

		SceneLight& GetSceneLight()
		{
			return m_sceneLight;
		}

	////////////////////////////////////////////////////////////////////////////////
	///�f�B���N�V�������C�g�̊֐�
	////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// �f�B���N�V�������C�g��ݒ�
		/// </summary>
		/// <param name="lightNo">���C�g�ԍ�</param>
		/// <param name="direction">���C�g����</param>
		/// <param name="color">���C�g�F</param>
		void SetDirectionLight(int lightNo, Vector3 direction, Vector4 color)
		{
			m_sceneLight.SetDirectionLight(lightNo, direction, color);
		}
		/// <summary>
		/// �f�B���N�V�������C�g�̌��̕�����ݒ肷��
		/// </summary>
		/// <param name="direction">����</param>
		void SetDirLightDirection(Vector3 direction)
		{
			m_sceneLight.SetDirLightDirection(direction);
		}
		/// <summary>
		/// �f�B���N�V�������C�g�̌��̐F��ݒ肷��
		/// </summary>
		/// <param name="color">�F</param>
		void SetDirLightColor(Vector4 color)
		{
			m_sceneLight.SetDirLightColor(color);
		}

		/// <summary>
		/// �f�B���N�V�������C�g�̌��̕������擾����
		/// </summary>
		/// <returns>���̕���</returns>
		const Vector3& GetDirLigDirection() const
		{
			return m_sceneLight.GetDirLigDirection();
		}
		/// <summary>
		/// �f�B���N�V�������C�g�̌��̐F���擾����
		/// </summary>
		/// <returns>�F</returns>
		const Vector4& GetDirLigColor() const
		{
			return m_sceneLight.GetDirLigColor();
		}

	////////////////////////////////////////////////////////////////////////////////
	///�����̊֐�
	////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// ������ݒ�
		/// </summary>
		/// <param name="ambient">����</param>
		void SetAmbient(Vector4 ambient)
		{
			m_sceneLight.SetAmbient(ambient);
		}

	////////////////////////////////////////////////////////////////////////////////////////
	///�J�����̈ʒu�̊֐�
	////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// �J�����̈ʒu��ݒ肷��
		/// </summary>
		/// <param name="eyePos"></param>
		void SetEyePos(Vector3 eyePos)
		{
			m_sceneLight.SetEyePos(eyePos);
		}

	////////////////////////////////////////////////////////////////////////////////
	///�|�C���g���C�g�̊֐�
	////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// �|�C���g���C�g��ݒ肷��
		/// </summary>
		/// <param name="pos">���C�g�̈ʒu</param>
		/// <param name="color">���C�g�̐F</param>
		/// <param name="range">x�Ƀ��C�g�̉e���͈�,y�ɉe���͈͂ɗݏ悷��p�����[�^</param>
		void SetPointLight(Vector3 pos, Vector4 color, Vector3 range)
		{
			m_sceneLight.SetPointLight(pos, color, range);
		}
		/// <summary>
		/// �|�C���g���C�g�̍��W��ݒ肷��
		/// </summary>
		/// <param name="pos"></param>
		void SetPointLightPosition(Vector3 pos)
		{
			m_sceneLight.SetPointLightPosition(pos);
		}
		/// <summary>
		/// �|�C���g���C�g�̐F��ݒ肷��
		/// </summary>
		/// <param name="color">�F</param>
		void SetPointLightColor(Vector4 color)
		{
			m_sceneLight.SetPointLightColor(color);
		}
		/// <summary>
		/// �e���͈͂Ɨݏ悷��p�����[�^��ݒ�
		/// </summary>
		/// <param name="attn">X�ɉe���͈�,Y�ɗݏ悷��p�����[�^</param>
		void SetPointLightAttn(Vector3 attn)
		{
			m_sceneLight.SetPointLightAttn(attn);
		}
		/// <summary>
		/// �|�C���g���C�g���g�p����
		/// </summary>
		void UsePointLight()
		{
			m_sceneLight.UsePointLight();
		}
		/// <summary>
		/// �|�C���g���C�g���g�p���Ȃ�
		/// </summary>
		void UnUsePointLight()
		{
			m_sceneLight.UnUsePointLight();
		}

		/// <summary>
		/// �|�C���g���C�g�̈ʒu���擾����
		/// </summary>
		/// <returns>���W</returns>
		const Vector3& GetPointLightPosition() const
		{
			return m_sceneLight.GetPointLightPosition();
		}
		/// <summary>
		/// �|�C���g���C�g�̌��̐F���擾
		/// </summary>
		/// <returns>�F</returns>
		const Vector4& GetPointLightColor() const
		{
			return m_sceneLight.GetPointLightColor();
		}
		/// <summary>
		/// �X�|�b�g���C�g�̉e���͈͂Ɨݏ悷��p�����[�^���擾
		/// </summary>
		/// <returns>X�ɉe���͈�,Y�ɗݏ悷��p�����[�^</returns>
		const Vector3& GetPointLightAttn() const
		{
			return m_sceneLight.GetPointLightAttn();
		}
		/// <summary>
		/// �|�C���g���C�g�͎g�p��?
		/// </summary>
		/// <returns>�g�p���Ȃ�true</returns>
		const int PointLightIsUse() const
		{
			return m_sceneLight.PointLightIsUse();
		}

	////////////////////////////////////////////////////////////////////////////////
	///�X�|�b�g���C�g�̊֐�
	////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// �X�|�b�g���C�g��ݒ肷��
		/// </summary>
		/// <param name="pos">�ʒu</param>
		/// <param name="color">�F</param>
		/// <param name="range">x�ɉe���͈�,y�ɉe���͈͂ɗݏ悷��p�����[�^</param>
		/// <param name="direction">�Ǝ˕���</param>
		/// <param name="angle">x�͏Ǝˊp�x,���͉e���ɗݏ悷��p�����[�^</param>
		void SetSpotLight(Vector3 pos, Vector4 color, Vector3 attn, Vector3 direction, Vector3 angle)
		{
			m_sceneLight.SetSpotLight(pos, color, attn, direction, angle);
		}
		/// <summary>
		/// �X�|�b�g���C�g�̈ʒu��ݒ肷��
		/// </summary>
		/// <param name="pos">���W</param>
		void SetSpotLightPosition(Vector3 pos)
		{
			m_sceneLight.SetSpotLightPosition(pos);
		}
		/// <summary>
		/// �X�|�b�g���C�g�̃��C�g�F�̐ݒ�
		/// </summary>
		/// <param name="color">�F</param>
		void SetSpotLightColor(Vector4 color)
		{
			m_sceneLight.SetSpotLightColor(color);
		}
		/// <summary>
		/// �e���͈͂Ɨݏ悷��p�����[�^��ݒ�
		/// </summary>
		/// <param name="attn">X�ɉe���͈�,Y�ɗݏ悷��p�����[�^</param>
		void SetSpotLightAttn(Vector3 attn)
		{
			m_sceneLight.SetSpotLightAttn(attn);
		}
		/// <summary>
		/// �X�|�b�g���C�g�̃��C�g�̕�����ݒ�
		/// </summary>
		/// <param name="direction">����</param>
		void SetSpotLightDirection(Vector3 direction)
		{
			m_sceneLight.SetSpotLightDirection(direction);
		}
		/// <summary>
		/// �X�|�b�g���C�g�̃��C�g�̊p�x��ݒ�
		/// </summary>
		/// <param name="angle">�p�x</param>
		void SetSpotLightAngle(Vector3 angle)
		{
			m_sceneLight.SetSpotLightAngle(angle);
		}
		/// <summary>
		/// �X�|�b�g���C�g���g�p����
		/// </summary>
		void UseSpotLight()
		{
			m_sceneLight.UseSpotLight();
		}
		/// <summary>
		/// �X�|�b�g���C�g���g�p���Ȃ�
		/// </summary>
		void UnUseSpotLight()
		{
			m_sceneLight.UnUseSpotLight();
		}

		/// <summary>
		/// �X�|�b�g���C�g�̈ʒu���擾
		/// </summary>
		/// <returns>���W</returns>
		const Vector3& GetSpotLightPosition() const
		{
			return m_sceneLight.GetSpotLightPosition();
		}
		/// <summary>
		/// �X�|�b�g���C�g�̌��̐F���擾
		/// </summary>
		/// <returns>�F</returns>
		const Vector4& GetSpotLightColor() const
		{
			return m_sceneLight.GetSpotLightColor();
		}
		/// <summary>
		/// �X�|�b�g���C�g�̉e���͈͂Ɨݏ悷��p�����[�^���擾
		/// </summary>
		/// <returns></returns>
		const Vector3& GetSpotLightAttn()const
		{
			return m_sceneLight.GetSpotLightAttn();
		}
		/// <summary>
		/// �X�|�b�g���C�g�̌��̕������擾����
		/// </summary>
		/// <returns>����</returns>
		const Vector3& GetSpotLightDirection() const
		{
			return m_sceneLight.GetSpotLightDirection();
		}
		/// <summary>
		/// �X�|�b�g���C�g�̊p�x���擾����
		/// </summary>
		/// <returns>�p�x</returns>
		const Vector3& GetSpotLightAngle() const
		{
			return m_sceneLight.GetSpotLightAngle();
		}
		/// <summary>
		/// �X�|�b�g���C�g�͎g�p���H
		/// </summary>
		/// <returns>�g�p���̏ꍇtrue</returns>
		const int SpotLightIsUse() const
		{
			return m_sceneLight.SpotLightIsUse();
		}

	////////////////////////////////////////////////////////////////////////////////
	///�������C�g�̊֐�
	////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// �������C�g��ݒ肷��
		/// </summary>
		/// <param name="groundColor">�n�ʐF</param>
		/// <param name="skyColor">�V���F</param>
		/// <param name="groundNormal">�n�ʂ̖@��</param>
		void SetHemiLight(Vector3 groundColor, Vector3 skyColor, Vector3 groundNormal)
		{
			SetHemiLightGroundColor(groundColor);
			SetHemiLightSkyColor(skyColor);
			SetHemiLightGroundNormal(groundNormal);
			UseHemiLight();
		}
		/// <summary>
		/// �������C�g�̒n�ʐF��ݒ�
		/// </summary>
		/// <param name="groundColor">�n�ʐF</param>
		void SetHemiLightGroundColor(Vector3 groundColor)
		{
			m_sceneLight.SetHemiLightGroundColor(groundColor);
		}
		/// <summary>
		/// �������C�g�̓V���F��ݒ�
		/// </summary>
		/// <param name="skyColor">�V���F</param>
		void SetHemiLightSkyColor(Vector3 skyColor)
		{
			m_sceneLight.SetHemiLightSkyColor(skyColor);
		}
		/// <summary>
		/// �������C�g�̒n�ʂ̖@��
		/// </summary>
		/// <param name="normal">�n�ʂ̖@��</param>
		void SetHemiLightGroundNormal(Vector3 normal)
		{
			m_sceneLight.SetHemiLightGroundNormal(normal);
		}
		/// <summary>
		/// �������C�g���g�p����
		/// </summary>
		void UseHemiLight()
		{
			m_sceneLight.UseHemiLight();
		}
		/// <summary>
		/// �������C�g���g�p���Ȃ�
		/// </summary>
		void UnUseHemiLight()
		{
			m_sceneLight.UnUseHemiLight();
		}

		/// <summary>
		/// �������C�g�̒n�ʐF���擾����
		/// </summary>
		/// <returns>�n�ʐF</returns>
		const Vector3& GetHemiLightGroundColor() const
		{
			return m_sceneLight.GetHemiLightGroundColor();
		}
		/// <summary>
		/// �������C�g�̓V���F���擾����
		/// </summary>
		/// <returns></returns>
		const Vector3& GetHemiLightSkyColor() const
		{
			return m_sceneLight.GetHemiLightSkyColor();
		}
		/// <summary>
		/// �������C�g�̒n�ʂ̖@�����擾����
		/// </summary>
		/// <returns></returns>
		const Vector3& GetHemiLightGroundNormal() const
		{
			return m_sceneLight.GetHemiLightGroundNormal();
		}
		/// <summary>
		/// �������C�g�͎g�p��?
		/// </summary>
		/// <returns>�g�p���̂Ƃ�true</returns>
		const int HemiLightIsUse() const
		{
			return m_sceneLight.HemiLightIsUse();
		}

	private:
		std::vector<ModelRender*>	m_modelList;				//���f���N���X�̃��X�g
		std::vector<SpriteRender*>	m_spriteList;				//�X�v���C�g�N���X�̃��X�g
		std::vector<FontRender*>	m_fontList;					//�t�H���g�N���X�̃��X�g

		SceneLight					m_sceneLight;				//�V�[�����C�g
	};
}