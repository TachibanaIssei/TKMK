#include "k2EngineLowPreCompile.h"

#include "EffectEngine.h"

namespace {
	const int BACK_BUFFER_COUNT = 2;	//バックバッファの数
}

namespace nsK2EngineLow {
	EffectEngine* EffectEngine::m_instance = nullptr;	//唯一のインスタンス。

	EffectEngine::EffectEngine()
	{
		K2_ASSERT(
			m_instance == nullptr,
			"EffectEngineのインスタンスを複数作ることはできません。"
		);
		//auto format = DXGI_FORMAT_R8G8B8A8_UNORM;
		auto format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		auto d3dDevice = g_graphicsEngine->GetD3DDevice();
		auto commandQueue = g_graphicsEngine->GetCommandQueue();

		for (int i = 0; i < MAX_VIEWPORT; i++) {
			// レンダラーを作成。
			m_renderer[i] = ::EffekseerRendererDX12::Create(
				d3dDevice,
				commandQueue,
				2,
				&format,
				1,
				DXGI_FORMAT_D32_FLOAT,
				false,
				8000
			);
			for (int j = 0;j < BACK_BUFFER_COUNT; j++) {
				//メモリプールの作成。
				m_memoryPool[i][j] = EffekseerRenderer::CreateSingleFrameMemoryPool(m_renderer[i]->GetGraphicsDevice());
				// コマンドリストの作成
				m_commandList[i][j] = EffekseerRenderer::CreateCommandList(m_renderer[i]->GetGraphicsDevice(), m_memoryPool[i][j]);
				
			}
			// エフェクトマネージャーの作成。
			m_manager[i] = ::Effekseer::Manager::Create(8000);
			m_manager[i]->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());
		}
	}
	Effekseer::EffectRef EffectEngine::LoadEffect(const int number)
	{
		Effekseer::EffectRef effect;
		auto it = m_effectMap.find(number);
		if (it != m_effectMap.end()) {
			//ロード済み。
			effect = it->second;
		}
		else {
			//ロードできない。
			//ResistEffectでエフェクト読み込んでね！
			std::abort();
		}
		return effect;
	}
	EffectEngine::~EffectEngine()
	{

	}
	int EffectEngine::Play(Effekseer::EffectRef effect)
	{
		auto handle = m_manager[0]->Play(effect, 0, 0, 0);
		m_manager[1]->Play(effect, 0, 0, 0);
		return handle;
	}
	void EffectEngine::Stop(int effectHandle)
	{
		m_manager[0]->StopEffect(effectHandle);
		m_manager[1]->StopEffect(effectHandle);
	}


	void EffectEngine::Update(float deltaTime, int cameraNumber)
	{
		int backBufferNo = g_graphicsEngine->GetBackBufferIndex();
		// Begin a command list
		// コマンドリストを開始する。
		EffekseerRendererDX12::BeginCommandList(m_commandList[cameraNumber][backBufferNo], g_graphicsEngine->GetCommandList());
		m_renderer[backBufferNo]->SetCommandList(m_commandList[cameraNumber][backBufferNo]);

		m_manager[cameraNumber]->Update();

		//レンダラーにカメラ行列を設定。
		m_renderer[backBufferNo]->SetCameraMatrix(*(const Effekseer::Matrix44*)&g_camera3D[cameraNumber]->GetViewMatrix());
		//レンダラーにプロジェクション行列を設定。
		m_renderer[backBufferNo]->SetProjectionMatrix(*(const Effekseer::Matrix44*)&g_camera3D[cameraNumber]->GetProjectionMatrix());

		m_renderer[backBufferNo]->SetTime(deltaTime);
	}

	void EffectEngine::BeginFrame(int cameraNumber)
	{
		int backBufferNo = g_graphicsEngine->GetBackBufferIndex();
		m_memoryPool[cameraNumber][backBufferNo]->NewFrame();
		// 描画モジュールの設定。
		m_manager[cameraNumber]->SetSpriteRenderer(m_renderer[backBufferNo]->CreateSpriteRenderer());
		m_manager[cameraNumber]->SetRibbonRenderer(m_renderer[backBufferNo]->CreateRibbonRenderer());
		m_manager[cameraNumber]->SetRingRenderer(m_renderer[backBufferNo]->CreateRingRenderer());
		m_manager[cameraNumber]->SetTrackRenderer(m_renderer[backBufferNo]->CreateTrackRenderer());
		m_manager[cameraNumber]->SetModelRenderer(m_renderer[backBufferNo]->CreateModelRenderer());

		// ローダーの設定。
		m_manager[cameraNumber]->SetTextureLoader(m_renderer[backBufferNo]->CreateTextureLoader());
		m_manager[cameraNumber]->SetModelLoader(m_renderer[backBufferNo]->CreateModelLoader());
		m_manager[cameraNumber]->SetMaterialLoader(m_renderer[backBufferNo]->CreateMaterialLoader());
	}
	void EffectEngine::BeginDraw()
	{
		int backBufferNo = g_graphicsEngine->GetBackBufferIndex();
		// Begin to rendering effects
		// エフェクトの描画開始処理を行う。
		m_renderer[backBufferNo]->BeginRendering();

	}
	void EffectEngine::EndDraw()
	{
		int backBufferNo = g_graphicsEngine->GetBackBufferIndex();
		// Finish to rendering effects
		// エフェクトの描画終了処理を行う。
		m_renderer[backBufferNo]->EndRendering();

		// Finish a command list
		// コマンドリストを終了する。
		m_renderer[backBufferNo]->SetCommandList(nullptr);

		// Begin to rendering effects
		EffekseerRendererDX12::EndCommandList(m_commandList[0][backBufferNo]);
	}
	void EffectEngine::Draw(int cameraNumber)
	{
		int backBufferNo = g_graphicsEngine->GetBackBufferIndex();
		// Begin to rendering effects
		// エフェクトの描画開始処理を行う。
		// m_renderer[cameraNumber][backBufferNo]->BeginRendering();
		m_renderer[backBufferNo]->SetCameraMatrix(*(const Effekseer::Matrix44*)&g_camera3D[cameraNumber]->GetViewMatrix());
		//レンダラーにプロジェクション行列を設定。
		m_renderer[backBufferNo]->SetProjectionMatrix(*(const Effekseer::Matrix44*)&g_camera3D[cameraNumber]->GetProjectionMatrix());

		// Render effects
		// エフェクトの描画を行う。
		m_manager[0]->Draw();

		
		// EffekseerRendererDX12::EndCommandList(m_commandList[cameraNumber][backBufferNo]);
	}

	void EffectEngine::ResistEffect(const int number, const char16_t* filePath)
	{
		Effekseer::EffectRef effect;
		auto it = m_effectMap.find(number);
		if (it == m_effectMap.end()) {
			//新規。
			effect = Effekseer::Effect::Create(m_manager[0], filePath);
			Effekseer::Effect::Create(m_manager[1], filePath);
			m_effectMap.insert({ number, effect });
		}
	}
}