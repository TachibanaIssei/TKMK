#include "stdafx.h"
#include "GameCamera.h"

#include "Game.h"
#include "KnightBase.h"
#include "KnightPlayer.h"

GameCamera::GameCamera()
{

}

GameCamera::~GameCamera()
{

}

bool GameCamera::Start()
{
	m_knightplayer = FindGO<KnightPlayer>("m_knightplayer");

	//�����_���王�_�܂ł̃x�N�g����ݒ�B80-160
	m_toCameraPos.Set(0.0f, 80.0f, -160.0f);
	g_camera3D->SetNear(1.0f);
	g_camera3D->SetFar(10000.0f);

	return true;
}

void GameCamera::Update()
{
	Vector3 TargetPos;
	/// <summary>
	/// ////////////////////
	/// </summary>
	TargetPos = m_knightplayer->GetPosition();
	TargetPos.y += 80.0f;



	Vector3 toCameraPosOld = m_toCameraPos;
	//�p�b�h�̓��͂�g���ăJ������񂷁B
	float x = g_pad[0]->GetRStickXF();
	float y = g_pad[0]->GetRStickYF();
	//Y������̉�]
	Quaternion qRot;
	qRot.SetRotationDeg(Vector3::AxisY, 1.3f * x);
	qRot.Apply(m_toCameraPos);
	//X������̉�]�B
	Vector3 axisX;
	axisX.Cross(Vector3::AxisY, m_toCameraPos);
	axisX.Normalize();
	qRot.SetRotationDeg(axisX, 1.3f * y);
	qRot.Apply(m_toCameraPos);
	//�J�����̉�]�̏����`�F�b�N����B
	//�����_���王�_�܂ł̃x�N�g���𐳋K������B
	//���K������ƁA�x�N�g���̑傫�����P�ɂȂ�B
	//�傫�����P�ɂȂ�Ƃ������Ƃ́A�x�N�g�����狭�����Ȃ��Ȃ�A�����݂̂̏��ƂȂ�Ƃ������ƁB
	Vector3 toPosDir = m_toCameraPos;
	toPosDir.Normalize();
	if (toPosDir.y < -0.3f) {
		//�J����������������B
		m_toCameraPos = toCameraPosOld;
	}
	else if (toPosDir.y > 0.9f) {
		//�J�����������������B
		m_toCameraPos = toCameraPosOld;
	}

	Vector3 pos;
	//���_�ƒ����_�𑫂�
	pos = TargetPos + m_toCameraPos;
	g_camera3D->SetTarget(TargetPos);
	g_camera3D->SetPosition(pos);
	//g_camera3D->SetPosition(m_toCameraPos);

	//�J�����̍X�V�B
	g_camera3D->Update();
}



