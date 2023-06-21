#include "k2EngineLowPreCompile.h"
#include "ModelRender.h"

void nsK2EngineLow::ModelRender::Init(const char* tkmFilepath, AnimationClip* animationClips, int numAnimationClips, EnModelUpAxis enModelUpAxis, bool shadow)
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
	
	//モデルクラスの初期化
	for (int i = 0; i < 2; i++)
	{
		//ディレクションライトの情報を作成
		MakeDirectionData(i);
		m_model[i].Init(m_modelInitData);
	}

	for(int i=0;i<2;i++){
		//シャドウマップ描画用のモデルの初期化
		if (shadow)
		{
			m_modelInitData.m_psEntryPointFunc = "PSShadowMapMain";
			m_modelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32_FLOAT;

			m_shadowModel[i].Init(m_modelInitData);
		}

	}

}

void nsK2EngineLow::ModelRender::InitBackGround(const char* tkmFilepath)
{
	m_modelInitData.m_tkmFilePath = tkmFilepath;
	m_modelInitData.m_fxFilePath = "Assets/shader/ShadowReciever.fx";

	for (int i = 0; i < 2; i++)
	{
		m_modelInitData.m_expandShaderResoruceView[0] = &g_renderingEngine->GetShadowMapTexture(i);
		//ライトカメラのビュープロジェクション行列を設定する
		g_renderingEngine->SetmLVP(i, g_renderingEngine->GetLightCamera(i).GetViewProjectionMatrix());
		MakeDirectionData(i);
		m_model[i].Init(m_modelInitData);
	}
}

void nsK2EngineLow::ModelRender::InitSkyCube(ModelInitData& initData)
{
	initData.m_colorBufferFormat[0] = DXGI_FORMAT_R16G16B16A16_FLOAT;
	for (int i = 0; i < 2; i++)
	{
		m_model[i].Init(initData);
		m_model[i].UpdateWorldMatrix(m_position, m_rotation, m_scale);
	}

}

void nsK2EngineLow::ModelRender::Update()
{
	for (int i = 0; i < 2; i++)
	{
		//ワールド行列の更新(座標、回転、大きさ)
		m_model[i].UpdateWorldMatrix(m_position, m_rotation, m_scale);

		//スケルトンが初期化されていたら
		if (m_skeleton.IsInited())
		{
			//スケルトンを更新する
			m_skeleton.Update(m_model[i].GetWorldMatrix());
		}

		//シャドウモデルが初期化されていたら
		if (m_shadowModel[i].IsInited())
		{
			m_shadowModel[i].UpdateWorldMatrix(m_position, m_rotation, m_scale);
			g_renderingEngine->SetmLVP(i, g_renderingEngine->GetLightCamera(i).GetViewProjectionMatrix());
		}
	}

	//アニメーションを進める
	m_animation.Progress(g_gameTime->GetFrameDeltaTime() * m_animationSpeed);
}

void nsK2EngineLow::ModelRender::MakeDirectionData(int lightNumber)
{
	m_modelInitData.m_expandConstantBuffer = &g_renderingEngine->GetSceneLight(lightNumber);
	m_modelInitData.m_expandConstantBufferSize = sizeof(g_renderingEngine->GetSceneLight(lightNumber));
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
