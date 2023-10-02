#pragma once

#include "PostEffect.h"
#include "Light.h"
#include "Shadow.h"

namespace nsK2EngineLow {
	static const int MAX_VIEWPORT = 2;

	class ModelRender;
	class SpriteRender;
	class FontRender;
	class RenderContext;

	class RenderingEngine
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
			enCameraDrawing_Solo = 0,	//1��ʂ����̂Ƃ�
			enCameraDrawing_Left = 0,	//�����
			enCameraDrawing_Right = 1,	//�E���
			enCameraDrawing_Num
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
		void AddSpriteList(SpriteRender* spriteRender,bool drawTiming = false)
		{
			if (drawTiming)
			{
				m_laterSpriteList.push_back(spriteRender);
			}
			else {
				m_spriteList.push_back(spriteRender);
			}
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
		void AddFontList(FontRender* fontRender)
		{
			m_fontList.push_back(fontRender);
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
		void ShadowModelRendering(RenderContext& rc, Camera& camera,int number);

		/// <summary>
		/// �`�揈�������s
		/// </summary>
		/// <param name="rc">�����_�[�R���e�L�X�g</param>
		void Execute(RenderContext& rc);

		/// <summary>
		/// �V�[�����C�g���擾
		/// </summary>
		/// <returns>�V�[�����C�g</returns>
		SceneLight& GetSceneLight(int lightNumber)
		{
			return m_sceneLight[lightNumber];
		}

		/// <summary>
		/// ���C�g�r���[�v���W�F�N�V�����s���ݒ肷��
		/// </summary>
		/// <param name="LVP">���C�g�r���[�v���W�F�N�V�����s��</param>
		void SetmLVP(int cameraDrawing, Matrix LVP)
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
		Texture& GetShadowMapTexture(int number)
		{
			return m_shadow.GetShadowMapTexture(number);
		}

		/// <summary>
		/// ���C�g�J�������擾
		/// </summary>
		/// <returns>���C�g�J����</returns>
		Camera& GetLightCamera(int number)
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
		void SetDirectionLight(int lightNo, Vector3 direction, Vector3 color)
		{
			m_sceneLight[enCameraDrawing_Left].SetDirectionLight(lightNo, direction, color);
			m_sceneLight[enCameraDrawing_Right].SetDirectionLight(lightNo, direction, color);
		}
		/// <summary>
		/// �f�B���N�V�������C�g�̌��̕�����ݒ肷��
		/// </summary>
		/// <param name="direction">����</param>
		void SetDirLightDirection(Vector3 direction)
		{
			m_sceneLight[enCameraDrawing_Left].SetDirLightDirection(direction);
			m_sceneLight[enCameraDrawing_Right].SetDirLightDirection(direction);
		}
		/// <summary>
		/// �f�B���N�V�������C�g�̌��̐F��ݒ肷��
		/// </summary>
		/// <param name="color">�F</param>
		void SetDirLightColor(Vector3 color)
		{
			m_sceneLight[enCameraDrawing_Left].SetDirLightColor(color);
			m_sceneLight[enCameraDrawing_Right].SetDirLightColor(color);
		}

		/// <summary>
		/// �f�B���N�V�������C�g�̌��̕������擾����
		/// </summary>
		/// <returns>���̕���</returns>
		const Vector3& GetDirLigDirection(EnCameraDrawing cameraDrawing) const
		{
			return m_sceneLight[cameraDrawing].GetDirLigDirection();
		}
		/// <summary>
		/// �f�B���N�V�������C�g�̌��̐F���擾����
		/// </summary>
		/// <returns>�F</returns>
		const Vector3& GetDirLigColor(EnCameraDrawing cameraDrawing) const
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
		void SetAmbient(Vector3 ambient)
		{
			m_sceneLight[enCameraDrawing_Left].SetAmbient(ambient);
			m_sceneLight[enCameraDrawing_Right].SetAmbient(ambient);
		}

	////////////////////////////////////////////////////////////////////////////////////////
	///�J�����̈ʒu�̊֐�
	////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// �J�����̈ʒu��ݒ肷��
		/// </summary>
		/// <param name="eyePos"></param>
		void SetEyePos(Vector3 eyePosLeft,Vector3 eyePosRight)
		{
			m_sceneLight[enCameraDrawing_Left].SetEyePos(eyePosLeft);
			m_sceneLight[enCameraDrawing_Right].SetEyePos(eyePosRight);
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
		void SetPointLight(Vector3 pos, Vector3 color, Vector3 range)
		{
			m_sceneLight[enCameraDrawing_Left].SetPointLight(pos, color, range);
			m_sceneLight[enCameraDrawing_Right].SetPointLight(pos, color, range);
		}
		/// <summary>
		/// �|�C���g���C�g�̍��W��ݒ肷��
		/// </summary>
		/// <param name="pos"></param>
		void SetPointLightPosition(Vector3 pos)
		{
			m_sceneLight[enCameraDrawing_Left].SetPointLightPosition(pos);
			m_sceneLight[enCameraDrawing_Right].SetPointLightPosition(pos);
		}
		/// <summary>
		/// �|�C���g���C�g�̐F��ݒ肷��
		/// </summary>
		/// <param name="color">�F</param>
		void SetPointLightColor(Vector3 color)
		{
			m_sceneLight[enCameraDrawing_Left].SetPointLightColor(color);
			m_sceneLight[enCameraDrawing_Right].SetPointLightColor(color);
		}
		/// <summary>
		/// �e���͈͂Ɨݏ悷��p�����[�^��ݒ�
		/// </summary>
		/// <param name="attn">X�ɉe���͈�,Y�ɗݏ悷��p�����[�^</param>
		void SetPointLightAttn(Vector3 attn)
		{
			m_sceneLight[enCameraDrawing_Left].SetPointLightAttn(attn);
			m_sceneLight[enCameraDrawing_Right].SetPointLightAttn(attn);
		}
		/// <summary>
		/// �|�C���g���C�g���g�p����
		/// </summary>
		void UsePointLight()
		{
			m_sceneLight[enCameraDrawing_Left].UsePointLight();
			m_sceneLight[enCameraDrawing_Right].UsePointLight();
		}
		/// <summary>
		/// �|�C���g���C�g���g�p���Ȃ�
		/// </summary>
		void UnUsePointLight()
		{
			m_sceneLight[enCameraDrawing_Left].UnUsePointLight();
			m_sceneLight[enCameraDrawing_Right].UnUsePointLight();
		}

		/// <summary>
		/// �|�C���g���C�g�̈ʒu���擾����
		/// </summary>
		/// <returns>���W</returns>
		const Vector3& GetPointLightPosition(EnCameraDrawing cameraDrawing) const
		{
			return m_sceneLight[cameraDrawing].GetPointLightPosition();
		}
		/// <summary>
		/// �|�C���g���C�g�̌��̐F���擾
		/// </summary>
		/// <returns>�F</returns>
		const Vector3& GetPointLightColor(EnCameraDrawing cameraDrawing) const
		{
			return m_sceneLight[cameraDrawing].GetPointLightColor();
		}
		/// <summary>
		/// �X�|�b�g���C�g�̉e���͈͂Ɨݏ悷��p�����[�^���擾
		/// </summary>
		/// <returns>X�ɉe���͈�,Y�ɗݏ悷��p�����[�^</returns>
		const Vector3& GetPointLightAttn(EnCameraDrawing cameraDrawing) const
		{
			return m_sceneLight[cameraDrawing].GetPointLightAttn();
		}
		/// <summary>
		/// �|�C���g���C�g�͎g�p��?
		/// </summary>
		/// <returns>�g�p���Ȃ�true</returns>
		const int PointLightIsUse(EnCameraDrawing cameraDrawing) const
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
		void SetSpotLight(Vector3 pos, Vector3 color, Vector3 attn, Vector3 direction, Vector3 angle)
		{
			m_sceneLight[enCameraDrawing_Left].SetSpotLight(pos, color, attn, direction, angle);
			m_sceneLight[enCameraDrawing_Right].SetSpotLight(pos, color, attn, direction, angle);
		}
		/// <summary>
		/// �X�|�b�g���C�g�̈ʒu��ݒ肷��
		/// </summary>
		/// <param name="pos">���W</param>
		void SetSpotLightPosition(Vector3 pos)
		{
			m_sceneLight[enCameraDrawing_Left].SetSpotLightPosition(pos);
			m_sceneLight[enCameraDrawing_Right].SetSpotLightPosition(pos);
		}
		/// <summary>
		/// �X�|�b�g���C�g�̃��C�g�F�̐ݒ�
		/// </summary>
		/// <param name="color">�F</param>
		void SetSpotLightColor(Vector3 color)
		{
			m_sceneLight[enCameraDrawing_Left].SetSpotLightColor(color);
			m_sceneLight[enCameraDrawing_Right].SetSpotLightColor(color);
		}
		/// <summary>
		/// �e���͈͂Ɨݏ悷��p�����[�^��ݒ�
		/// </summary>
		/// <param name="attn">X�ɉe���͈�,Y�ɗݏ悷��p�����[�^</param>
		void SetSpotLightAttn(Vector3 attn)
		{
			m_sceneLight[enCameraDrawing_Left].SetSpotLightAttn(attn);
			m_sceneLight[enCameraDrawing_Right].SetSpotLightAttn(attn);
		}
		/// <summary>
		/// �X�|�b�g���C�g�̃��C�g�̕�����ݒ�
		/// </summary>
		/// <param name="direction">����</param>
		void SetSpotLightDirection(Vector3 direction)
		{
			m_sceneLight[enCameraDrawing_Left].SetSpotLightDirection(direction);
			m_sceneLight[enCameraDrawing_Right].SetSpotLightDirection(direction);
		}
		/// <summary>
		/// �X�|�b�g���C�g�̃��C�g�̊p�x��ݒ�
		/// </summary>
		/// <param name="angle">�p�x</param>
		void SetSpotLightAngle(Vector3 angle)
		{
			m_sceneLight[enCameraDrawing_Left].SetSpotLightAngle(angle);
			m_sceneLight[enCameraDrawing_Right].SetSpotLightAngle(angle);
		}
		/// <summary>
		/// �X�|�b�g���C�g���g�p����
		/// </summary>
		void UseSpotLight()
		{
			m_sceneLight[enCameraDrawing_Left].UseSpotLight();
			m_sceneLight[enCameraDrawing_Right].UseSpotLight();
		}
		/// <summary>
		/// �X�|�b�g���C�g���g�p���Ȃ�
		/// </summary>
		void UnUseSpotLight()
		{
			m_sceneLight[enCameraDrawing_Left].UnUseSpotLight();
			m_sceneLight[enCameraDrawing_Right].UnUseSpotLight();
		}

		/// <summary>
		/// �X�|�b�g���C�g�̈ʒu���擾
		/// </summary>
		/// <returns>���W</returns>
		const Vector3& GetSpotLightPosition(EnCameraDrawing cameraDrawing) const
		{
			return m_sceneLight[cameraDrawing].GetSpotLightPosition();
		}
		/// <summary>
		/// �X�|�b�g���C�g�̌��̐F���擾
		/// </summary>
		/// <returns>�F</returns>
		const Vector3& GetSpotLightColor(EnCameraDrawing cameraDrawing) const
		{
			return m_sceneLight[cameraDrawing].GetSpotLightColor();
		}
		/// <summary>
		/// �X�|�b�g���C�g�̉e���͈͂Ɨݏ悷��p�����[�^���擾
		/// </summary>
		/// <returns></returns>
		const Vector3& GetSpotLightAttn(EnCameraDrawing cameraDrawing)const
		{
			return m_sceneLight[cameraDrawing].GetSpotLightAttn();
		}
		/// <summary>
		/// �X�|�b�g���C�g�̌��̕������擾����
		/// </summary>
		/// <returns>����</returns>
		const Vector3& GetSpotLightDirection(EnCameraDrawing cameraDrawing) const
		{
			return m_sceneLight[cameraDrawing].GetSpotLightDirection();
		}
		/// <summary>
		/// �X�|�b�g���C�g�̊p�x���擾����
		/// </summary>
		/// <returns>�p�x</returns>
		const Vector3& GetSpotLightAngle(EnCameraDrawing cameraDrawing) const
		{
			return m_sceneLight[cameraDrawing].GetSpotLightAngle();
		}
		/// <summary>
		/// �X�|�b�g���C�g�͎g�p���H
		/// </summary>
		/// <returns>�g�p���̏ꍇtrue</returns>
		const int SpotLightIsUse(EnCameraDrawing cameraDrawing) const
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
			m_sceneLight[enCameraDrawing_Left].SetHemiLightGroundColor(groundColor);
			m_sceneLight[enCameraDrawing_Right].SetHemiLightGroundColor(groundColor);
		}
		/// <summary>
		/// �������C�g�̓V���F��ݒ�
		/// </summary>
		/// <param name="skyColor">�V���F</param>
		void SetHemiLightSkyColor(Vector3 skyColor)
		{
			m_sceneLight[enCameraDrawing_Left].SetHemiLightSkyColor(skyColor);
			m_sceneLight[enCameraDrawing_Right].SetHemiLightSkyColor(skyColor);
		}
		/// <summary>
		/// �������C�g�̒n�ʂ̖@��
		/// </summary>
		/// <param name="normal">�n�ʂ̖@��</param>
		void SetHemiLightGroundNormal(Vector3 normal)
		{
			m_sceneLight[enCameraDrawing_Left].SetHemiLightGroundNormal(normal);
			m_sceneLight[enCameraDrawing_Right].SetHemiLightGroundNormal(normal);
		}
		/// <summary>
		/// �������C�g���g�p����
		/// </summary>
		void UseHemiLight()
		{
			m_sceneLight[enCameraDrawing_Left].UseHemiLight();
			m_sceneLight[enCameraDrawing_Right].UseHemiLight();
		}
		/// <summary>
		/// �������C�g���g�p���Ȃ�
		/// </summary>
		void UnUseHemiLight()
		{
			m_sceneLight[enCameraDrawing_Left].UnUseHemiLight();
			m_sceneLight[enCameraDrawing_Right].UnUseHemiLight();
		}

		/// <summary>
		/// �������C�g�̒n�ʐF���擾����
		/// </summary>
		/// <returns>�n�ʐF</returns>
		const Vector3& GetHemiLightGroundColor(EnCameraDrawing cameraDrawing) const
		{
			return m_sceneLight[cameraDrawing].GetHemiLightGroundColor();
		}
		/// <summary>
		/// �������C�g�̓V���F���擾����
		/// </summary>
		/// <returns></returns>
		const Vector3& GetHemiLightSkyColor(EnCameraDrawing cameraDrawing) const
		{
			return m_sceneLight[cameraDrawing].GetHemiLightSkyColor();
		}
		/// <summary>
		/// �������C�g�̒n�ʂ̖@�����擾����
		/// </summary>
		/// <returns></returns>
		const Vector3& GetHemiLightGroundNormal(EnCameraDrawing cameraDrawing) const
		{
			return m_sceneLight[cameraDrawing].GetHemiLightGroundNormal();
		}
		/// <summary>
		/// �������C�g�͎g�p��?
		/// </summary>
		/// <returns>�g�p���̂Ƃ�true</returns>
		const int HemiLightIsUse(EnCameraDrawing cameraDrawing) const
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
		void SpriteRendering(RenderContext& rc, bool drawTiming);
		/// <summary>
		/// �X�v���C�g����ʕ������̃r���[�|�[�g�ɕ`��
		/// </summary>
		/// <param name="rc"></param>
		/// <param name="viewportNo"></param>
		void SpriteViewportRendering(RenderContext& rc, int viewportNo);
		/// <summary>
		/// �t�H���g��`�悷��
		/// </summary>
		/// <param name="rc">�����_�[�R���e�L�X�g</param>
		void FontRendering(RenderContext& rc);
		/// <summary>
		/// �t�H���g����ʕ������̃r���[�|�[�g�ɕ`��
		/// </summary>
		/// <param name="rc"></param>
		/// <param name="viewportNo"></param>
		void FontViewportRendering(RenderContext& rc, int viewportNo);
		/// <summary>
		/// �G�t�F�N�g�`������s����
		/// </summary>
		/// <param name="rc"></param>
		void ExcuteEffectRender(RenderContext& rc);
		/// <summary>
		/// vector�R���e�i�̃��X�g����������
		/// </summary>
		void ClearVectorList();

	private:
		std::vector<ModelRender*>	m_modelList;				//���f���N���X�̃��X�g
		std::vector<SpriteRender*>	m_spriteList;				//�X�v���C�g�N���X�̃��X�g
		std::vector<SpriteRender*>	m_laterSpriteList;			//�`�揇���x���X�v���C�g�N���X�̃��X�g
		std::vector<SpriteRender*>	m_spriteDrawViewportList[MAX_VIEWPORT];	//��ʕ������̃r���[�|�[�g�ɕ`�悷��X�v���C�g�̃��X�g
		std::vector<FontRender*>	m_fontList;					//�t�H���g�N���X�̃��X�g
		std::vector<FontRender*>	m_fontDrawViewportList[MAX_VIEWPORT];	//��ʕ������̃r���[�|�[�g�ɕ`�悷��t�H���g�N���X�̃��X�g

		SceneLight					m_sceneLight[MAX_VIEWPORT];				//�V�[�����C�g

		RenderTarget				m_mainRenderTarget;			//���C�������_�[�^�[�Q�b�g
		Sprite						m_copyToFrameBufferSprite;	//�e�N�X�`����\��t���邽�߂̃X�v���C�g��������

		Shadow						m_shadow;					//�V���h�E�}�b�v
		PostEffect					m_postEffect;				//�|�X�g�G�t�F�N�g

		EnCameraDrawing m_cameraDrawing = enCameraDrawing_Left;

		D3D12_VIEWPORT m_soloViewPort;					//1��ʗp�̃r���[�|�[�g
		D3D12_VIEWPORT m_viewPorts[MAX_VIEWPORT];	//��ʕ����p�̃r���[�|�[�g
		EnGameMode m_gameMode = enGameMode_SoloPlay;
	};
}