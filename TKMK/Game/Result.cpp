#include "stdafx.h"
#include "Result.h"

#include "Tittle.h"

Result::Result()
{

}

Result::~Result()
{

}

bool Result::Start()
{
	//Resultの初期化
	m_spriteRender.Init("Assets/sprite/result.DDS", 1920.0f, 1080.0f);
	m_spriteRender.SetPosition(0.0f, 0.0f, 0.0f);
	m_spriteRender.SetScale(1.0f, 1.0f, 1.0f);
	m_sRotation.SetRotationZ(0.0f);
	m_spriteRender.SetRotation(m_sRotation);
	m_spriteRender.Update();
	
	//順位によってBGMが変化
	
	switch (Rank)
	{
	case 1:
		m_bgm = NewGO<SoundSource>(0);
		m_bgm->Init(3);
		m_bgm->Play(true);
		m_bgm->SetVolume(0.3f);
		break;
	case 2:
		m_bgm = NewGO<SoundSource>(0);
		m_bgm->Init(4);
		m_bgm->Play(true);
		m_bgm->SetVolume(0.3f);
		break;
	case 3:
	case 4:
		m_bgm = NewGO<SoundSource>(0);
		m_bgm->Init(5);
		m_bgm->Play(true);
		m_bgm->SetVolume(0.3f);
		break;
	}


	return true;
}

void Result::Update()
{
	//リザルト画面からタイトル画面への遷移
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		Tittle* tittle = NewGO<Tittle>(0, "tittle");
		DeleteGO(this);
		DeleteGO(m_bgm);
	}
}

void Result::Render(RenderContext& rc)
{
	m_spriteRender.Draw(rc);
}