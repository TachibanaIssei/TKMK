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
	//tittle‚Ì‰Šú‰»
	m_spriteRender.Init("Assets/sprite/tittle.DDS",1920.0f,1080.0f);
	m_spriteRender.SetPosition(0.0f, 0.0f, 0.0f);
	m_spriteRender.SetScale(1.0f, 1.0f, 1.0f);
	m_sRotation.SetRotationZ(0.0f);
	m_spriteRender.SetRotation(m_sRotation);
	m_spriteRender.Update();

	return true;
}

void Tittle::Update()
{
	//ƒQ[ƒ€‰æ–Ê‚Ö‚Ì‘JˆÚ
	if (g_pad[0]->IsTrigger(enButtonA)) {
		Game* game = NewGO<Game>(0, "game");
		DeleteGO(this);
	}

	m_spriteRender.Update();
}

void Tittle::Render(RenderContext& rc)
{
	m_spriteRender.Draw(rc);
}
