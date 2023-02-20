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
			
			// スケルトンを初期化。
			InitSkeleton(tkmFilepath);
			// アニメーションを初期化。
			InitAnimation(animationClips, numAnimationClips, enModelUpAxis);
			
			if (m_animationClips != nullptr)
			{
				//スケルトンを指定する
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
		/// アニメーション再生。
		/// </summary>
		/// <param name="animNo">アニメーションクリップの番号。</param>
		/// <param name="interpolateTime">補完時間(単位：秒。)</param>
		void PlayAnimation(int animNo, float interpolateTime = 0.0f)
		{
			m_animation.Play(animNo, interpolateTime);
		}

		/// <summary>
		/// アニメーションの再生中？
		/// </summary>
		bool IsPlayingAnimation() const
		{
			return m_animation.IsPlaying();
		}

		/// <summary>
		/// アニメーション再生の速度を設定する。
		/// </summary>
		/// <param name="animationSpeed">数値の分だけ倍にする。</param>
		void SetAnimationSpeed(const float animationSpeed)
		{
			m_animationSpeed = animationSpeed;
		}

	private:
		/// <summary>
		/// スケルトンの初期化。
		/// </summary>
		/// <param name="filePath">ファイルパス。</param>
		void InitSkeleton(const char* filePath)
		{
			//スケルトンのデータを読み込み。
			std::string skeletonFilePath = filePath;
			int pos = (int)skeletonFilePath.find(".tkm");
			skeletonFilePath.replace(pos, 4, ".tks");
			m_skeleton.Init(skeletonFilePath.c_str());
		}

		/// <summary>
		/// アニメーションの初期化。
		/// </summary>
		/// <param name="animationClips">アニメーションクリップ。</param>
		/// <param name="numAnimationClips">アニメーションクリップの数。</param>
		/// <param name="enModelUpAxis">モデルの上向き。</param>
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
		AnimationClip* m_animationClips = nullptr;		// アニメーションクリップ。
		int	m_numAnimationClips = 0;					// アニメーションクリップの数。
		Animation m_animation;							// アニメーション。
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