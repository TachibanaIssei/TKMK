#include "k2EngineLowPreCompile.h"
#include "RenderingEngine.h"

void nsK2EngineLow::RenderingEngine::Init()
{
	InitViewPorts();
	InitRenderTargets();
	m_shadow.Init();
	m_postEffect.Init(m_mainRenderTarget);
	InitCopyToFrameBufferSprite();

	m_sceneLight.Init();
}

void nsK2EngineLow::RenderingEngine::InitRenderTargets()
{
	//メインレンダリングターゲット
	m_mainRenderTarget.Create(
		FRAME_BUFFER_W,
		FRAME_BUFFER_H,
		1,
		1,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT_D32_FLOAT
	);
}

void nsK2EngineLow::RenderingEngine::InitCopyToFrameBufferSprite()
{
	SpriteInitData spriteInitData;

	//メインレンダリングターゲットのテクスチャ
	spriteInitData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();
	spriteInitData.m_width = m_mainRenderTarget.GetWidth();
	spriteInitData.m_height = m_mainRenderTarget.GetHeight();

	spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";

	m_copyToFrameBufferSprite.Init(spriteInitData);
}

void nsK2EngineLow::RenderingEngine::InitViewPorts()
{
	//左の画面
	m_viewPorts[0].Width = FRAME_BUFFER_W / 2;		//画面の横サイズ
	m_viewPorts[0].Height = FRAME_BUFFER_H;	//画面の縦サイズ
	m_viewPorts[0].TopLeftX = 0;					//画面左上のx座標
	m_viewPorts[0].TopLeftY = 0;					//画面左上のy座標
	m_viewPorts[0].MinDepth = 0.0f;					//深度値の最小値
	m_viewPorts[0].MaxDepth = 1.0f;					//深度値の最大値

	//右の画面
	m_viewPorts[1].Width = FRAME_BUFFER_W / 2;		//画面の横サイズ
	m_viewPorts[1].Height = FRAME_BUFFER_H;	//画面の縦サイズ
	m_viewPorts[1].TopLeftX = FRAME_BUFFER_W / 2;	//画面左上のx座標
	m_viewPorts[1].TopLeftY = 0;					//画面左上のy座標
	m_viewPorts[1].MinDepth = 0.0f;					//深度値の最小値
	m_viewPorts[1].MaxDepth = 1.0f;					//深度値の最大値
}

void nsK2EngineLow::RenderingEngine::DrawModelInViewPorts(RenderContext& rc)
{
	if (m_isSplitScreen)
	{
		for (int i = 0; i < m_viewPortCount; i++)
		{
			rc.SetViewport(m_viewPorts[i]);
			//モデル描画
			ModelRendering(rc);
		}
	}
	else
	{
		ModelRendering(rc);
	}
}

void nsK2EngineLow::RenderingEngine::ModelRendering(RenderContext& rc)
{
	for (auto& modelObj : m_modelList) {
		modelObj->OnRenderModel(rc);
	}
}

void nsK2EngineLow::RenderingEngine::ShadowModelRendering(RenderContext& rc, Camera& camera)
{
	for (auto& modelObj : m_modelList) {
		modelObj->OnRenderShadowModel(rc,camera);
	}
}

void nsK2EngineLow::RenderingEngine::SpriteRendering(RenderContext& rc, bool drawTiming = false)
{
	if (drawTiming) {
		for (auto& spriteObj : m_laterSpriteList)
		{
			spriteObj->OnRenderSprite(rc);
		}
	}
	else {
		for (auto& spriteObj : m_spriteList)
		{
			spriteObj->OnRenderSprite(rc);
		}
	}
}

void nsK2EngineLow::RenderingEngine::FontRendering(RenderContext& rc)
{
	for (auto& fontObj : m_fontList)
	{
		fontObj->OnRenderFont(rc);
	}
}

void nsK2EngineLow::RenderingEngine::Execute(RenderContext& rc)
{
	SetEyePos(g_camera3D->GetPosition());

	m_shadow.Render(rc);

	//レンダリングターゲットを変更
	rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
	rc.SetRenderTargetAndViewport(m_mainRenderTarget);
	rc.ClearRenderTargetView(m_mainRenderTarget);

	DrawModelInViewPorts(rc);

	//書き込み終了待ち
	rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);

	m_postEffect.Render(rc, m_mainRenderTarget);

	rc.SetRenderTarget(
		g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
		g_graphicsEngine->GetCurrentFrameBuffuerDSV()
	);
	m_copyToFrameBufferSprite.Draw(rc);

	//画像の描画
	SpriteRendering(rc);
	//文字の描画
	FontRendering(rc);
	//文字の上に画像を描画
	SpriteRendering(rc, true);

	m_modelList.clear();
	m_spriteList.clear();
	m_laterSpriteList.clear();
	m_fontList.clear();
}