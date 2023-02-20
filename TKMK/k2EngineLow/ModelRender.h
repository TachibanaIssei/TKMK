#pragma once

namespace nsK2EngineLow {

	class ModelRender
	{
	public:
		void Init(
			const char* tkmFilepath,
			AnimationClip* animationClips,
			int numAnimationClips,
			EnModelUpAxis enModelUpAxis
		)
		{
			m_modelInitData.m_tkmFilePath = tkmFilepath;
			m_modelInitData.m_fxFilePath = "Assets/shader/model.fx";
			m_modelInitData.m_modelUpAxis = enModelUpAxis;
			
			// �X�P���g�����������B
			InitSkeleton(tkmFilepath);
			// �A�j���[�V�������������B
			InitAnimation(animationClips, numAnimationClips, enModelUpAxis);
			
			if (m_animationClips != nullptr)
			{
				//�X�P���g�����w�肷��
				m_modelInitData.m_skeleton = &m_skeleton;
			}

			m_model.Init(m_modelInitData);
		};

		void SetPosition(Vector3 position)
		{
			m_position = position;
		};

		void SetRotation(Quaternion rotation)
		{
			m_rotation = rotation;
		}

		void SetScale(Vector3 scale)
		{
			m_scale = scale;
		}

		void Update()
		{
			m_model.UpdateWorldMatrix(m_position,m_rotation, m_scale);
			if (m_skeleton.IsInited())
			{
				m_skeleton.Update(m_model.GetWorldMatrix());
			}
			m_animation.Progress(g_gameTime->GetFrameDeltaTime() * m_animationSpeed);
		};

		void Draw(RenderContext& rc)
		{
			m_model.Draw(rc);
		};

		/// <summary>
		/// �A�j���[�V�����Đ��B
		/// </summary>
		/// <param name="animNo">�A�j���[�V�����N���b�v�̔ԍ��B</param>
		/// <param name="interpolateTime">�⊮����(�P�ʁF�b�B)</param>
		void PlayAnimation(int animNo, float interpolateTime = 0.0f)
		{
			m_animation.Play(animNo, interpolateTime);
		}

		/// <summary>
		/// �A�j���[�V�����̍Đ����H
		/// </summary>
		bool IsPlayingAnimation() const
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
		void InitSkeleton(const char* filePath)
		{
			//�X�P���g���̃f�[�^��ǂݍ��݁B
			std::string skeletonFilePath = filePath;
			int pos = (int)skeletonFilePath.find(".tkm");
			skeletonFilePath.replace(pos, 4, ".tks");
			m_skeleton.Init(skeletonFilePath.c_str());
		}

		/// <summary>
		/// �A�j���[�V�����̏������B
		/// </summary>
		/// <param name="animationClips">�A�j���[�V�����N���b�v�B</param>
		/// <param name="numAnimationClips">�A�j���[�V�����N���b�v�̐��B</param>
		/// <param name="enModelUpAxis">���f���̏�����B</param>
		void InitAnimation(AnimationClip* animationClips,
			int numAnimationClips,
			EnModelUpAxis enModelUpAxis)
		{
			m_animationClips = animationClips;
			m_numAnimationClips = numAnimationClips;
			if (m_animationClips != nullptr) {
				m_animation.Init(m_skeleton,
					m_animationClips,
					numAnimationClips);
			}
		}

	private:
		AnimationClip* m_animationClips = nullptr;		// �A�j���[�V�����N���b�v�B
		int	m_numAnimationClips = 0;					// �A�j���[�V�����N���b�v�̐��B
		Animation m_animation;							// �A�j���[�V�����B
		Vector3 m_position = Vector3(0.0f,0.0f,0.0f);
		Vector3 m_scale = Vector3::One;
		Quaternion m_rotation = Quaternion::Identity;
		Model m_model;
		ModelInitData m_modelInitData;
		bool						m_isUpdateAnimation = true;
		Skeleton					m_skeleton;
		float						m_animationSpeed = 1.0f;
	};

}