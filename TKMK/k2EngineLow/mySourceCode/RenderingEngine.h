#pragma once

#include "MyRenderer.h"
#include "PostEffect/PostEffect.h"
#include "Light/SceneLight.h"
#include "Shadow/Shadow.h"

namespace nsK2EngineLow {
	static const int MAX_VIEWPORT = 4;
	static const int DUO_VIEWPORT = 2;

	class IRenderer;
	class SpriteRender;
	class FontRender;
	class RenderContext;

	class RenderingEngine : public Noncopyable
	{
	public:
		struct SLightingCB
		{
			Light m_light;	//���C�g
			//Matrix mlvp[MAX_DIRECTIONAL_LIGHT][NUM_SHADOW_MAP];	//���C�g�r���[�v���W�F�N�V�����s��
		};

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
		/// �`��I�u�W�F�N�g��ǉ�
		/// </summary>
		/// <param name="renderObject">�`��I�u�W�F�N�g</param>
		void AddRenderObject(IRenderer* renderObject)
		{
			m_renderObjects.emplace_back(renderObject);
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

		/// <summary>
		/// �f�B���N�V�������C�g��ݒ�
		/// </summary>
		/// <param name="lightNo">���C�g�ԍ�</param>
		/// <param name="direction">���C�g����</param>
		/// <param name="color">���C�g�F</param>
		void SetDirectionLight(const int lightNo, const Vector3 direction, const Vector3 color);
		/// <summary>
		/// �L���X�g�V���h�E�t���O���擾
		/// </summary>
		/// <param name="lightNo">���C�g�ԍ�</param>
		/// <returns>true��������e�𗎂Ƃ�</returns>
		void SetDirectionLightCastShadow(const int lightNo, const bool flag);

		/// <summary>
		/// ������ݒ�
		/// </summary>
		/// <param name="ambient">����</param>
		void SetAmbient(const Vector3 ambient);

		/// <summary>
		/// ���C�e�B���O�̒萔�o�b�t�@���擾
		/// </summary>
		/// <returns>�萔�o�b�t�@</returns>
		SLightingCB& GetLightingCB()
		{
			return m_lightingCB;
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
		/// �t�H���[�h�����_�����O�ł̃��f���`��
		/// </summary>
		/// <param name="rc"></param>
		void FowardRendering(RenderContext& rc);
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
		std::vector<IRenderer*>		m_renderObjects;
		std::vector<SpriteRender*>	m_spriteList;							//�X�v���C�g�N���X�̃��X�g
		std::vector<SpriteRender*>	m_laterSpriteList;						//�`�揇���x���X�v���C�g�N���X�̃��X�g
		std::vector<SpriteRender*>	m_spriteDrawViewportList[MAX_VIEWPORT];	//��ʕ������̃r���[�|�[�g�ɕ`�悷��X�v���C�g�̃��X�g
		std::vector<SpriteRender*>	m_spriteFrontDrawList;					//�S�Ăɂ����đO�ʂɕ`�悳���
		std::vector<FontRender*>	m_fontList;								//�t�H���g�N���X�̃��X�g
		std::vector<FontRender*>	m_laterFontList;						//laterSprite�̏ォ�當����`��
		std::vector<FontRender*>	m_fontDrawViewportList[MAX_VIEWPORT];	//��ʕ������̃r���[�|�[�g�ɕ`�悷��t�H���g�N���X�̃��X�g

		SceneLight					m_sceneLight[MAX_VIEWPORT];				//�V�[�����C�g
		SLightingCB					m_lightingCB;							//���C�g�̒萔�o�b�t�@

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