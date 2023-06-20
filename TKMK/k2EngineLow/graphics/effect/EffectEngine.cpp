#include "k2EngineLowPreCompile.h"

#include "EffectEngine.h"

namespace nsK2EngineLow {
	EffectEngine* EffectEngine::m_instance = nullptr;	//�B��̃C���X�^���X�B

	EffectEngine::EffectEngine()
	{
		K2_ASSERT(
			m_instance == nullptr,
			"EffectEngine�̃C���X�^���X�𕡐���邱�Ƃ͂ł��܂���B"
		);
		//auto format = DXGI_FORMAT_R8G8B8A8_UNORM;
		auto format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		auto d3dDevice = g_graphicsEngine->GetD3DDevice();
		auto commandQueue = g_graphicsEngine->GetCommandQueue();
		for (int i = 0; i < 2; i++) {
			// �����_���[���쐬�B
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
			//�������v�[���̍쐬�B
			m_memoryPool[i] = EffekseerRenderer::CreateSingleFrameMemoryPool(m_renderer[i]->GetGraphicsDevice());
			// �R�}���h���X�g�̍쐬
			m_commandList[i] = EffekseerRenderer::CreateCommandList(m_renderer[i]->GetGraphicsDevice(), m_memoryPool[i]);
			// �G�t�F�N�g�}�l�[�W���[�̍쐬�B
			m_manager[i] = ::Effekseer::Manager::Create(8000);
			m_manager[i]->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());
		}

		
	}
	Effekseer::EffectRef EffectEngine::LoadEffect(const int number)
	{
		Effekseer::EffectRef effect;
		auto it = m_effectMap.find(number);
		if (it != m_effectMap.end()) {
			//���[�h�ς݁B
			effect = it->second;
		}
		else {
			//���[�h�ł��Ȃ��B
			//ResistEffect�ŃG�t�F�N�g�ǂݍ���łˁI
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
		// �R�}���h���X�g���J�n����B
		EffekseerRendererDX12::BeginCommandList(m_commandList[cameraNumber], g_graphicsEngine->GetCommandList());
		m_renderer[cameraNumber]->SetCommandList(m_commandList[cameraNumber]);

		m_manager[cameraNumber]->Update();

		//�����_���[�ɃJ�����s���ݒ�B
		m_renderer[cameraNumber]->SetCameraMatrix(*(const Effekseer::Matrix44*)&g_camera3D[cameraNumber]->GetViewMatrix());
		//�����_���[�Ƀv���W�F�N�V�����s���ݒ�B
		m_renderer[cameraNumber]->SetProjectionMatrix(*(const Effekseer::Matrix44*)&g_camera3D[cameraNumber]->GetProjectionMatrix());

		m_renderer[cameraNumber]->SetTime(deltaTime);
	}

	void EffectEngine::BeginFrame(int cameraNumber)
	{
		//int backBufferNo = g_graphicsEngine->GetBackBufferIndex();
		m_memoryPool[cameraNumber]->NewFrame();
		// �`�惂�W���[���̐ݒ�B
		m_manager[cameraNumber]->SetSpriteRenderer(m_renderer[cameraNumber]->CreateSpriteRenderer());
		m_manager[cameraNumber]->SetRibbonRenderer(m_renderer[cameraNumber]->CreateRibbonRenderer());
		m_manager[cameraNumber]->SetRingRenderer(m_renderer[cameraNumber]->CreateRingRenderer());
		m_manager[cameraNumber]->SetTrackRenderer(m_renderer[cameraNumber]->CreateTrackRenderer());
		m_manager[cameraNumber]->SetModelRenderer(m_renderer[cameraNumber]->CreateModelRenderer());

		// ���[�_�[�̐ݒ�B
		m_manager[cameraNumber]->SetTextureLoader(m_renderer[cameraNumber]->CreateTextureLoader());
		m_manager[cameraNumber]->SetModelLoader(m_renderer[cameraNumber]->CreateModelLoader());
		m_manager[cameraNumber]->SetMaterialLoader(m_renderer[cameraNumber]->CreateMaterialLoader());
	}
	void EffectEngine::Draw(int cameraNumber)
	{
		//int backBufferNo = g_graphicsEngine->GetBackBufferIndex();
		// Begin to rendering effects
		// �G�t�F�N�g�̕`��J�n�������s���B
		m_renderer[cameraNumber]->BeginRendering();

		// Render effects
		// �G�t�F�N�g�̕`����s���B
		m_manager[cameraNumber]->Draw();

		// Finish to rendering effects
		// �G�t�F�N�g�̕`��I���������s���B
		m_renderer[cameraNumber]->EndRendering();

		// Finish a command list
		// �R�}���h���X�g���I������B
		m_renderer[cameraNumber]->SetCommandList(nullptr);
		EffekseerRendererDX12::EndCommandList(m_commandList[cameraNumber]);
	}

	void EffectEngine::ResistEffect(const int number, const char16_t* filePath)
	{
		Effekseer::EffectRef effect;
		auto it = m_effectMap.find(number);
		if (it == m_effectMap.end()) {
			//�V�K�B
			effect = Effekseer::Effect::Create(m_manager[0], filePath);
			Effekseer::Effect::Create(m_manager[1], filePath);
			m_effectMap.insert({ number, effect });
		}
	}
}