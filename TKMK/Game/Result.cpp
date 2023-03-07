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

	return true;
}

void Result::Update()
{
	//リザルト画面からタイトル画面への遷移
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		Tittle* tittle = NewGO<Tittle>(0, "tittle");
		DeleteGO(this);
	}
}

void Result::Render(RenderContext& rc)
{
	m_spriteRender.Draw(rc);
}