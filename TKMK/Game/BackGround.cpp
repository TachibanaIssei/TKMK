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

	//�X�^�W�A���̏��̐���
	m_modelRender.InitBackGround("Assets/modelData/background/stadium05_ground.tkm");

	m_modelRender.SetPosition(m_position);
	m_modelRender.SetRotation(m_rotation);
	m_scale.y = 1.1f;
	m_modelRender.SetScale(m_scale);
	m_modelRender.Update();
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());
	m_physicsStaticObject.GetbtCollisionObject()->setUserIndex(enCollisionAttr_SlipThroughWall);

	//�X�^�W�A���̕ǂ̐���
	m_modelRender_Wall.Init("Assets/modelData/background/stadium05_Wall.tkm");
	m_modelRender_Wall.SetPosition(m_position);
	m_modelRender_Wall.SetRotation(m_rotation);
	m_scale.y = 0.7f;
	m_modelRender_Wall.SetScale(m_scale);
	m_modelRender_Wall.Update();
	m_physicsStaticObject_Wall.CreateFromModel(m_modelRender_Wall.GetModel(), m_modelRender_Wall.GetModel().GetWorldMatrix());
	m_physicsStaticObject_Wall.GetbtCollisionObject()->setUserIndex(enCollisionAttr_Wall);


	/*m_stagecollision.Init("Assets/modelData/background/backgroundcoll.tkm");
	m_stagecollision.SetPosition(m_position);
	m_stagecollision.SetRotation(m_rotation);
	m_scale.y = 1.3f;
	m_stagecollision.SetScale(m_scale);
	m_stagecollision.Update();
	m_physicsStaticObject.CreateFromModel(m_stagecollision.GetModel(), m_stagecollision.GetModel().GetWorldMatrix());
	m_physicsStaticObject.GetbtCollisionObject()->setUserIndex(enCollisionAttr_Wall);*/

	return true;
}

void BackGround::Update()
{
	m_modelRender.Update();
	m_modelRender_Wall.Update();
}

void BackGround::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
	m_modelRender_Wall.Draw(rc);
}
