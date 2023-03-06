#include "k2EngineLowPreCompile.h"
#include "ModelRender.h"

void nsK2EngineLow::ModelRender::Init(const char* tkmFilepath, AnimationClip* animationClips, int numAnimationClips, EnModelUpAxis enModelUpAxis)
{
	//tkmファイルパスを設定
	m_modelInitData.m_tkmFilePath = tkmFilepath;
	//シェーダーファイルパスを設定
	m_modelInitData.m_fxFilePath = "Assets/shader/model.fx";
	//モデルの上方向を設定
	m_modelInitData.m_modelUpAxis = enModelUpAxis;

	// スケルトンを初期化。
	InitSkeleton(tkmFilepath);
	// アニメーションを初期化。
	InitAnimation(animationClips, numAnimationClips, enModelUpAxis);

	//アニメーションが設定されていたら。
	if (m_animationClips != nullptr)
	{
		//スケルトンを指定する
		m_modelInitData.m_skeleton = &m_skeleton;
		//スキンがある用の頂点シェーダーを設定する。
		m_modelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
	}
	
	//ディレクションライトの情報を作成
	MakeDirectionData();

	//モデルクラスの初期化
	m_model.Init(m_modelInitData);
}

void nsK2EngineLow::ModelRender::Update()
{
	//ワールド行列の更新(座標、回転、大きさ)
	m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);

	//スケルトンが初期化されていたら
	if (m_skeleton.IsInited())
	{
		//スケルトンを更新する
		m_skeleton.Update(m_model.GetWorldMatrix());
	}

	//アニメーションを進める
	m_animation.Progress(g_gameTime->GetFrameDeltaTime() * m_animationSpeed);
}

void nsK2EngineLow::ModelRender::MakeDirectionData()
{
	m_modelInitData.m_expandConstantBuffer = &g_renderingEngine->GetSceneLight();
	m_modelInitData.m_expandConstantBufferSize = sizeof(g_renderingEngine->GetSceneLight());
}

void nsK2EngineLow::ModelRender::InitSkeleton(const char* filePath)
{
	//スケルトンのデータを読み込み。
	std::string skeletonFilePath = filePath;
	int pos = (int)skeletonFilePath.find(".tkm");
	skeletonFilePath.replace(pos, 4, ".tks");
	m_skeleton.Init(skeletonFilePath.c_str());
}

void nsK2EngineLow::ModelRender::InitAnimation(AnimationClip* animationClips, int numAnimationClips, EnModelUpAxis enModelUpAxis)
{
	m_animationClips = animationClips;
	m_numAnimationClips = numAnimationClips;

	if (m_animationClips != nullptr) {
		//アニメーションの初期化
		m_animation.Init(
			m_skeleton,
			m_animationClips,
			numAnimationClips
		);
	}
}
