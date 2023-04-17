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

	//�����_���王�_�܂ł̃x�N�g����ݒ�B80-160
	m_toCameraPos.Set(0.0f, 50.0f, -160.0f);
	//�J�������v���C���[�̌��ɂ���Ƃ��Ɏg��
	m_position = m_toCameraPos;

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
	//�X�^�[�g�O�̃J�������[�N
	/*if (ok == false)
	{
		StartCameraSet();

		if (rotamount >= 180.0f)
		{
			ok = true;
		}
		else
		return;
	}*/


	if (g_pad[0]->IsTrigger(enButtonY))
	{
		CameraReset();
	}
	else
	{
		//�J�����̎��_��ݒ�
		Target();
	}

}

void GameCamera::Target()
{
	////�����_�̌v�Z
	//Vector3 TargetPos;
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

/// <summary>
/// �J�����̎��_���v���C���[�̔w���𑨂�����̂ɕύX����
/// </summary>
void GameCamera::CameraReset()
{
	//�v���C���[�̑O�������擾
	Vector3 toCameraPosXZ = player->CharSetForward();
	//�ړ����Ă��Ȃ��Ȃ甲���o��
	if (toCameraPosXZ.x == 0.0f && toCameraPosXZ.y == 0.0f)
	{
		return;
	}
	//���K��
	toCameraPosXZ.Normalize();
	//�����_���王�_�Ɍ������x�N�g����XZ������������
	toCameraPosXZ *= (-160);
	//�����_���王�_�Ɍ������x�N�g����Y�����̐ݒ�
	Vector3 toCameraPosY=Vector3::AxisY;
	//�����_���王�_�Ɍ������x�N�g����Y������������
	toCameraPosY.y *= 80;

	//���_���v�Z����
	Vector3 newCameraPos = toCameraPosXZ + toCameraPosY;

	//���_(m_toCameraPos)������������
	m_toCameraPos.Set(newCameraPos);

	//�����_�̌v�Z
	Vector3 TargetPos;
	TargetPos = player->GetCharPosition();
	TargetPos.y += 40.0f;
	//���_���璍���_�ւ̃x�N�g�������߂�
	newCameraPos += TargetPos;

	//�J�����̈ʒu�̏Փˉ�������
	Vector3 newCamPos;
	m_cameraCollisionSolver.Execute(
		newCamPos,
		newCameraPos,
		TargetPos
	);

	//���_�ƒ����_��ݒ�
	g_camera3D->SetTarget(TargetPos);
	g_camera3D->SetPosition(newCamPos);

	//�J�����̍X�V�B
	g_camera3D->Update();
}

/// <summary>
/// �X�^�[�g���̃J�������[�N
/// </summary>
void GameCamera::StartCameraSet()
{
	////�����_�̌v�Z
	//Vector3 TargetPos;
	TargetPos = player->GetCharPosition();

	TargetPos.y += 40.0f;

	Vector3 toCameraPosOld = m_toCameraPos;

	float x = 1.0f;
	float y = 0.0f;

	rotamount += 1.0f;

	//Y������̉�]
	Quaternion qRot;
	qRot.SetRotationDeg(Vector3::AxisY, 1.0f * x);
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

