#include "stdafx.h"
#include "GameCamera.h"

#include "Game.h"
#include "KnightBase.h"
#include "KnightPlayer.h"
#include "WizardPlayer.h"
#include "Player.h"
#include "Game.h"

namespace
{
	const float MAX_CAMERA_TOP = -0.1f;		//�J�����̏�����̍ő�l
	const float MAX_CAMERA_UNDER = 0.6f;	//�J�����̉������̍ő�l
}

GameCamera::GameCamera()
{

}

GameCamera::~GameCamera()
{

}

bool GameCamera::Start()
{
	//�Q�[���̃C���X�^���X��T��
	game = FindGO<Game>("game");
	//�v���C���[�̃C���X�^���X��T��
	player = FindGO<Player>("player");

	//
	/*wizardPlayer = FindGO<WizardPlayer>("wizardPlayer");
	m_knightplayer= FindGO<KnightPlayer>("m_knightplayer");*/

	//�����_���王�_�܂ł̃x�N�g����ݒ�B80-160
	m_toCameraPos.Set(0.0f, 80.0f, -160.0f);
	g_camera3D->SetNear(1.0f);
	g_camera3D->SetFar(10000.0f);

	m_cameraCollisionSolver.Init(1.0f);

	m_cameraState = enGameState;

	return true;
}

void GameCamera::Update()
{
	//�|�[�Y�X�e�[�g�̂Ƃ��͏��������Ȃ�
	if (m_cameraState == enPauseState) {
		return;
	}

	//�����_�̌v�Z
	Vector3 TargetPos;
	//TargetPos = m_knightplayer->GetPosition();
	//TargetPos = wizardPlayer->GetPosition();
	TargetPos = player->GetCharPosition();

	TargetPos.y += 40.0f;

	Vector3 toCameraPosOld = m_toCameraPos;

	//�p�b�h�̓��͂��g���ăJ�������񂷁B
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

	//�J�����̉�]�̏�����`�F�b�N����B
	Vector3 toPosDir = m_toCameraPos;
	toPosDir.Normalize();
	if (toPosDir.y < MAX_CAMERA_TOP) {
		//�J����������������B
		m_toCameraPos = toCameraPosOld;
	}
	else if (toPosDir.y > MAX_CAMERA_UNDER) {
		//�J�����������������B
		m_toCameraPos = toCameraPosOld;
	}

	//�J�����̈ʒu�̏Փˉ�������
	Vector3 newCamPos;
	m_cameraCollisionSolver.Execute(
		newCamPos,
		TargetPos + m_toCameraPos,
		TargetPos
	);

	//���_�ƒ����_��ݒ�
	g_camera3D->SetTarget(TargetPos);
	g_camera3D->SetPosition(newCamPos);

	//�J�����̍X�V�B
	g_camera3D->Update();
}



