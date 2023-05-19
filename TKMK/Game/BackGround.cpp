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

	//地面
	m_modelRenderGround.InitBackGround("Assets/modelData/background/stadium_ground.tkm");

	m_modelRenderGround.SetPosition(m_position);
	m_modelRenderGround.SetRotation(m_rotation);
	m_scale.y = 1.1f;
	m_modelRenderGround.SetScale(m_scale);
	m_modelRenderGround.Update();
	m_physicsStaticObjectGround.CreateFromModel(m_modelRenderGround.GetModel(), m_modelRenderGround.GetModel().GetWorldMatrix());
	m_physicsStaticObjectGround.GetbtCollisionObject()->setUserIndex(enCollisionAttr_Ground);

	//塔と中心の壁
	m_modelRenderObject.Init("Assets/modelData/background/stadium_object.tkm");

	m_modelRenderObject.SetPosition(m_position);
	m_modelRenderObject.SetRotation(m_rotation);
	m_scale.y = 1.1f;
	m_modelRenderObject.SetScale(m_scale);
	m_modelRenderObject.Update();
	m_physicsStaticObjectTower.CreateFromModel(m_modelRenderObject.GetModel(), m_modelRenderObject.GetModel().GetWorldMatrix());
	m_physicsStaticObjectTower.GetbtCollisionObject()->setUserIndex(enCollisionAttr_SlipThroughWall);

	//外壁
	m_modelRenderWall.Init("Assets/modelData/background/stadium05_Wall.tkm");
	m_modelRenderWall.SetPosition(m_position);
	m_modelRenderWall.SetRotation(m_rotation);
	m_scale.y = 0.7f;
	m_modelRenderWall.SetScale(m_scale);
	m_modelRenderWall.Update();
	m_physicsStaticObjectWall.CreateFromModel(m_modelRenderWall.GetModel(), m_modelRenderWall.GetModel().GetWorldMatrix());
	m_physicsStaticObjectWall.GetbtCollisionObject()->setUserIndex(enCollisionAttr_Wall);

	return true;
}

void BackGround::Update()
{
	m_modelRenderGround.Update();
	m_modelRenderObject.Update();
	m_modelRenderWall.Update();
}

void BackGround::Render(RenderContext& rc)
{
	m_modelRenderGround.Draw(rc);
	m_modelRenderObject.Draw(rc);
	m_modelRenderWall.Draw(rc);
}
