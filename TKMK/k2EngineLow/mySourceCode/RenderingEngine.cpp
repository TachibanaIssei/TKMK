#include "k2EngineLowPreCompile.h"
#include "RenderingEngine.h"

namespace {
	const UINT FRAME_BUFFER_WIDTH_HALF = FRAME_BUFFER_W / 2;	//画面分割用のビューポートで使用する横幅
	const UINT FRAME_BUFFER_HEIGHT_HALF = FRAME_BUFFER_H / 2;	//画面分割用のビューポートで使用する縦幅
}

namespace nsK2EngineLow
{
	void RenderingEngine::Init(const bool isSoftShadow)
	{
		m_isSoftShadow = isSoftShadow;

		InitViewPorts();
		InitRenderTargets();
		InitGBuffer();
		InitShadowMapRender();
		m_postEffect.Init(m_mainRenderTarget);
		InitCopyToFrameBufferSprite();
		InitDeferredLightingSprite();

		m_sceneLight.Init();
	}

	void RenderingEngine::InitRenderTargets()
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

		float clearColor[4] = { 0.0f,0.0f,0.0f,0.0f };

		m_2DRenderTarget.Create(
			UI_SPACE_WIDTH,
			UI_SPACE_HEIGHT,
			1,
			1,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_FORMAT_UNKNOWN,
			clearColor
		);

		//m_2DSpriteの初期化
		SpriteInitData spriteInitData;
		//テクスチャは2Dレンダ―ターゲット
		spriteInitData.m_textures[0] = &m_2DRenderTarget.GetRenderTargetTexture();
		// 解像度はmainRenderTargetの幅と高さ
		spriteInitData.m_width = m_mainRenderTarget.GetWidth();
		spriteInitData.m_height = m_mainRenderTarget.GetHeight();
		spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
		spriteInitData.m_vsEntryPointFunc = "VSMain";
		spriteInitData.m_psEntryPoinFunc = "PSMain";
		spriteInitData.m_alphaBlendMode = AlphaBlendMode_None;
		//レンダリングターゲットのフォーマット
		spriteInitData.m_colorBufferFormat[0] = m_mainRenderTarget.GetColorBufferFormat();
		m_2DSprite.Init(spriteInitData);

		//m_mainSpriteの初期化
		//テクスチャはメインレンダ―ターゲット
		spriteInitData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();
		//解像度は2Dレンダ―ターゲットの幅と高さ
		spriteInitData.m_width = m_2DRenderTarget.GetWidth();
		spriteInitData.m_height = m_2DRenderTarget.GetHeight();
		//レンダリングターゲットのフォーマット
		spriteInitData.m_colorBufferFormat[0] = m_2DRenderTarget.GetColorBufferFormat();
		m_mainSprite.Init(spriteInitData);
	}

	void RenderingEngine::InitCopyToFrameBufferSprite()
	{
		SpriteInitData spriteInitData;

		//メインレンダリングターゲットのテクスチャ
		spriteInitData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();
		spriteInitData.m_width = m_mainRenderTarget.GetWidth();
		spriteInitData.m_height = m_mainRenderTarget.GetHeight();

		spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";

		m_copyToFrameBufferSprite.Init(spriteInitData);
	}

	void RenderingEngine::EffectBeginRender()
	{
		if (m_gameMode == enGameMode_DuoPlay)
		{
			EffectEngine::GetInstance()->BeginFrame(enCameraDrawing_Left);
			EffectEngine::GetInstance()->BeginFrame(enCameraDrawing_Right);
		}
		else {
			EffectEngine::GetInstance()->BeginFrame(enCameraDrawing_Solo);
		}
	}

	void RenderingEngine::InitGBuffer()
	{
		int frameBuffer_w = g_graphicsEngine->GetFrameBufferWidth();
		int frameBuffer_h = g_graphicsEngine->GetFrameBufferHeight();

		// アルベドカラーを出力用のレンダリングターゲットを初期化する
		float clearColor[] = { 0.5f, 0.5f, 0.5f, 1.0f };
		m_gBuffer[enGBufferAlbedoDepth].Create(
			frameBuffer_w,
			frameBuffer_h,
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT,
			clearColor
		);

		// 法線出力用のレンダリングターゲットを初期化する
		m_gBuffer[enGBufferNormal].Create(
			frameBuffer_w,
			frameBuffer_h,
			1,
			1,
			DXGI_FORMAT_R8G8B8A8_SNORM,
			DXGI_FORMAT_UNKNOWN
		);

		// メタリック、影パラメータ、スムース出力用のレンダリングターゲットを初期化する    
		m_gBuffer[enGBufferMetaricShadowSmooth].Create(
			frameBuffer_w,
			frameBuffer_h,
			1,
			1,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_FORMAT_UNKNOWN
		);
	}

	void RenderingEngine::InitDeferredLightingSprite()
	{
		SpriteInitData spriteInitData;

		// 画面全体にレンダリングするので幅と高さはフレームバッファーの幅と高さと同じ
		spriteInitData.m_width = g_graphicsEngine->GetFrameBufferWidth();
		spriteInitData.m_height = g_graphicsEngine->GetFrameBufferHeight();

		// ディファードライティングで使用するテクスチャを設定
		int texNo = 0;
		for (auto& gBuffer : m_gBuffer)
		{
			spriteInitData.m_textures[texNo++] = &gBuffer.GetRenderTargetTexture();
		}
		spriteInitData.m_fxFilePath = "Assets/shader/DeferredLighting.fx";

		if (m_isSoftShadow)
		{
			spriteInitData.m_psEntryPoinFunc = "PSMainSoftShadow";
		}
		else
		{
			spriteInitData.m_psEntryPoinFunc = "PSMainHardShadow";
		}

		//ライトの情報を定数バッファに渡す
		spriteInitData.m_expandConstantBuffer = &m_lightingCB;
		spriteInitData.m_expandConstantBufferSize = sizeof(m_lightingCB);

		for (int viewportNo = 0; viewportNo < MAX_VIEWPORT; viewportNo++)
		{
			for (int areaNo = 0; areaNo < NUM_SHADOW_MAP; areaNo++)
			{
				spriteInitData.m_textures[texNo++] = &m_shadowMapRenders[viewportNo].GetShadowMap(areaNo);
			}
		}
		spriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R16G16B16A16_FLOAT;
		m_diferredLightingSprite.Init(spriteInitData);
	}

	void RenderingEngine::InitShadowMapRender()
	{
		for (auto& shadowMapRender : m_shadowMapRenders)
		{
			shadowMapRender.Init(m_isSoftShadow);
		}
	}

	void RenderingEngine::InitViewPorts()
	{
		//画面全体
		{
			m_soloViewPort.Width = FRAME_BUFFER_W;		//画面の横サイズ
			m_soloViewPort.Height = FRAME_BUFFER_H;		//画面の縦サイズ
			m_soloViewPort.TopLeftX = 0;				//画面左上のx座標
			m_soloViewPort.TopLeftY = 0;				//画面左上のy座標
			m_soloViewPort.MinDepth = 0.0f;				//深度値の最小値
			m_soloViewPort.MaxDepth = 1.0f;				//深度値の最大値
		}

		//2画面分割
		{
			//左の画面
			m_duoViewPorts[enCameraDrawing_Left].Width = FRAME_BUFFER_WIDTH_HALF - 2;
			m_duoViewPorts[enCameraDrawing_Left].Height = FRAME_BUFFER_H;
			m_duoViewPorts[enCameraDrawing_Left].TopLeftX = 0;
			m_duoViewPorts[enCameraDrawing_Left].TopLeftY = 0;
			m_duoViewPorts[enCameraDrawing_Left].MinDepth = 0.0f;
			m_duoViewPorts[enCameraDrawing_Left].MaxDepth = 1.0f;

			//右の画面
			m_duoViewPorts[enCameraDrawing_Right].Width = FRAME_BUFFER_WIDTH_HALF + 2;
			m_duoViewPorts[enCameraDrawing_Right].Height = FRAME_BUFFER_H;
			m_duoViewPorts[enCameraDrawing_Right].TopLeftX = FRAME_BUFFER_WIDTH_HALF;
			m_duoViewPorts[enCameraDrawing_Right].TopLeftY = 0;
			m_duoViewPorts[enCameraDrawing_Right].MinDepth = 0.0f;
			m_duoViewPorts[enCameraDrawing_Right].MaxDepth = 1.0f;
		}

		//4画面分割
		{
			//左上
			m_quarteViewPorts[enCameraDrawing_LeftUp].Width = FRAME_BUFFER_WIDTH_HALF - 2;
			m_quarteViewPorts[enCameraDrawing_LeftUp].Height = FRAME_BUFFER_HEIGHT_HALF - 2;
			m_quarteViewPorts[enCameraDrawing_LeftUp].TopLeftX = 0;
			m_quarteViewPorts[enCameraDrawing_LeftUp].TopLeftY = 0;
			m_quarteViewPorts[enCameraDrawing_LeftUp].MinDepth = 0.0f;
			m_quarteViewPorts[enCameraDrawing_LeftUp].MaxDepth = 1.0f;

			//右上
			m_quarteViewPorts[enCameraDrawing_RightUp].Width = FRAME_BUFFER_WIDTH_HALF + 2;
			m_quarteViewPorts[enCameraDrawing_RightUp].Height = FRAME_BUFFER_HEIGHT_HALF - 2;
			m_quarteViewPorts[enCameraDrawing_RightUp].TopLeftX = FRAME_BUFFER_WIDTH_HALF;
			m_quarteViewPorts[enCameraDrawing_RightUp].TopLeftY = 0;
			m_quarteViewPorts[enCameraDrawing_RightUp].MinDepth = 0.0f;
			m_quarteViewPorts[enCameraDrawing_RightUp].MaxDepth = 1.0f;

			//左下
			m_quarteViewPorts[enCameraDrawing_LeftDown].Width = FRAME_BUFFER_WIDTH_HALF - 2;
			m_quarteViewPorts[enCameraDrawing_LeftDown].Height = FRAME_BUFFER_HEIGHT_HALF + 2;
			m_quarteViewPorts[enCameraDrawing_LeftDown].TopLeftX = 0;
			m_quarteViewPorts[enCameraDrawing_LeftDown].TopLeftY = FRAME_BUFFER_HEIGHT_HALF;
			m_quarteViewPorts[enCameraDrawing_LeftDown].MinDepth = 0.0f;
			m_quarteViewPorts[enCameraDrawing_LeftDown].MaxDepth = 1.0f;

			//右下
			m_quarteViewPorts[enCameraDrawing_RightDown].Width = FRAME_BUFFER_WIDTH_HALF + 2;
			m_quarteViewPorts[enCameraDrawing_RightDown].Height = FRAME_BUFFER_HEIGHT_HALF + 2;
			m_quarteViewPorts[enCameraDrawing_RightDown].TopLeftX = FRAME_BUFFER_WIDTH_HALF;
			m_quarteViewPorts[enCameraDrawing_RightDown].TopLeftY = FRAME_BUFFER_HEIGHT_HALF;
			m_quarteViewPorts[enCameraDrawing_RightDown].MinDepth = 0.0f;
			m_quarteViewPorts[enCameraDrawing_RightDown].MaxDepth = 1.0f;
		}
	}

	void RenderingEngine::DrawModelInViewPorts(RenderContext& rc)
	{
		//2画面分割に描画
		if (m_gameMode == enGameMode_DuoPlay)
		{
			for (int i = 0; i < DUO_VIEWPORT; i++)
			{
				if (i == enCameraDrawing_Left)
				{
					m_cameraDrawing = enCameraDrawing_Left;
				}
				else if (i == enCameraDrawing_Right)
				{
					m_cameraDrawing = enCameraDrawing_Right;
				}

				rc.SetViewport(m_duoViewPorts[i]);
				//モデル描画
				for (auto& renderObj : m_renderObjects) {
					renderObj->OnForwardRender(rc);
				}
				//敵のHPバーなどの画像と文字を描画
				SpriteViewportRendering(rc, i);
				FontViewportRendering(rc, i);
			}
		}
		//4画面分割に描画
		else if (m_gameMode == enGameMode_TrioPlay || m_gameMode == enGameMode_QuartetPlay)
		{
			for (int i = 0; i < MAX_VIEWPORT; i++)
			{
				if (i == enCameraDrawing_LeftUp)
				{
					m_cameraDrawing = enCameraDrawing_LeftUp;
				}
				else if (i == enCameraDrawing_RightUp)
				{
					m_cameraDrawing = enCameraDrawing_RightUp;
				}
				else if (i == enCameraDrawing_LeftDown)
				{
					m_cameraDrawing = enCameraDrawing_LeftDown;
				}
				else if (i == enCameraDrawing_RightDown)
				{
					m_cameraDrawing = enCameraDrawing_RightDown;
				}

				rc.SetViewport(m_quarteViewPorts[i]);
				//モデル描画
				for (auto& renderObj : m_renderObjects) {
					renderObj->OnForwardRender(rc);
				}
				//敵のHPバーなどの画像と文字を描画
				SpriteViewportRendering(rc, i);
				FontViewportRendering(rc, i);
			}
		}
		//画面分割をしない
		else
		{
			rc.SetViewport(m_soloViewPort);
			m_cameraDrawing = enCameraDrawing_Solo;
			for (auto& renderObj : m_renderObjects) {
				renderObj->OnForwardRender(rc);
			}
		}
	}

	void RenderingEngine::RenderToGBuffer(RenderContext& rc)
	{
		BeginGPUEvent("RenderToGBuffer");
		// レンダリングターゲットをG-Bufferに変更
		RenderTarget* rts[enGBufferNum] = {
			&m_gBuffer[enGBufferAlbedoDepth],         // 0番目のレンダリングターゲット
			&m_gBuffer[enGBufferNormal],              // 1番目のレンダリングターゲット
			&m_gBuffer[enGBufferMetaricShadowSmooth], // 2番目のレンダリングターゲット
		};

		// まず、レンダリングターゲットとして設定できるようになるまで待つ
		rc.WaitUntilToPossibleSetRenderTargets(ARRAYSIZE(rts), rts);

		// レンダリングターゲットを設定
		rc.SetRenderTargets(ARRAYSIZE(rts), rts);

		// レンダリングターゲットをクリア
		rc.ClearRenderTargetViews(ARRAYSIZE(rts), rts);

		DrawModelOnGBuffer(rc);

		// レンダリングターゲットへの書き込み待ち
		rc.WaitUntilFinishDrawingToRenderTargets(ARRAYSIZE(rts), rts);
		EndGPUEvent();
	}

	void RenderingEngine::DrawModelOnGBuffer(RenderContext& rc)
	{
		//シザリング矩形も設定する。
		D3D12_RECT scissorRect;
		scissorRect.bottom = static_cast<LONG>(m_soloViewPort.Height);
		scissorRect.top = 0;
		scissorRect.left = 0;
		scissorRect.right = static_cast<LONG>(m_soloViewPort.Width);
		rc.SetScissorRect(scissorRect);

		//2画面分割に描画
		if (m_gameMode == enGameMode_DuoPlay)
		{
			for (int i = 0; i < DUO_VIEWPORT; i++)
			{
				if (i == enCameraDrawing_Left)
				{
					m_cameraDrawing = enCameraDrawing_Left;
					m_gbufferCB.drawCameraNumber = enCameraDrawing_Left;
				}
				else if (i == enCameraDrawing_Right)
				{
					m_cameraDrawing = enCameraDrawing_Right;
					m_gbufferCB.drawCameraNumber = enCameraDrawing_Right;
				}

				rc.SetViewport(m_duoViewPorts[i]);
				//モデル描画
				for (auto& renderObj : m_renderObjects) {
					renderObj->OnRenderToGBuffer(rc);
				}
			}
		}
		//4画面分割に描画
		else if (m_gameMode == enGameMode_TrioPlay || m_gameMode == enGameMode_QuartetPlay)
		{
			for (int i = 0; i < MAX_VIEWPORT; i++)
			{
				if (i == enCameraDrawing_LeftUp)
				{
					m_cameraDrawing = enCameraDrawing_LeftUp;
					m_gbufferCB.drawCameraNumber = enCameraDrawing_LeftUp;

				}
				else if (i == enCameraDrawing_RightUp)
				{
					m_cameraDrawing = enCameraDrawing_RightUp;
					m_gbufferCB.drawCameraNumber = enCameraDrawing_RightUp;
				}
				else if (i == enCameraDrawing_LeftDown)
				{
					m_cameraDrawing = enCameraDrawing_LeftDown;
					m_gbufferCB.drawCameraNumber = enCameraDrawing_LeftDown;
				}
				else if (i == enCameraDrawing_RightDown)
				{
					m_cameraDrawing = enCameraDrawing_RightDown;
					m_gbufferCB.drawCameraNumber = enCameraDrawing_RightDown;
				}

				rc.SetViewport(m_quarteViewPorts[i]);
				//モデル描画
				for (auto& renderObj : m_renderObjects) {
					renderObj->OnRenderToGBuffer(rc);
				}
			}
		}
		//画面分割をしない
		else
		{
			rc.SetViewport(m_soloViewPort);
			m_cameraDrawing = enCameraDrawing_Solo;
			m_gbufferCB.drawCameraNumber = enCameraDrawing_Solo;
			for (auto& renderObj : m_renderObjects) {
				renderObj->OnRenderToGBuffer(rc);
			}
		}
	}

	void RenderingEngine::DeferredLighting(RenderContext& rc)
	{
		// ディファードライティングに必要なライト情報を更新する
		for (int currentViewport = 0; currentViewport < m_gameMode; currentViewport++)
		{
			m_lightingCB.m_light.eyeInfomation.eyePos[currentViewport] = g_camera3D[currentViewport]->GetPosition();
			m_lightingCB.m_light.eyeInfomation.mViewProjInv[currentViewport].Inverse(g_camera3D[currentViewport]->GetViewProjectionMatrix());
			for (int areaNo = 0; areaNo < NUM_SHADOW_MAP; areaNo++)
			{
				m_lightingCB.mlvp[currentViewport][areaNo] = m_shadowMapRenders[currentViewport].GetLVPMatrix(areaNo);
			}
		}

		BeginGPUEvent("DeferredLighting");

		rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
		rc.SetRenderTargetAndViewport(m_mainRenderTarget);

		// G-Bufferの内容を元にしてディファードライティング
		m_diferredLightingSprite.Draw(rc);

		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);

		EndGPUEvent();
	}

	void RenderingEngine::FowardRendering(RenderContext& rc)
	{
		BeginGPUEvent("FowardRendering");

		rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
		rc.SetRenderTarget(
			m_mainRenderTarget.GetRTVCpuDescriptorHandle(),
			m_gBuffer[enGBufferAlbedoDepth].GetDSVCpuDescriptorHandle()
		);

		DrawModelInViewPorts(rc);

		//レンダリングターゲットへの書き込み終了待ち
		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);

		EndGPUEvent();
	}

	void RenderingEngine::RenderToShadowMap(RenderContext& rc)
	{
		BeginGPUEvent("RenderToShadowMap");
		for (int viewportNumber = 0; viewportNumber < m_gameMode; viewportNumber++)
		{
			if (m_sceneLight.IsCastShadow())
			{
				m_shadowMapRenders[viewportNumber].Render(
					rc,
					m_lightingCB.m_light.directionalLight[0].direction,
					m_renderObjects,
					viewportNumber
				);
			}
		}
		EndGPUEvent();
	}

	void RenderingEngine::SpriteRendering(RenderContext& rc, const bool drawTiming)
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

	void RenderingEngine::SpriteViewportRendering(RenderContext& rc, const int viewportNo)
	{
		for (auto& spriteObj : m_spriteDrawViewportList[viewportNo])
		{
			spriteObj->OnRenderSprite(rc);
		}
	}

	void RenderingEngine::SpriteFrontRendering(RenderContext& rc)
	{
		for (auto& spriteObj : m_spriteFrontDrawList)
		{
			spriteObj->OnRenderSprite(rc);
		}
	}

	void RenderingEngine::FontRendering(RenderContext& rc, const bool drawTiming)
	{
		if (drawTiming)
		{
			for (auto& fontObj : m_laterFontList)
			{
				fontObj->OnRenderFont(rc);
			}
		}
		else
		{
			for (auto& fontObj : m_fontList)
			{
				fontObj->OnRenderFont(rc);
			}
		}
	}

	void RenderingEngine::FontViewportRendering(RenderContext& rc, const int viewportNo)
	{
		for (auto& fontObj : m_fontDrawViewportList[viewportNo])
		{
			fontObj->OnRenderFont(rc);
		}
	}

	void RenderingEngine::ExcuteEffectRender(RenderContext& rc)
	{
		rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
		rc.SetRenderTarget(
			m_mainRenderTarget.GetRTVCpuDescriptorHandle(),
			m_gBuffer[enGBufferAlbedoDepth].GetDSVCpuDescriptorHandle()
		);

		if (m_gameMode == enGameMode_DuoPlay) {
			g_camera2D->SetWidth(FRAME_BUFFER_WIDTH_HALF);
			EffectEngine::GetInstance()->Update(g_gameTime->GetFrameDeltaTime(), enCameraDrawing_Left);
			//左画面
			{
				EffectEngine::GetInstance()->BeginDraw(enCameraDrawing_Left);
				rc.SetViewport(m_duoViewPorts[enCameraDrawing_Left]);
				EffectEngine::GetInstance()->Draw();
				EffectEngine::GetInstance()->EndDraw();
			}
			//右画面
			{
				EffectEngine::GetInstance()->BeginDraw(enCameraDrawing_Right);
				rc.SetViewport(m_duoViewPorts[enCameraDrawing_Right]);
				EffectEngine::GetInstance()->Draw();
				EffectEngine::GetInstance()->EndDraw();
			}
			EffectEngine::GetInstance()->Flush();
			//1画面のスプライトのアスペクト比に合わせる。
			g_camera2D->SetWidth(static_cast<float>(g_graphicsEngine->GetFrameBufferWidth()));
			//ビューポートを画面全体用に切り替える
			rc.SetViewport(m_soloViewPort);
		}

		else if (m_gameMode == enGameMode_TrioPlay || m_gameMode == enGameMode_QuartetPlay)
		{
			g_camera2D->SetWidth(FRAME_BUFFER_WIDTH_HALF);
			g_camera2D->SetHeight(FRAME_BUFFER_HEIGHT_HALF);
			EffectEngine::GetInstance()->Update(g_gameTime->GetFrameDeltaTime(), enCameraDrawing_LeftUp);
			//左上画面
			{
				EffectEngine::GetInstance()->BeginDraw(enCameraDrawing_LeftUp);
				rc.SetViewport(m_quarteViewPorts[enCameraDrawing_LeftUp]);
				EffectEngine::GetInstance()->Draw();
				EffectEngine::GetInstance()->EndDraw();
			}
			//右上画面
			{
				EffectEngine::GetInstance()->BeginDraw(enCameraDrawing_RightUp);
				rc.SetViewport(m_quarteViewPorts[enCameraDrawing_RightUp]);
				EffectEngine::GetInstance()->Draw();
				EffectEngine::GetInstance()->EndDraw();
			}
			//左下画面
			{
				EffectEngine::GetInstance()->BeginDraw(enCameraDrawing_LeftDown);
				rc.SetViewport(m_quarteViewPorts[enCameraDrawing_LeftDown]);
				EffectEngine::GetInstance()->Draw();
				EffectEngine::GetInstance()->EndDraw();
			}
			//右下画面
			{
				EffectEngine::GetInstance()->BeginDraw(enCameraDrawing_RightDown);
				rc.SetViewport(m_quarteViewPorts[enCameraDrawing_RightDown]);
				EffectEngine::GetInstance()->Draw();
				EffectEngine::GetInstance()->EndDraw();
			}
			EffectEngine::GetInstance()->Flush();
			//1画面のスプライトのアスペクト比に合わせる。
			g_camera2D->SetWidth(static_cast<float>(g_graphicsEngine->GetFrameBufferWidth()));
			g_camera2D->SetHeight(static_cast<float>(g_graphicsEngine->GetFrameBufferHeight()));
			//ビューポートを画面全体用に切り替える
			rc.SetViewport(m_soloViewPort);
		}

		//1画面
		else {
			//1画面のスプライトのアスペクト比に合わせる。
			g_camera2D->SetWidth(static_cast<float>(g_graphicsEngine->GetFrameBufferWidth()));

			rc.SetViewport(m_soloViewPort);

			//1画面オンリーのエフェクト更新
			EffectEngine::GetInstance()->Update(g_gameTime->GetFrameDeltaTime(), enCameraDrawing_Solo);
			//1画面オンリーのエフェクト描画
			EffectEngine::GetInstance()->BeginDraw(enCameraDrawing_Solo);
			EffectEngine::GetInstance()->Draw();
			EffectEngine::GetInstance()->EndDraw();
			EffectEngine::GetInstance()->Flush();
		}
		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);

	}

	void RenderingEngine::Render2D(RenderContext& rc)
	{
		BeginGPUEvent("Render2D");

		rc.WaitUntilToPossibleSetRenderTarget(m_2DRenderTarget);
		rc.SetRenderTargetAndViewport(m_2DRenderTarget);
		rc.ClearRenderTargetView(m_2DRenderTarget);

		//メインレンダーターゲットのカラーバッファを描画する
		m_mainSprite.Draw(rc);

		//画像の描画
		SpriteRendering(rc);
		//文字の描画
		FontRendering(rc);
		//文字の上に画像を描画
		SpriteRendering(rc, true);
		SpriteFrontRendering(rc);
		FontRendering(rc, true);

		rc.WaitUntilFinishDrawingToRenderTarget(m_2DRenderTarget);

		//レンダリングターゲットをメインレンダリングターゲットへ変更する
		rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
		rc.SetRenderTargetAndViewport(m_mainRenderTarget);

		//メインレンダーターゲットに2D描画後のスプライトを描画する
		m_2DSprite.Draw(rc);

		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);

		EndGPUEvent();
	}

	void RenderingEngine::ClearRenderList()
	{
		m_renderObjects.clear();
		m_spriteList.clear();
		m_laterSpriteList.clear();
		m_fontList.clear();
		m_laterFontList.clear();
		m_spriteFrontDrawList.clear();

		for (int i = 0; i < MAX_VIEWPORT; i++)
		{
			m_spriteDrawViewportList[i].clear();
			m_fontDrawViewportList[i].clear();
		}
	}

	void RenderingEngine::SetLightingCB()
	{
		m_lightingCB.m_light = m_sceneLight.GetSceneLight();
		m_lightingCB.gameMode = m_gameMode;
	}

	void RenderingEngine::Execute(RenderContext& rc)
	{
		SetLightingCB();

		RenderToShadowMap(rc);
		RenderToGBuffer(rc);
		DeferredLighting(rc);
		FowardRendering(rc);

		//ポストエフェクト
		m_postEffect.Render(rc, m_mainRenderTarget);

		//エフェクトの描画
		ExcuteEffectRender(rc);

		//画像と文字の描画
		Render2D(rc);

		rc.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		);

		//メインレンダーターゲットをコピーする
		m_copyToFrameBufferSprite.Draw(rc);

		ClearRenderList();
	}

	void RenderingEngine::SetDirectionLight(const int lightNo, const Vector3 direction, const Vector3 color)
	{
		m_sceneLight.SetDirectionLight(lightNo, direction, color);
	}

	void RenderingEngine::SetDirectionLightCastShadow(const bool flag)
	{
		m_sceneLight.SetDirectionLightCastShadow(flag);
	}
	void RenderingEngine::SetAmbient(const Vector3 ambient)
	{
		m_sceneLight.SetAmbient(ambient);
	}
}