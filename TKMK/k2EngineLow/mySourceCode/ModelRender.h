#pragma once

namespace nsK2EngineLow {
	class ModelRender : public IRenderer
	{
	//�����o�֐�
	public:
		/// <summary>
		/// �ʏ�̏�����
		/// </summary>
		/// <param name="tkmFilepath">tkm�t�@�C���p�X</param>
		/// <param name="enModelUpAxis">���f���̏����</param>
		/// <param name="animationClips">�A�j���[�V�����N���b�v</param>
		/// <param name="numAnimationClips">�A�j���[�V�����N���b�v�̐�</param>
		void Init(
			const char* tkmFilepath,
			AnimationClip* animationClips = nullptr,
			const int numAnimationClips = 0,
			const EnModelUpAxis enModelUpAxis = enModelUpAxisZ,
			const bool isShadowReceiver = true,
			const bool isFrontCullingOnDrawShadowMap = false
		);

		/// <summary>
		/// �X�J�C�L���[�u������������
		/// </summary>
		/// <param name="initData"></param>
		void InitSkyCube(ModelInitData& initData);

		/// <summary>
		/// ���f�����擾����
		/// </summary>
		/// <returns>���f��</returns>
		Model& GetModel()
		{
			return m_model[0];
		}

		/// <summary>
		/// ���W�A��]�A�g������ׂĐݒ�
		/// </summary>
		/// <param name="pos">���W</param>
		/// <param name="rotation">��]</param>
		/// <param name="scale">�g��</param>
		void SetTRS(const Vector3& pos, const Quaternion& rotation, const Vector3& scale)
		{
			SetPosition(pos);
			SetRotation(rotation);
			SetScale(scale);
		}

		/// <summary>
		/// ���W�̐ݒ�
		/// </summary>
		/// <param name="position">Vector3�̍��W</param>
		void SetPosition(const Vector3& position)
		{
			m_position = position;
		};

		/// <summary>
		/// ���W�̐ݒ�
		/// </summary>
		/// <param name="x">X��</param>
		/// <param name="y">Y��</param>
		/// <param name="z">Z��</param>
		void SetPosition(const float x, const float y, const float z)
		{
			SetPosition({ x, y, z });
		};

		/// <summary>
		/// ��]�̐ݒ�
		/// </summary>
		/// <param name="rotation">��]</param>
		void SetRotation(const Quaternion& rotation)
		{
			m_rotation = rotation;
		}

		/// <summary>
		/// �傫���̐ݒ�
		/// </summary>
		/// <param name="scale">�傫��</param>
		void SetScale(const Vector3& scale)
		{
			m_scale = scale;
		}

		/// <summary>
		/// �傫���̐ݒ�
		/// </summary>
		/// <param name="x">X��</param>
		/// <param name="y">Y��</param>
		/// <param name="z">Z��</param>
		void SetScale(const float x, const float y, const float z)
		{
			SetScale({ x,y,z });
		};

		/// <summary>
		/// �X�V����
		/// </summary>
		void Update();

		/// <summary>
		/// �`�揈��
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g</param>
		void Draw(RenderContext& rc);

		void OnRenderShadowModel(RenderContext& rc,Camera& camera, const int number) override
		{
			m_shadowModel[number].Draw(rc, camera);
		}

		/// <summary>
		/// �{�[���̖��O����{�[���ԍ�������
		/// </summary>
		/// <param name="boneName">�{�[���̖��O</param>
		/// <returns>�{�[���ԍ��B������Ȃ������ꍇ��-1���Ԃ�</returns>
		const int FindBoneID(const wchar_t* boneName)const
		{
			return m_skeleton.FindBoneID(boneName);
		}

		/// <summary>
		/// �{�[���ԍ�����{�[�����擾����
		/// </summary>
		/// <param name="boneNo">�{�[���ԍ�</param>
		/// <returns>�{�[��</returns>
		Bone* GetBone(const int boneNo) const
		{
			return m_skeleton.GetBone(boneNo);
		}

		/// <summary>
		/// �A�j���[�V�����C�x���g��ǉ�����
		/// </summary>
		/// <param name="eventListener"></param>
		void AddAnimationEvent(const AnimationEventListener eventListener)
		{
			m_animation.AddAnimationEventListener(eventListener);
		}

		/// <summary>
		/// �A�j���[�V�����Đ��B
		/// </summary>
		/// <param name="animNo">�A�j���[�V�����N���b�v�̔ԍ��B</param>
		/// <param name="interpolateTime">�⊮����(�P�ʁF�b�B)</param>
		void PlayAnimation(const int animNo, const float interpolateTime = 0.0f)
		{
			m_animation.Play(animNo, interpolateTime);
		}

		/// <summary>
		/// �A�j���[�V�����̍Đ����H
		/// </summary>
		const bool IsPlayingAnimation() const
		{
			return m_animation.IsPlaying();
		}

		/// <summary>
		/// �A�j���[�V�����Đ��̑��x��ݒ肷��B
		/// </summary>
		/// <param name="animationSpeed">���l�̕������{�ɂ���B</param>
		void SetAnimationSpeed(const float animationSpeed)
		{
			m_animationSpeed = animationSpeed;
		}

	private:
		/// <summary>
		/// �X�P���g���̏������B
		/// </summary>
		/// <param name="filePath">�t�@�C���p�X�B</param>
		void InitSkeleton(const char* filePath);

		/// <summary>
		/// �A�j���[�V�����̏������B
		/// </summary>
		/// <param name="animationClips">�A�j���[�V�����N���b�v�B</param>
		/// <param name="numAnimationClips">�A�j���[�V�����N���b�v�̐��B</param>
		/// <param name="enModelUpAxis">���f���̏�����B</param>
		void InitAnimation(
			AnimationClip* animationClips,
			const int numAnimationClips,
			const EnModelUpAxis enModelUpAxis
		);
		/// <summary>
		/// �t�H���[�h�����_�����O�p�̃��f����������
		/// </summary>
		/// <param name="tkmFilePath"></param>
		/// <param name="enModelUpAxis"></param>
		/// <param name="isShadowReciever"></param>
		void InitModelOnFowardRendering(
			const char* tkmFilePath,
			const EnModelUpAxis enModelUpAxis,
			const bool isShadowReciever
		);

		void InitModelOnShadowMap(
			const char* tkmFilePath,
			EnModelUpAxis modelUpAxis,
			bool isFrontCullingOnDrawShadowMap
		);
		/// <summary>
		/// ���_�V�F�[�_�[�̃G���g���|�|�C���g��ݒ�
		/// </summary>
		/// <param name="initData">���f���������f�[�^</param>
		void SetupVertexShaderEntryPointFunc(ModelInitData& modelInitData);
		/// <summary>
		/// �t�H���[�h�����_�����O�ł̃��f���`��
		/// </summary>
		/// <param name="rc"></param>
		void OnForwardRender(RenderContext& rc) override;

	//�����o�ϐ�
	private:
		Skeleton					m_skeleton;									//�X�P���g��
		AnimationClip*				m_animationClips	= nullptr;				//�A�j���[�V�����N���b�v�B
		int							m_numAnimationClips = 0;					//�A�j���[�V�����N���b�v�̐��B
		Animation					m_animation;								//�A�j���[�V�����B
		float						m_animationSpeed	= 1.0f;					//�A�j���[�V�����X�s�[�h
		Vector3						m_position			= Vector3::Zero;		//���W
		Vector3						m_scale				= Vector3::One;			//�傫��
		Quaternion					m_rotation			= Quaternion::Identity;	//��]
		Model						m_model[MAX_VIEWPORT];						//Model�N���X
		Model						m_shadowModel[MAX_VIEWPORT];				//�V���h�E�}�b�v�`��p
	};

}