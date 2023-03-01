#include "stdafx.h"
#include "Rezult.h"

#include "Tittle.h"

Rezult::Rezult()
{

}

Rezult::~Rezult()
{

}

bool Rezult::Start()
{
	//rezultの初期化
	m_spriteRender.Init("Assets/sprite/result.DDS", 1920.0f, 1080.0f);
	m_spriteRender.SetPosition(0.0f, 0.0f, 0.0f);
	m_spriteRender.SetScale(1.0f, 1.0f, 1.0f);
	m_sRotation.SetRotationZ(0.0f);
	m_spriteRender.SetRotation(m_sRotation);
	m_spriteRender.Update();

	return true;
}

void Rezult::Update()
{
	//リザルト画面からタイトル画面への遷移
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		Tittle* tittle = NewGO<Tittle>(0, "tittle");
		DeleteGO(this);
	}
}

void Rezult::Render(RenderContext& rc)
{
	m_spriteRender.Draw(rc);
}