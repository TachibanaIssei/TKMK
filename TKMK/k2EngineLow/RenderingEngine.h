#pragma once

#include "PostEffect.h"
#include "Light.h"
#include "Shadow.h"

namespace nsK2EngineLow {
	static const int MAX_VIEWPORT = 4;
	static const int DUO_VIEWPORT = 2;

	class ModelRender;
	class SpriteRender;
	class FontRender;
	class RenderContext;

	class RenderingEngine : public Noncopyable
	{
	public:
		enum EnGameMode {
			enGameMode_SoloPlay = 1,
			enGameMode_DuoPlay,
			enGameMode_TrioPlay,
			enGameMode_QuartetPlay,
			enGameMode_Num
		};
		/// <summary>
		/// ���݂̃J�����`��
		/// </summary>
		enum EnCameraDrawing
		{
			enCameraDrawing_Solo = 0,		//1��ʂ����̂Ƃ�
			enCameraDrawing_Left = 0,		//�����
			enCameraDrawing_Right = 1,		//�E���
			enCameraDrawing_LeftUp = 0,		//����
			enCameraDrawing_RightUp = 1,	//�E��
			enCameraDrawing_LeftDown = 2,	//����
			enCameraDrawing_RightDown = 3	//�E��
		};

		void Init();

		void InitRenderTargets();
		void InitCopyToFrameBufferSprite();

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
		void AddSpriteList(SpriteRender* spriteRender, const bool drawTiming = false)
		{
			if (drawTiming)
			{
				m_laterSpriteList.push_back(spriteRender);
			}
			else {
				m_spriteList.push_back(spriteRender);
			}
		}
		void AddFrontSpriteList(SpriteRender* spriteRender)
		{
			m_spriteFrontDrawList.push_back(spriteRender);
		}

		/// <summary>
		/// ��ʕ������̃r���[�|�[�g�ɕ`�悷��X�v���C�g�����X�g�ɒǉ�
		/// </summary>
		/// <param name="spriteRender"></param>
		/// <param name="viewportNo"></param>
		void AddSpriteDrawViewportList(SpriteRender* spriteRender, const int viewportNo)
		{
			m_spriteDrawViewportList[viewportNo].push_back(spriteRender);
		}

		/// <summary>
		/// �t�H���g�����_�[�N���X�����X�g�ɒǉ�����
		/// </summary>
		/// <param name="fontRender">�t�H���g�����_�[</param>
		void AddFontList(FontRender* fontRender, const bool drawTiming = false)
		{
			if (drawTiming)
			{
				m_laterFontList.push_back(fontRender);
			}
			else
			{
				m_fontList.push_back(fontRender);
			}
		}
		/// <summary>
		/// ��ʕ������̃r���[�|�[�g�ɕ`�悷��t�H���g�����_�[�N���X�����X�g�ɒǉ�����
		/// </summary>
		/// <param name="spriteRender"></param>
		/// <param name="viewportNo"></param>
		void AddFontDrawViewportList(FontRender* fontRender, const int viewportNo)
		{
			m_fontDrawViewportList[viewportNo].push_back(fontRender);
		}

		/// <summary>
		/// �V���h�E���f����`�悷��
		/// </summary>
		/// <param name="rc">�����_�[�R���e�L�X�g</param>
		void ShadowModelRendering(RenderContext& rc, Camera& camera, const int number);

		/// <summary>
		/// �`�揈�������s
		/// </summary>
		/// <param name="rc">�����_�[�R���e�L�X�g</param>
		void Execute(RenderContext& rc);

		/// <summary>
		/// �V�[�����C�g���擾
		/// </summary>
		/// <returns>�V�[�����C�g</returns>
		SceneLight& GetSceneLight(const int lightNumber)
		{
			return m_sceneLight[lightNumber];
		}

		/// <summary>
		/// ���C�g�r���[�v���W�F�N�V�����s���ݒ肷��
		/// </summary>
		/// <param name="LVP">���C�g�r���[�v���W�F�N�V�����s��</param>
		void SetmLVP(const int cameraDrawing, const Matrix LVP)
		{
			m_sceneLight[cameraDrawing].SetmLVP(LVP);
		}

		////////////////////////////////////////////////////////////////////////////////
		///�V���h�E�}�b�v�̊֐�
		////////////////////////////////////////////////////////////////////////////////
			/// <summary>
			/// �V���h�E�}�b�v�̃e�N�X�`�����擾
			/// </summary>
			/// <returns>�V���h�E�}�b�v�̃e�N�X�`��</returns>
		Texture& GetShadowMapTexture(const int number)
		{
			return m_shadow.GetShadowMapTexture(number);
		}

		/// <summary>
		/// ���C�g�J�������擾
		/// </summary>
		/// <returns>���C�g�J����</returns>
		Camera& GetLightCamera(const int number)
		{
			return m_shadow.GetLightCamera(number);
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
		void SetDirectionLight(const int lightNo, const Vector3 direction, const Vector3 color)
		{
			m_sceneLight[enCameraDrawing_Left].SetDirectionLight(lightNo, direction, color);
			m_sceneLight[enCameraDrawing_Right].SetDirectionLight(lightNo, direction, color);
			m_sceneLight[enCameraDrawing_RightDown].SetDirectionLight(lightNo, direction, color);
			m_sceneLight[enCameraDrawing_LeftDown].SetDirectionLight(lightNo, direction, color);
		}
		/// <summary>
		/// �f�B���N�V�������C�g�̌��̕�����ݒ肷��
		/// </summary>
		/// <param name="direction">����</param>
		void SetDirLightDirection(const Vector3 direction)
		{
			m_sceneLight[enCameraDrawing_Left].SetDirLightDirection(direction);
			m_sceneLight[enCameraDrawing_Right].SetDirLightDirection(direction);
			m_sceneLight[enCameraDrawing_RightDown].SetDirLightDirection(direction);
			m_sceneLight[enCameraDrawing_LeftDown].SetDirLightDirection(direction);
		}
		/// <summary>
		/// �f�B���N�V�������C�g�̌��̐F��ݒ肷��
		/// </summary>
		/// <param name="color">�F</param>
		void SetDirLightColor(const Vector3 color)
		{
			m_sceneLight[enCameraDrawing_Left].SetDirLightColor(color);
			m_sceneLight[enCameraDrawing_Right].SetDirLightColor(color);
			m_sceneLight[enCameraDrawing_RightDown].SetDirLightColor(color);
			m_sceneLight[enCameraDrawing_LeftDown].SetDirLightColor(color);
		}

		/// <summary>
		/// �f�B���N�V�������C�g�̌��̕������擾����
		/// </summary>
		/// <returns>���̕���</returns>
		const Vector3& GetDirLigDirection(const EnCameraDrawing cameraDrawing) const
		{
			return m_sceneLight[cameraDrawing].GetDirLigDirection();
		}
		/// <summary>
		/// �f�B���N�V�������C�g�̌��̐F���擾����
		/// </summary>
		/// <returns>�F</returns>
		const Vector3& GetDirLigColor(const EnCameraDrawing cameraDrawing) const
		{
			return m_sceneLight[cameraDrawing].GetDirLigColor();
		}

		////////////////////////////////////////////////////////////////////////////////
		///�����̊֐�
		////////////////////////////////////////////////////////////////////////////////
			/// <summary>
			/// ������ݒ�
			/// </summary>
			/// <param name="ambient">����</param>
		void SetAmbient(const Vector3 ambient)
		{
			m_sceneLight[enCameraDrawing_Left].SetAmbient(ambient);
			m_sceneLight[enCameraDrawing_Right].SetAmbient(ambient);
			m_sceneLight[enCameraDrawing_RightDown].SetAmbient(ambient);
			m_sceneLight[enCameraDrawing_LeftDown].SetAmbient(ambient);
		}

		////////////////////////////////////////////////////////////////////////////////////////
		///�J�����̈ʒu�̊֐�
		////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>
			/// �J�����̈ʒu��ݒ肷��
			/// </summary>
			/// <param name="eyePos"></param>
		void SetEyePos(const Vector3& eyePosLeft, const Vector3& eyePosRight, const Vector3& eyePosLeftDown, const Vector3& eyePosRightDown)
		{
			m_sceneLight[enCameraDrawing_Left].SetEyePos(eyePosLeft);
			m_sceneLight[enCameraDrawing_Right].SetEyePos(eyePosRight);
			m_sceneLight[enCameraDrawing_LeftDown].SetEyePos(eyePosLeftDown);
			m_sceneLight[enCameraDrawing_RightDown].SetEyePos(eyePosRightDown);

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
		void SetPointLight(const Vector3 pos, const Vector3 color, const Vector3 range)
		{
			m_sceneLight[enCameraDrawing_Left].SetPointLight(pos, color, range);
			m_sceneLight[enCameraDrawing_Right].SetPointLight(pos, color, range);
			m_sceneLight[enCameraDrawing_LeftDown].SetPointLight(pos, color, range);
			m_sceneLight[enCameraDrawing_RightDown].SetPointLight(pos, color, range);
		}
		/// <summary>
		/// �|�C���g���C�g�̍��W��ݒ肷��
		/// </summary>
		/// <param name="pos"></param>
		void SetPointLightPosition(const Vector3 pos)
		{
			m_sceneLight[enCameraDrawing_Left].SetPointLightPosition(pos);
			m_sceneLight[enCameraDrawing_Right].SetPointLightPosition(pos);
			m_sceneLight[enCameraDrawing_LeftDown].SetPointLightPosition(pos);
			m_sceneLight[enCameraDrawing_RightDown].SetPointLightPosition(pos);
		}
		/// <summary>
		/// �|�C���g���C�g�̐F��ݒ肷��
		/// </summary>
		/// <param name="color">�F</param>
		void SetPointLightColor(const Vector3 color)
		{
			m_sceneLight[enCameraDrawing_Left].SetPointLightColor(color);
			m_sceneLight[enCameraDrawing_Right].SetPointLightColor(color);
			m_sceneLight[enCameraDrawing_RightDown].SetPointLightColor(color);
			m_sceneLight[enCameraDrawing_LeftDown].SetPointLightColor(color);
		}
		/// <summary>
		/// �e���͈͂Ɨݏ悷��p�����[�^��ݒ�
		/// </summary>
		/// <param name="attn">X�ɉe���͈�,Y�ɗݏ悷��p�����[�^</param>
		void SetPointLightAttn(const Vector3 attn)
		{
			m_sceneLight[enCameraDrawing_Left].SetPointLightAttn(attn);
			m_sceneLight[enCameraDrawing_Right].SetPointLightAttn(attn);
			m_sceneLight[enCameraDrawing_LeftDown].SetPointLightAttn(attn);
			m_sceneLight[enCameraDrawing_RightDown].SetPointLightAttn(attn);
		}
		/// <summary>
		/// �|�C���g���C�g���g�p����
		/// </summary>
		void UsePointLight()
		{
			m_sceneLight[enCameraDrawing_Left].UsePointLight();
			m_sceneLight[enCameraDrawing_Right].UsePointLight();
			m_sceneLight[enCameraDrawing_RightDown].UsePointLight();
			m_sceneLight[enCameraDrawing_LeftDown].UsePointLight();
		}
		/// <summary>
		/// �|�C���g���C�g���g�p���Ȃ�
		/// </summary>
		void UnUsePointLight()
		{
			m_sceneLight[enCameraDrawing_Left].UnUsePointLight();
			m_sceneLight[enCameraDrawing_Right].UnUsePointLight();
			m_sceneLight[enCameraDrawing_RightDown].UnUsePointLight();
			m_sceneLight[enCameraDrawing_LeftDown].UnUsePointLight();
		}

		/// <summary>
		/// �|�C���g���C�g�̈ʒu���擾����
		/// </summary>
		/// <returns>���W</returns>
		const Vector3& GetPointLightPosition(const EnCameraDrawing cameraDrawing) const
		{
			return m_sceneLight[cameraDrawing].GetPointLightPosition();
		}
		/// <summary>
		/// �|�C���g���C�g�̌��̐F���擾
		/// </summary>
		/// <returns>�F</returns>
		const Vector3& GetPointLightColor(const EnCameraDrawing cameraDrawing) const
		{
			return m_sceneLight[cameraDrawing].GetPointLightColor();
		}
		/// <summary>
		/// �X�|�b�g���C�g�̉e���͈͂Ɨݏ悷��p�����[�^���擾
		/// </summary>
		/// <returns>X�ɉe���͈�,Y�ɗݏ悷��p�����[�^</returns>
		const Vector3& GetPointLightAttn(const EnCameraDrawing cameraDrawing) const
		{
			return m_sceneLight[cameraDrawing].GetPointLightAttn();
		}
		/// <summary>
		/// �|�C���g���C�g�͎g�p��?
		/// </summary>
		/// <returns>�g�p���Ȃ�true</returns>
		const int PointLightIsUse(const EnCameraDrawing cameraDrawing) const
		{
			return m_sceneLight[cameraDrawing].PointLightIsUse();
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
		void SetSpotLight(const Vector3 pos, const Vector3 color, const Vector3 attn, const Vector3 direction, const Vector3 angle)
		{
			m_sceneLight[enCameraDrawing_Left].SetSpotLight(pos, color, attn, direction, angle);
			m_sceneLight[enCameraDrawing_Right].SetSpotLight(pos, color, attn, direction, angle);
			m_sceneLight[enCameraDrawing_LeftDown].SetSpotLight(pos, color, attn, direction, angle);
			m_sceneLight[enCameraDrawing_RightDown].SetSpotLight(pos, color, attn, direction, angle);
		}
		/// <summary>
		/// �X�|�b�g���C�g�̈ʒu��ݒ肷��
		/// </summary>
		/// <param name="pos">���W</param>
		void SetSpotLightPosition(const Vector3 pos)
		{
			m_sceneLight[enCameraDrawing_Left].SetSpotLightPosition(pos);
			m_sceneLight[enCameraDrawing_Right].SetSpotLightPosition(pos);
			m_sceneLight[enCameraDrawing_LeftDown].SetSpotLightPosition(pos);
			m_sceneLight[enCameraDrawing_RightDown].SetSpotLightPosition(pos);
		}
		/// <summary>
		/// �X�|�b�g���C�g�̃��C�g�F�̐ݒ�
		/// </summary>
		/// <param name="color">�F</param>
		void SetSpotLightColor(const Vector3 color)
		{
			m_sceneLight[enCameraDrawing_Left].SetSpotLightColor(color);
			m_sceneLight[enCameraDrawing_Right].SetSpotLightColor(color);
			m_sceneLight[enCameraDrawing_RightDown].SetSpotLightColor(color);
			m_sceneLight[enCameraDrawing_LeftDown].SetSpotLightColor(color);
		}
		/// <summary>
		/// �e���͈͂Ɨݏ悷��p�����[�^��ݒ�
		/// </summary>
		/// <param name="attn">X�ɉe���͈�,Y�ɗݏ悷��p�����[�^</param>
		void SetSpotLightAttn(const Vector3 attn)
		{
			m_sceneLight[enCameraDrawing_Left].SetSpotLightAttn(attn);
			m_sceneLight[enCameraDrawing_Right].SetSpotLightAttn(attn);
			m_sceneLight[enCameraDrawing_LeftDown].SetSpotLightAttn(attn);
			m_sceneLight[enCameraDrawing_RightDown].SetSpotLightAttn(attn);
		}
		/// <summary>
		/// �X�|�b�g���C�g�̃��C�g�̕�����ݒ�
		/// </summary>
		/// <param name="direction">����</param>
		void SetSpotLightDirection(const Vector3 direction)
		{
			m_sceneLight[enCameraDrawing_Left].SetSpotLightDirection(direction);
			m_sceneLight[enCameraDrawing_Right].SetSpotLightDirection(direction);
			m_sceneLight[enCameraDrawing_RightDown].SetSpotLightDirection(direction);
			m_sceneLight[enCameraDrawing_LeftDown].SetSpotLightDirection(direction);
		}
		/// <summary>
		/// �X�|�b�g���C�g�̃��C�g�̊p�x��ݒ�
		/// </summary>
		/// <param name="angle">�p�x</param>
		void SetSpotLightAngle(const Vector3 angle)
		{
			m_sceneLight[enCameraDrawing_Left].SetSpotLightAngle(angle);
			m_sceneLight[enCameraDrawing_Right].SetSpotLightAngle(angle);
			m_sceneLight[enCameraDrawing_LeftDown].SetSpotLightAngle(angle);
			m_sceneLight[enCameraDrawing_RightDown].SetSpotLightAngle(angle);
		}
		/// <summary>
		/// �X�|�b�g���C�g���g�p����
		/// </summary>
		void UseSpotLight()
		{
			m_sceneLight[enCameraDrawing_Left].UseSpotLight();
			m_sceneLight[enCameraDrawing_Right].UseSpotLight();
			m_sceneLight[enCameraDrawing_RightDown].UseSpotLight();
			m_sceneLight[enCameraDrawing_LeftDown].UseSpotLight();
		}
		/// <summary>
		/// �X�|�b�g���C�g���g�p���Ȃ�
		/// </summary>
		void UnUseSpotLight()
		{
			m_sceneLight[enCameraDrawing_Left].UnUseSpotLight();
			m_sceneLight[enCameraDrawing_Right].UnUseSpotLight();
			m_sceneLight[enCameraDrawing_LeftDown].UnUseSpotLight();
			m_sceneLight[enCameraDrawing_RightDown].UnUseSpotLight();
		}

		/// <summary>
		/// �X�|�b�g���C�g�̈ʒu���擾
		/// </summary>
		/// <returns>���W</returns>
		const Vector3& GetSpotLightPosition(const EnCameraDrawing cameraDrawing) const
		{
			return m_sceneLight[cameraDrawing].GetSpotLightPosition();
		}
		/// <summary>
		/// �X�|�b�g���C�g�̌��̐F���擾
		/// </summary>
		/// <returns>�F</returns>
		const Vector3& GetSpotLightColor(const EnCameraDrawing cameraDrawing) const
		{
			return m_sceneLight[cameraDrawing].GetSpotLightColor();
		}
		/// <summary>
		/// �X�|�b�g���C�g�̉e���͈͂Ɨݏ悷��p�����[�^���擾
		/// </summary>
		/// <returns></returns>
		const Vector3& GetSpotLightAttn(const EnCameraDrawing cameraDrawing)const
		{
			return m_sceneLight[cameraDrawing].GetSpotLightAttn();
		}
		/// <summary>
		/// �X�|�b�g���C�g�̌��̕������擾����
		/// </summary>
		/// <returns>����</returns>
		const Vector3& GetSpotLightDirection(const EnCameraDrawing cameraDrawing) const
		{
			return m_sceneLight[cameraDrawing].GetSpotLightDirection();
		}
		/// <summary>
		/// �X�|�b�g���C�g�̊p�x���擾����
		/// </summary>
		/// <returns>�p�x</returns>
		const Vector3& GetSpotLightAngle(const EnCameraDrawing cameraDrawing) const
		{
			return m_sceneLight[cameraDrawing].GetSpotLightAngle();
		}
		/// <summary>
		/// �X�|�b�g���C�g�͎g�p���H
		/// </summary>
		/// <returns>�g�p���̏ꍇtrue</returns>
		const int SpotLightIsUse(const EnCameraDrawing cameraDrawing) const
		{
			return m_sceneLight[cameraDrawing].SpotLightIsUse();
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
		void SetHemiLight(const Vector3 groundColor, const Vector3 skyColor, const Vector3 groundNormal)
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
		void SetHemiLightGroundColor(const Vector3 groundColor)
		{
			m_sceneLight[enCameraDrawing_Left].SetHemiLightGroundColor(groundColor);
			m_sceneLight[enCameraDrawing_Right].SetHemiLightGroundColor(groundColor);
			m_sceneLight[enCameraDrawing_RightDown].SetHemiLightGroundColor(groundColor);
			m_sceneLight[enCameraDrawing_LeftDown].SetHemiLightGroundColor(groundColor);
		}
		/// <summary>
		/// �������C�g�̓V���F��ݒ�
		/// </summary>
		/// <param name="skyColor">�V���F</param>
		void SetHemiLightSkyColor(const Vector3 skyColor)
		{
			m_sceneLight[enCameraDrawing_Left].SetHemiLightSkyColor(skyColor);
			m_sceneLight[enCameraDrawing_Right].SetHemiLightSkyColor(skyColor);
			m_sceneLight[enCameraDrawing_RightDown].SetHemiLightSkyColor(skyColor);
			m_sceneLight[enCameraDrawing_LeftDown].SetHemiLightSkyColor(skyColor);
		}
		/// <summary>
		/// �������C�g�̒n�ʂ̖@��
		/// </summary>
		/// <param name="normal">�n�ʂ̖@��</param>
		void SetHemiLightGroundNormal(const Vector3 normal)
		{
			m_sceneLight[enCameraDrawing_Left].SetHemiLightGroundNormal(normal);
			m_sceneLight[enCameraDrawing_Right].SetHemiLightGroundNormal(normal);
			m_sceneLight[enCameraDrawing_RightDown].SetHemiLightGroundNormal(normal);
			m_sceneLight[enCameraDrawing_LeftDown].SetHemiLightGroundNormal(normal);
		}
		/// <summary>
		/// �������C�g���g�p����
		/// </summary>
		void UseHemiLight()
		{
			m_sceneLight[enCameraDrawing_Left].UseHemiLight();
			m_sceneLight[enCameraDrawing_Right].UseHemiLight();
			m_sceneLight[enCameraDrawing_RightDown].UseHemiLight();
			m_sceneLight[enCameraDrawing_LeftDown].UseHemiLight();
		}
		/// <summary>
		/// �������C�g���g�p���Ȃ�
		/// </summary>
		void UnUseHemiLight()
		{
			m_sceneLight[enCameraDrawing_Left].UnUseHemiLight();
			m_sceneLight[enCameraDrawing_Right].UnUseHemiLight();
			m_sceneLight[enCameraDrawing_RightDown].UnUseHemiLight();
			m_sceneLight[enCameraDrawing_LeftDown].UnUseHemiLight();
		}

		/// <summary>
		/// �������C�g�̒n�ʐF���擾����
		/// </summary>
		/// <returns>�n�ʐF</returns>
		const Vector3& GetHemiLightGroundColor(const EnCameraDrawing cameraDrawing) const
		{
			return m_sceneLight[cameraDrawing].GetHemiLightGroundColor();
		}
		/// <summary>
		/// �������C�g�̓V���F���擾����
		/// </summary>
		/// <returns></returns>
		const Vector3& GetHemiLightSkyColor(const EnCameraDrawing cameraDrawing) const
		{
			return m_sceneLight[cameraDrawing].GetHemiLightSkyColor();
		}
		/// <summary>
		/// �������C�g�̒n�ʂ̖@�����擾����
		/// </summary>
		/// <returns></returns>
		const Vector3& GetHemiLightGroundNormal(const EnCameraDrawing cameraDrawing) const
		{
			return m_sceneLight[cameraDrawing].GetHemiLightGroundNormal();
		}
		/// <summary>
		/// �������C�g�͎g�p��?
		/// </summary>
		/// <returns>�g�p���̂Ƃ�true</returns>
		const int HemiLightIsUse(const EnCameraDrawing cameraDrawing) const
		{
			return m_sceneLight[cameraDrawing].HemiLightIsUse();
		}

		////////////////////////////////////////////////////////////////////////////////
		///��ʕ����̊֐�
		////////////////////////////////////////////////////////////////////////////////

			/// <summary>
			/// �Q�[�����[�h�������_�����O�G���W���ɐݒ肷��
			/// </summary>
		void SetGameModeToRenderingEngine(const EnGameMode gameMode)
		{
			m_gameMode = gameMode;
		}
		/// <summary>
		/// �Q�[�����[�h���擾
		/// </summary>
		/// <returns></returns>
		const EnGameMode GetGameMode() const
		{
			return m_gameMode;
		}
		/// <summary>
		/// �ǂ���̃J������`�撆��
		/// </summary>
		/// <returns></returns>
		const EnCameraDrawing GetCameraDrawing() const
		{
			return m_cameraDrawing;
		}

		/// <summary>
		/// �t���[���̍ŏ��ɍs����G�t�F�N�g�̏���
		/// </summary>
		void EffectBeginRender();

	private:
		/// <summary>
		/// �r���[�|�[�g��Init����
		/// </summary>
		void InitViewPorts();

		/// <summary>
		/// ���f���`����r���[�|�[�g�̐����s����֐�
		/// </summary>
		/// <param name="rc">�����_�[�R���e�L�X�g</param>
		void DrawModelInViewPorts(RenderContext& rc);

		/// <summary>
		/// ���f����`�悷��
		/// </summary>
		/// <param name="rc">�����_�[�R���e�L�X�g</param>
		void ModelRendering(RenderContext& rc);
		/// <summary>
		/// �X�v���C�g��`�悷��
		/// </summary>
		/// <param name="rc">�����_�[�R���e�L�X�g</param>
		void SpriteRendering(RenderContext& rc, const bool drawTiming = false);
		/// <summary>
		/// �X�v���C�g����ʕ������̃r���[�|�[�g�ɕ`��
		/// </summary>
		/// <param name="rc"></param>
		/// <param name="viewportNo"></param>
		void SpriteViewportRendering(RenderContext& rc, const int viewportNo);
		void SpriteFrontRendering(RenderContext& rc);
		/// <summary>
		/// �t�H���g��`�悷��
		/// </summary>
		/// <param name="rc">�����_�[�R���e�L�X�g</param>
		void FontRendering(RenderContext& rc, const bool drawTiming = false);
		/// <summary>
		/// �t�H���g����ʕ������̃r���[�|�[�g�ɕ`��
		/// </summary>
		/// <param name="rc"></param>
		/// <param name="viewportNo"></param>
		void FontViewportRendering(RenderContext& rc, const int viewportNo);
		/// <summary>
		/// �G�t�F�N�g�`������s����
		/// </summary>
		/// <param name="rc"></param>
		void ExcuteEffectRender(RenderContext& rc);
		/// <summary>
		/// 2D�`����s��
		/// </summary>
		/// <param name="rc"></param>
		void Render2D(RenderContext& rc);
		/// <summary>
		/// �`�惊�X�g��clear����
		/// </summary>
		void ClearRenderList();

	private:
		std::vector<ModelRender*>	m_modelList;							//���f���N���X�̃��X�g
		std::vector<SpriteRender*>	m_spriteList;							//�X�v���C�g�N���X�̃��X�g
		std::vector<SpriteRender*>	m_laterSpriteList;						//�`�揇���x���X�v���C�g�N���X�̃��X�g
		std::vector<SpriteRender*>	m_spriteDrawViewportList[MAX_VIEWPORT];	//��ʕ������̃r���[�|�[�g�ɕ`�悷��X�v���C�g�̃��X�g
		std::vector<SpriteRender*>	m_spriteFrontDrawList;					//�S�Ăɂ����đO�ʂɕ`�悳���
		std::vector<FontRender*>	m_fontList;								//�t�H���g�N���X�̃��X�g
		std::vector<FontRender*>	m_laterFontList;						//laterSprite�̏ォ�當����`��
		std::vector<FontRender*>	m_fontDrawViewportList[MAX_VIEWPORT];	//��ʕ������̃r���[�|�[�g�ɕ`�悷��t�H���g�N���X�̃��X�g

		SceneLight					m_sceneLight[MAX_VIEWPORT];				//�V�[�����C�g

		RenderTarget				m_mainRenderTarget;						//���C�������_�[�^�[�Q�b�g
		RenderTarget				m_2DRenderTarget;						//2D�����_�[�^�[�Q�b�g
		Sprite						m_2DSprite;
		Sprite						m_mainSprite;
		Sprite						m_copyToFrameBufferSprite;				//�e�N�X�`����\��t���邽�߂̃X�v���C�g��������

		Shadow						m_shadow;								//�V���h�E�}�b�v
		PostEffect					m_postEffect;							//�|�X�g�G�t�F�N�g

		EnCameraDrawing m_cameraDrawing = enCameraDrawing_Left;

		D3D12_VIEWPORT m_soloViewPort;										//1��ʗp�̃r���[�|�[�g
		D3D12_VIEWPORT m_duoViewPorts[DUO_VIEWPORT];						//2��ʕ����p�̃r���[�|�[�g
		D3D12_VIEWPORT m_quarteViewPorts[MAX_VIEWPORT];						//4��ʕ����p�̃r���[�|�[�g
		EnGameMode m_gameMode = enGameMode_SoloPlay;
	};
}