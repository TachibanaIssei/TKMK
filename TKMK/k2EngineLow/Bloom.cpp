#include "k2EngineLowPreCompile.h"
#include "Bloom.h"

void nsK2EngineLow::Bloom::Init(RenderTarget& mainRenderTarget)
{
	InitRenderTarget(mainRenderTarget);
	InitLuminanceSprite(mainRenderTarget);
	InitGaussianBlur();
	InitFinalSprite(mainRenderTarget);
}

void nsK2EngineLow::Bloom::InitRenderTarget(RenderTarget& mainRenderTarget)
{
	//輝度抽出用のレンダリングターゲット
	m_luminnceRenderTarget.Create(
		mainRenderTarget.GetWidth(),				//メインレンダリングターゲットと同じ
		mainRenderTarget.GetHeight(),				//メインレンダリングターゲットと同じ
		1,
		1,
		mainRenderTarget.GetColorBufferFormat(),
		DXGI_FORMAT_D32_FLOAT
	);
}

void nsK2EngineLow::Bloom::InitLuminanceSprite(RenderTarget& mainRenderTarget)
{
	SpriteInitData luminanceSpriteInitData;
	luminanceSpriteInitData.m_fxFilePath = "Assets/shader/postEffect/postEffect.fx";
	luminanceSpriteInitData.m_vsEntryPointFunc = "VSMain";
	luminanceSpriteInitData.m_psEntryPoinFunc = "PSSamplingLuminance";

	//スプライトの幅と高さはluminnceRenderTargetと同じ
	luminanceSpriteInitData.m_width = m_luminnceRenderTarget.GetWidth();
	luminanceSpriteInitData.m_height = m_luminnceRenderTarget.GetHeight();

	//テクスチャはメインレンダリングターゲットのカラーバッファ
	luminanceSpriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
	//書き込むレンダリングターゲットのフォーマットを指定
	luminanceSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;

	//スプライトを初期化する
	m_luminanceSprite.Init(luminanceSpriteInitData);
}

void nsK2EngineLow::Bloom::InitFinalSprite(RenderTarget& mainRenderTarget)
{
	SpriteInitData finalSpriteInitData;
	finalSpriteInitData.m_textures[0] = &m_gaussianBlur.GetBokeTexture();

	//mainRenderTargetとおなじ幅と高さ
	finalSpriteInitData.m_width = mainRenderTarget.GetWidth();
	finalSpriteInitData.m_height = mainRenderTarget.GetHeight();

	finalSpriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";

	//加算合成で描画するのでブレンディングモードを加算にする
	finalSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Add;

	finalSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;

	//スプライトを初期化
	m_finalSprite.Init(finalSpriteInitData);
}

void nsK2EngineLow::Bloom::InitGaussianBlur()
{
	m_gaussianBlur.Init(&m_luminnceRenderTarget.GetRenderTargetTexture());
}

void nsK2EngineLow::Bloom::OnRender(RenderContext& rc, RenderTarget& mainRenderTarget)
{
	//輝度抽出用のレンダリングターゲットに変更
	rc.WaitUntilToPossibleSetRenderTarget(m_luminnceRenderTarget);
	rc.SetRenderTargetAndViewport(m_luminnceRenderTarget);
	rc.ClearRenderTargetView(m_luminnceRenderTarget);

	//輝度抽出を行う
	m_luminanceSprite.Draw(rc);

	//レンダリングエンジンへの書き込み終了待ち
	rc.WaitUntilFinishDrawingToRenderTarget(m_luminnceRenderTarget);

	//ガウシアンブラーを実行する
	m_gaussianBlur.ExecuteOnGPU(rc, 20);

	//ボケ画像をメインレンダリングターゲットに加算合成
	rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
	rc.SetRenderTargetAndViewport(mainRenderTarget);

	//最終合成
	m_finalSprite.Draw(rc);

	//レンダリングターゲットへの書き込み終了待ち
	rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);
}
