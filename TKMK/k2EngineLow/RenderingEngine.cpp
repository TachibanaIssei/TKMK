#include "k2EngineLowPreCompile.h"
#include "RenderingEngine.h"

namespace {
	const UINT FRAME_BUFFER_WIDTH_HALF = FRAME_BUFFER_W / 2;	//画面分割用のビューポートで使用する横幅
}

void nsK2EngineLow::RenderingEngine::Init()
{
	InitViewPorts();
	InitRenderTargets();
	m_shadow.Init();
	m_postEffect.Init(m_mainRenderTarget);
	InitCopyToFrameBufferSprite();

	m_sceneLight[enCameraDrawing_Left].Init();
	m_sceneLight[enCameraDrawing_Right].Init();
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

void nsK2EngineLow::RenderingEngine::EffectBeginRender()
{
	if (GetSplitScreenFlag())
	{
		EffectEngine::GetInstance()->BeginFrame(0);
		EffectEngine::GetInstance()->BeginFrame(1);
	}
	else {
		EffectEngine::GetInstance()->BeginFrame(0);
	}
}

void nsK2EngineLow::RenderingEngine::InitViewPorts()
{
	m_soloViewPort.Width = FRAME_BUFFER_W;		//画面の横サイズ
	m_soloViewPort.Height = FRAME_BUFFER_H;		//画面の縦サイズ
	m_soloViewPort.TopLeftX = 0;				//画面左上のx座標
	m_soloViewPort.TopLeftY = 0;				//画面左上のy座標
	m_soloViewPort.MinDepth = 0.0f;				//深度値の最小値
	m_soloViewPort.MaxDepth = 1.0f;				//深度値の最大値

	//左の画面
	m_viewPorts[enCameraDrawing_Left].Width		= FRAME_BUFFER_WIDTH_HALF;	//画面の横サイズ
	m_viewPorts[enCameraDrawing_Left].Height	= FRAME_BUFFER_H;			//画面の縦サイズ
	m_viewPorts[enCameraDrawing_Left].TopLeftX	= 0;						//画面左上のx座標
	m_viewPorts[enCameraDrawing_Left].TopLeftY	= 0;						//画面左上のy座標
	m_viewPorts[enCameraDrawing_Left].MinDepth	= 0.0f;						//深度値の最小値
	m_viewPorts[enCameraDrawing_Left].MaxDepth	= 1.0f;						//深度値の最大値

	//右の画面
	m_viewPorts[enCameraDrawing_Right].Width	= FRAME_BUFFER_WIDTH_HALF;	//画面の横サイズ
	m_viewPorts[enCameraDrawing_Right].Height	= FRAME_BUFFER_H;			//画面の縦サイズ
	m_viewPorts[enCameraDrawing_Right].TopLeftX = FRAME_BUFFER_WIDTH_HALF;	//画面左上のx座標
	m_viewPorts[enCameraDrawing_Right].TopLeftY = 0;						//画面左上のy座標
	m_viewPorts[enCameraDrawing_Right].MinDepth = 0.0f;						//深度値の最小値
	m_viewPorts[enCameraDrawing_Right].MaxDepth = 1.0f;						//深度値の最大値
}

void nsK2EngineLow::RenderingEngine::DrawModelInViewPorts(RenderContext& rc)
{
	//画面分割をする
	if (m_isSplitScreen)
	{
		for (int i = 0; i < MAX_VIEWPORT; i++)
		{
			if (i == enCameraDrawing_Left)
			{
				m_cameraDrawing = enCameraDrawing_Left;
			}
			else if (i == enCameraDrawing_Right)
			{
				m_cameraDrawing = enCameraDrawing_Right;
			}

			rc.SetViewport(m_viewPorts[i]);
			//モデル描画
			ModelRendering(rc);
		}
	}
	//画面分割をしない
	else
	{
		rc.SetViewport(m_soloViewPort);
		m_cameraDrawing = enCameraDrawing_Solo;
		ModelRendering(rc);
	}
}

void nsK2EngineLow::RenderingEngine::ModelRendering(RenderContext& rc)
{
	for (auto& modelObj : m_modelList) {
		modelObj->OnRenderModel(rc);
	}
}

void nsK2EngineLow::RenderingEngine::ShadowModelRendering(RenderContext& rc, Camera& camera,int number)
{
	for (auto& modelObj : m_modelList) {
		modelObj->OnRenderShadowModel(rc,camera,number);
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

void nsK2EngineLow::RenderingEngine::ExcuteEffectRender(RenderContext& rc)
{
	if (GetSplitScreenFlag()) {
		g_camera2D->SetWidth(FRAME_BUFFER_WIDTH_HALF);
		EffectEngine::GetInstance()->Update(g_gameTime->GetFrameDeltaTime(), enCameraDrawing_Left);
		EffectEngine::GetInstance()->BeginDraw();
		//左画面
		{
			rc.SetViewport(m_viewPorts[enCameraDrawing_Left]);
			EffectEngine::GetInstance()->Draw(enCameraDrawing_Left);
		}
		//右画面
		{
			rc.SetViewport(m_viewPorts[enCameraDrawing_Right]);
			EffectEngine::GetInstance()->Draw(enCameraDrawing_Right);
		}
		EffectEngine::GetInstance()->EndDraw();
		//1画面のスプライトのアスペクト比に合わせる。
		g_camera2D->SetWidth(static_cast<float>(g_graphicsEngine->GetFrameBufferWidth()));
		//ビューポートを画面全体用に切り替える
		rc.SetViewportAndScissor(m_soloViewPort);

	}
	//1画面
	else {
		//1画面のスプライトのアスペクト比に合わせる。
		g_camera2D->SetWidth(static_cast<float>(g_graphicsEngine->GetFrameBufferWidth()));

		rc.SetViewport(m_soloViewPort);

		//1画面オンリーのエフェクト更新
		EffectEngine::GetInstance()->Update(g_gameTime->GetFrameDeltaTime(), enCameraDrawing_Solo);
		//1画面オンリーのエフェクト描画
		EffectEngine::GetInstance()->Draw(enCameraDrawing_Solo);
	}
	
}

void nsK2EngineLow::RenderingEngine::ClearVectorList()
{
	m_modelList.clear();
	m_spriteList.clear();
	m_laterSpriteList.clear();
	m_fontList.clear();
}

void nsK2EngineLow::RenderingEngine::Execute(RenderContext& rc)
{
	SetEyePos(g_camera3D[enCameraDrawing_Left]->GetPosition());

	m_shadow.Render(rc);

	//レンダリングターゲットを変更
	rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
	rc.SetRenderTargetAndViewport(m_mainRenderTarget);
	rc.ClearRenderTargetView(m_mainRenderTarget);

	DrawModelInViewPorts(rc);

	//書き込み終了待ち
	rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);
	
	m_postEffect.Render(rc, m_mainRenderTarget);

	ExcuteEffectRender(rc);

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

	ClearVectorList();
}