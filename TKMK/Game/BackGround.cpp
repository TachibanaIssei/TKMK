#include "stdafx.h"
#include "BackGround.h"

BackGround::BackGround()
{
}

BackGround::~BackGround()
{
}

bool BackGround::Start()
{
	m_modelRender.Init("Assets/modelData/background/testBackGround.tkm");
	
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetRotation(m_rotation);
	m_modelRender.SetScale(m_scale);

	m_modelRender.Update();

	return true;
}

void BackGround::Update()
{
	m_modelRender.Update();
}

void BackGround::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
