#include "stdafx.h"
#include "Tittle.h"

#include "Game.h"

Tittle::Tittle()
{

}

Tittle::~Tittle()
{

}

bool Tittle::Start()
{
	//tittle�̏�����
	m_spriteRender.Init("Assets/sprite/tittle.DDS",1920.0f,1080.0f);
	m_spriteRender.SetPosition(0.0f, 0.0f, 0.0f);
	m_spriteRender.SetScale(1.0f, 1.0f, 1.0f);
	m_sRotation.SetRotationZ(0.0f);
	m_spriteRender.SetRotation(m_sRotation);
	m_spriteRender.Update();

	//BGM�̐ݒ�
	g_soundEngine->ResistWaveFileBank(1, "Assets/sound/gameBGM/TitleBGM1.wav");
	//�I����
	g_soundEngine->ResistWaveFileBank(5, "Assets/sound/sentaku/sentaku4.wav");

	m_bgm = NewGO<SoundSource>(0);
	m_bgm->Init(1);
	m_bgm->Play(true);
	m_bgm->SetVolume(0.5f);

	return true;
}

void Tittle::Update()
{
	//�Q�[����ʂւ̑J��
	if (g_pad[0]->IsTrigger(enButtonA)) {
		//�I����
		m_se = NewGO<SoundSource>(0);
		m_se->Init(5);
		m_se->Play(false);
		m_se->SetVolume(1.0f);
		//game��ʂ֑J��
		Game* game = NewGO<Game>(0, "game");
		DeleteGO(this);
		DeleteGO(m_bgm);
	}

	m_spriteRender.Update();
}

void Tittle::Render(RenderContext& rc)
{
	m_spriteRender.Draw(rc);
}
