#include "stdafx.h"
#include "KnightUlt.h"

namespace {
	const Vector3 CollsionSize = Vector3(300.0f, 120.0f, 15.0f);
	const float SphereSize = 20.0f;
}

KnightUlt::KnightUlt()
{

}

KnightUlt::~KnightUlt()
{
	DeleteGO(UltCollision);
	DeleteGO(UltDeleteJudgeCollision);
}

bool KnightUlt::Start()
{
	//移動速度を計算。
	m_moveSpeed = Vector3::AxisZ;
	m_rotation.Apply(m_moveSpeed);
	m_position += m_moveSpeed * 20.0f;
	m_moveSpeed *= 180.0f;
	m_rotation.AddRotationDegY(360.0f);

	SetCollision(CollsionSize);

	model.Init("Assets/modelData/character/Knight/Knight_Ult.tkm");
	model.SetPosition(m_position);
	model.SetRotation(m_rotation);
	model.Update();


	//m_position_judge = m_moveSpeed * 20.0f;

	//コリジョンオブジェクトを作成する。
	UltDeleteJudgeCollision = NewGO<CollisionObject>(0);
	UltDeleteJudgeCollision->CreateSphere(m_position, m_rotation, SphereSize);
	//コリジョンの名前
	UltDeleteJudgeCollision->SetName("judgeCollision");
	//懲り所オブジェクトが自動で削除されないようにする。
	UltDeleteJudgeCollision->SetIsEnableAutoDelete(false);

	return true;
}

void KnightUlt::Update()
{
	//座標を移動させる。
	m_position += m_moveSpeed * g_gameTime->GetFrameDeltaTime();

	m_position_judge = m_position;
	//前方向
	m_forward = m_moveSpeed;

	//コリジョンオブジェクトに座標を設定する
	UltCollision->SetPosition(m_position);
	//壁判定用の当たり判定の座標を設定する
	UltDeleteJudgeCollision->SetPosition(m_position);
	//タイマーを加算する。
	m_timer += g_gameTime->GetFrameDeltaTime();

	if (m_timer > 4.0f) {
		//自身を削除する。
		DeleteGO(this);
	}


	model.SetPosition(m_position);
	model.Update();


	UltCollision->Update();
	UltDeleteJudgeCollision->Update();
}

void KnightUlt::Render(RenderContext& rc)
{
	model.Draw(rc);
}