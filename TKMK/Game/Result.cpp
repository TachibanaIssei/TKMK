#include "stdafx.h"
#include "Result.h"

#include "Tittle.h"
#include "Game.h"

Result::Result()
{

}

Result::~Result()
{

}

bool Result::Start()
{
	Game* game = FindGO<Game>("game");

	game->GetActorPoints(charPoints);

	wchar_t moji2[256];
	swprintf(moji2, L"%d,%d,%d,%d", charPoints[0], charPoints[1], charPoints[2], charPoints[3]);
	moji.SetText(moji2);
	moji.SetPosition(Vector3::Zero);
	moji.SetColor(g_vec4Black);

	DeleteGO(game);

	//Resultの初期化
	m_spriteRender.Init("Assets/sprite/result.DDS", 1920.0f, 1080.0f);
	m_spriteRender.SetPosition(0.0f, 0.0f, 0.0f);
	m_spriteRender.SetScale(1.0f, 1.0f, 1.0f);
	m_sRotation.SetRotationZ(0.0f);
	m_spriteRender.SetRotation(m_sRotation);
	m_spriteRender.Update();

	return true;
}

void Result::Update()
{
	//リザルト画面からタイトル画面への遷移
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		Tittle* tittle = NewGO<Tittle>(0, "tittle");
		tittle->SetTitleScene(titleScene);
		DeleteGO(this);
	}
}

void Result::Rank()
{
	
	/*wchar_t moji3[256];
	if (charPoints[0] > charPoints[1] && charPoints[0] > charPoints[2] && charPoints[0] > charPoints[3])
	{
		swprintf(moji3, L"Win");
	}
	else
	{
		swprintf(moji3, L"Lose");
	}
	jiji.SetText(moji3);
	jiji.SetPosition(90.0f, 90.0f, 90.0f);
	jiji.SetColor(g_vec4Black);*/
}

void Result::Render(RenderContext& rc)
{
	m_spriteRender.Draw(rc);
	moji.Draw(rc);
	jiji.Draw(rc);
}