#include "stdafx.h"
#include "Lamp.h"

Lamp::Lamp()
{

}

Lamp::~Lamp()
{

}

bool Lamp::Start()
{
	m_modelRender.InitBackGround("Assets/modelData/background/stadium05_lamp.tkm");
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetRotation(m_rotation);
	m_modelRender.SetScale(m_scale);
	m_modelRender.Update();
	
	return true;
}

void Lamp::Update()
{

}

void Lamp::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
