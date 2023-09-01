#include "k2EngineLowPreCompile.h"
#include "RenderingEngine.h"

namespace
{
	const int MAX_MODEL = 200;
	const int MAX_SPRITE = 100;
	const int MAX_FONT = 100;
}

nsK2EngineLow::RenderingEngine::RenderingEngine()
{
	m_modelList.reserve(MAX_MODEL);
	m_spriteList.reserve(MAX_SPRITE);
	m_laterSpriteList.reserve(MAX_SPRITE);
	m_fontList.reserve(MAX_FONT);
}

nsK2EngineLow::RenderingEngine::~RenderingEngine()
{
}

void nsK2EngineLow::RenderingEngine::Init()
{
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

	//テクスチャはmainRenderTargetのカラーバッファ
	spriteInitData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();
	spriteInitData.m_width = m_mainRenderTarget.GetWidth();
	spriteInitData.m_height = m_mainRenderTarget.GetHeight();

	spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";

	//スプライトを初期化
	m_copyToFrameBufferSprite.Init(spriteInitData);
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
	//スプライトを文字の上に描画する
	SpriteRendering(rc, true);

	m_modelList.clear();
	m_spriteList.clear();
	m_laterSpriteList.clear();
	m_fontList.clear();
}