#include "k2EngineLowPreCompile.h"
#include "RenderingEngine.h"

void nsK2EngineLow::RenderingEngine::Init()
{
	InitRenderTargets();
	m_postEffect.InitBloom(m_mainRenderTarget);
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

	//シャドウマップ描画用のレンダリングターゲット
	m_shadowMap.Create(
		1024,
		1024,
		1,
		1,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_FORMAT_D32_FLOAT,
		m_clearColor
	);

	m_lightCamera.SetPosition(0, 600, 0);
	m_lightCamera.SetTarget(0, 0, 0);
	m_lightCamera.SetUp(1, 0, 0);
	m_lightCamera.SetViewAngle(Math::DegToRad(20.0f));
	m_lightCamera.Update();
}

void nsK2EngineLow::RenderingEngine::InitCopyToFrameBufferSprite()
{
	SpriteInitData spriteInitData;

	//テクスチャはmainRenderTargetのカラーバッファ
	spriteInitData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();
	spriteInitData.m_width = m_mainRenderTarget.GetWidth();
	spriteInitData.m_height = m_mainRenderTarget.GetHeight();

	spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";

	//スプライトを初期化
	m_copyToFrameBufferSprite.Init(spriteInitData);

	// シャドウマップを表示するためのスプライトを初期化する
	SpriteInitData shadowInitData;
	shadowInitData.m_textures[0] = &m_shadowMap.GetRenderTargetTexture();
	shadowInitData.m_fxFilePath = "Assets/shader/sprite.fx";
	shadowInitData.m_width = 256;
	shadowInitData.m_height = 256;

	m_shadowSprite.Init(shadowInitData);
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

void nsK2EngineLow::RenderingEngine::SpriteRendering(RenderContext& rc)
{
	for (auto& spriteObj : m_spriteList)
	{
		spriteObj->OnRenderSprite(rc);
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

	rc.WaitUntilToPossibleSetRenderTarget(m_shadowMap);
	rc.SetRenderTargetAndViewport(m_shadowMap);
	rc.ClearRenderTargetView(m_shadowMap);
	//影モデルを描画
	ShadowModelRendering(rc,m_lightCamera);
	rc.WaitUntilFinishDrawingToRenderTarget(m_shadowMap);

	//メインレンダリングターゲットに変更
	rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
	rc.SetRenderTargetAndViewport(m_mainRenderTarget);
	rc.ClearRenderTargetView(m_mainRenderTarget);

	//モデルを描画
	ModelRendering(rc);

	//レンダリングターゲットへの書き込み終了待ち
	rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);

	m_postEffect.Render(rc, m_mainRenderTarget);

	rc.SetRenderTarget(
		g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
		g_graphicsEngine->GetCurrentFrameBuffuerDSV()
	);
	m_copyToFrameBufferSprite.Draw(rc);

	//スプライトを描画
	SpriteRendering(rc);
	//文字を描画
	FontRendering(rc);

	//シャドウマップを描画
	m_shadowSprite.Update({ FRAME_BUFFER_W / -2.0f, FRAME_BUFFER_H / 2.0f,  0.0f }, g_quatIdentity, g_vec3One, { 0.0f, 1.0f });
	m_shadowSprite.Draw(rc);

	m_modelList.clear();
	m_spriteList.clear();
	m_fontList.clear();
}