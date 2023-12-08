#include "k2EngineLowPreCompile.h"
#include "ModelRender.h"

namespace nsK2EngineLow
{
	void ModelRender::Init(const char* tkmFilepath, AnimationClip* animationClips, const int numAnimationClips, const EnModelUpAxis enModelUpAxis, const bool isShadowReceiver, const bool isFrontCullingOnDrawShadowMap)
	{
		// スケルトンを初期化。
		InitSkeleton(tkmFilepath);
		// アニメーションを初期化。
		InitAnimation(animationClips, numAnimationClips, enModelUpAxis);
		InitModelOnFowardRendering(tkmFilepath, enModelUpAxis, isShadowReceiver);
		
		InitModelOnShadowMap(tkmFilepath, enModelUpAxis, isFrontCullingOnDrawShadowMap);
	}

	void ModelRender::InitSkyCube(ModelInitData& initData)
	{
		initData.m_colorBufferFormat[0] = DXGI_FORMAT_R16G16B16A16_FLOAT;
		for (int i = 0; i < MAX_VIEWPORT; i++)
		{
			m_model[i].Init(initData);
			m_model[i].UpdateWorldMatrix(m_position, m_rotation, m_scale);
		}

	}

	void ModelRender::Update()
	{
		for (int i = 0; i < MAX_VIEWPORT; i++)
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
				//g_renderingEngine->SetmLVP(i, g_renderingEngine->GetLightCamera(i).GetViewProjectionMatrix());
			}
		}

		//アニメーションを進める
		m_animation.Progress(g_gameTime->GetFrameDeltaTime() * m_animationSpeed);
	}

	void ModelRender::Draw(RenderContext& rc)
	{
		g_renderingEngine->AddRenderObject(this);
	}

	void ModelRender::OnForwardRender(RenderContext& rc)
	{
		if (m_model[g_renderingEngine->GetCameraDrawing()].IsInited())
		{
			m_model[g_renderingEngine->GetCameraDrawing()].Draw(
				rc, 1, g_renderingEngine->GetCameraDrawing()
			);
		}
	}

	void ModelRender::InitSkeleton(const char* filePath)
	{
		//スケルトンのデータを読み込み。
		std::string skeletonFilePath = filePath;
		int pos = (int)skeletonFilePath.find(".tkm");
		skeletonFilePath.replace(pos, 4, ".tks");
		m_skeleton.Init(skeletonFilePath.c_str());
	}

	void ModelRender::InitAnimation(AnimationClip* animationClips, const int numAnimationClips, const EnModelUpAxis enModelUpAxis)
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

	void ModelRender::InitModelOnFowardRendering(const char* tkmFilePath, const EnModelUpAxis enModelUpAxis, const bool isShadowReciever)
	{
		ModelInitData modelInitData;
		modelInitData.m_fxFilePath = "Assets/shader/model.fx";

		//頂点シェーダーのエントリーポイント設定
		SetupVertexShaderEntryPointFunc(modelInitData);

		if (m_animationClips != nullptr)
		{
			//スケルトンを指定する
			modelInitData.m_skeleton = &m_skeleton;
		}

		/*if (g_renderingEngine->IsSoftShadow())
		{
			modelInitData.m_psEntryPointFunc = "PSMainSoftShadow";
		}
		else
		{*/
			modelInitData.m_psEntryPointFunc = "PSMainHardShadow";
		//}

		modelInitData.m_modelUpAxis = enModelUpAxis;
		modelInitData.m_tkmFilePath = tkmFilePath;
		modelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R16G16B16A16_FLOAT;
		//モデルクラスの初期化
		for (int i = 0; i < MAX_VIEWPORT; i++)
		{
			//ライトの情報を定数バッファへ渡す
			modelInitData.m_expandConstantBuffer = &g_renderingEngine->GetSceneLight(i);
			modelInitData.m_expandConstantBufferSize = sizeof(g_renderingEngine->GetSceneLight(i));

			m_model[i].Init(modelInitData);
		}
	}

	void ModelRender::InitModelOnShadowMap(const char* tkmFilePath, EnModelUpAxis modelUpAxis, bool isFrontCullingOnDrawShadowMap)
	{
		ModelInitData modelInitData;
		modelInitData.m_tkmFilePath = tkmFilePath;
		modelInitData.m_modelUpAxis = modelUpAxis;
		if (isFrontCullingOnDrawShadowMap)
		{
			//表カリングを行う
			modelInitData.m_cullMode = D3D12_CULL_MODE_FRONT;
		}
		SetupVertexShaderEntryPointFunc(modelInitData);

		if (m_animationClips != nullptr)
		{
			modelInitData.m_skeleton = &m_skeleton;
		}
		//modelInitData.m_fxFilePath = "Assets/shader/preProcess/DrawShadowMap.fx";

		/*if (g_renderingEngine->IsSoftShadow())
		{
			modelInitData.m_colorBufferFormat[0] = g_softShadowMapFormat.colorBufferFormat;
		}
		else
		{
			modelInitData.m_colorBufferFormat[0] = g_hardShadowMapFormat.colorBufferFormat;
		}*/

		/*for (int ligNo = 0; ligNo < MAX_DIRECTIONAL_LIGHT; ligNo++)
		{
			Model* shadowModelArray = m_shadowModels[ligNo];
			for (int shadowMapNo = 0; shadowMapNo < NUM_SHADOW_MAP; shadowMapNo++)
			{
				shadowModelArray[shadowMapNo].Init(modelInitData);
			}
		}*/
	}

	void ModelRender::SetupVertexShaderEntryPointFunc(ModelInitData& modelInitData)
	{
		modelInitData.m_vsSkinEntryPointFunc = "VSMain";
		modelInitData.m_vsEntryPointFunc = "VSMain";

		if (m_animationClips != nullptr) {
			// アニメーションあり。
			modelInitData.m_vsSkinEntryPointFunc = "VSMainSkin";
		}
	}
}