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
	m_modelRender.InitBackGround("Assets/modelData/background/stadium04.tkm");
	
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetRotation(m_rotation);
	m_scale.y = 1.3f;
	m_modelRender.SetScale(m_scale);

	m_modelRender.Update();
	m_stagecollision.Init("Assets/modelData/background/backgroundcoll.tkm");
	m_stagecollision.SetPosition(m_position);
	m_stagecollision.SetRotation(m_rotation);
	m_scale.y = 1.3f;
	m_stagecollision.SetScale(m_scale);
	m_stagecollision.Update();
	m_physicsStaticObject.CreateFromModel(m_stagecollision.GetModel(), m_stagecollision.GetModel().GetWorldMatrix());
	m_physicsStaticObject.GetbtCollisionObject()->setUserIndex(enCollisionAttr_Wall);

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
