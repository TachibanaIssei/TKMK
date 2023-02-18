#pragma once
class ModelRender
{
public:
	ModelRender();
	~ModelRender();

	/// <summary>
	/// 初期化　通常はこの関数で初期化する
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	/// <param name="animationClips">アニメーションクリップ</param>
	/// <param name="numAnimationClips">アニメーションクリップの数</param>
	/// <param name="enModelUpAxis">モデルの上方向</param>
	/// <param name="isShadowReciever">シャドウレシーバーフラグがtrueなら影が落ちる</param>
	void Init(
		const char* filePath,
		AnimationClip* animationClips = nullptr,
		int numAnimationClips = 0,
		EnModelUpAxis enModelUpAxis = enModelUpAxisZ,
		bool isShadowReciever = true
	);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// アニメーションを再生する
	/// </summary>
	/// <param name="animNo">アニメーションクリップの番号</param>
	/// <param name="interpolateTime">補完時間(単位:秒)</param>
	void PlayAnimation(int animNo, float interpolateTime = 0.0f)
	{
		m_animation.Play(animNo, interpolateTime);
	}

	/// <summary>
	/// アニメーションの再生中か判定する
	/// </summary>
	/// <returns>再生中だったらtrue</returns>
	bool IsPlayingAnimation() const
	{
		return m_animation.IsPlaying();
	}

private:
	/// <summary>
	/// アニメーション済み頂点バッファの計算処理を初期化
	/// </summary>
	/// <param name="tkmFilePath">ファイルパス</param>
	/// <param name="enModelUpAxis">モデルの上軸</param>
	void InitComputeAnimationVertexBuffer(
		const char* tkmFilePath,
		EnModelUpAxis enModelUpAxis
	);

	//todo RenderingEngineについて見ていく
	void InitModelOnRenderGBuffer(

	);

private:
	Animation m_animation;	//アニメーション
};

