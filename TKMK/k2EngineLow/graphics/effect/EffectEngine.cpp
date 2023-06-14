#include "k2EngineLowPreCompile.h"

#include "EffectEngine.h"

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
		for (int i = 0; i < 2; i++) {
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
			//メモリプールの作成。
			m_memoryPool[i] = EffekseerRenderer::CreateSingleFrameMemoryPool(m_renderer[i]->GetGraphicsDevice());
			// コマンドリストの作成
			m_commandList[i] = EffekseerRenderer::CreateCommandList(m_renderer[i]->GetGraphicsDevice(), m_memoryPool[i]);
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
		//int backBufferNo = g_graphicsEngine->GetBackBufferIndex();
		// Begin a command list
		// コマンドリストを開始する。
		EffekseerRendererDX12::BeginCommandList(m_commandList[cameraNumber], g_graphicsEngine->GetCommandList());
		m_renderer[cameraNumber]->SetCommandList(m_commandList[cameraNumber]);

		m_manager[cameraNumber]->Update();

		//レンダラーにカメラ行列を設定。
		m_renderer[cameraNumber]->SetCameraMatrix(*(const Effekseer::Matrix44*)&g_camera3D[cameraNumber]->GetViewMatrix());
		//レンダラーにプロジェクション行列を設定。
		m_renderer[cameraNumber]->SetProjectionMatrix(*(const Effekseer::Matrix44*)&g_camera3D[cameraNumber]->GetProjectionMatrix());

		m_renderer[cameraNumber]->SetTime(deltaTime);
	}

	void EffectEngine::BeginFrame(int cameraNumber)
	{
		//int backBufferNo = g_graphicsEngine->GetBackBufferIndex();
		m_memoryPool[cameraNumber]->NewFrame();
		// 描画モジュールの設定。
		m_manager[cameraNumber]->SetSpriteRenderer(m_renderer[cameraNumber]->CreateSpriteRenderer());
		m_manager[cameraNumber]->SetRibbonRenderer(m_renderer[cameraNumber]->CreateRibbonRenderer());
		m_manager[cameraNumber]->SetRingRenderer(m_renderer[cameraNumber]->CreateRingRenderer());
		m_manager[cameraNumber]->SetTrackRenderer(m_renderer[cameraNumber]->CreateTrackRenderer());
		m_manager[cameraNumber]->SetModelRenderer(m_renderer[cameraNumber]->CreateModelRenderer());

		// ローダーの設定。
		m_manager[cameraNumber]->SetTextureLoader(m_renderer[cameraNumber]->CreateTextureLoader());
		m_manager[cameraNumber]->SetModelLoader(m_renderer[cameraNumber]->CreateModelLoader());
		m_manager[cameraNumber]->SetMaterialLoader(m_renderer[cameraNumber]->CreateMaterialLoader());
	}
	void EffectEngine::Draw(int cameraNumber)
	{
		//int backBufferNo = g_graphicsEngine->GetBackBufferIndex();
		// Begin to rendering effects
		// エフェクトの描画開始処理を行う。
		m_renderer[cameraNumber]->BeginRendering();

		// Render effects
		// エフェクトの描画を行う。
		m_manager[cameraNumber]->Draw();

		// Finish to rendering effects
		// エフェクトの描画終了処理を行う。
		m_renderer[cameraNumber]->EndRendering();

		// Finish a command list
		// コマンドリストを終了する。
		m_renderer[cameraNumber]->SetCommandList(nullptr);
		EffekseerRendererDX12::EndCommandList(m_commandList[cameraNumber]);
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