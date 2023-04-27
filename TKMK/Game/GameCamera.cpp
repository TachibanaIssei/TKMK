#include "stdafx.h"
#include "GameCamera.h"

#include "Game.h"
#include "KnightBase.h"
#include "KnightPlayer.h"
#include "WizardPlayer.h"
#include "Player.h"
#include "Game.h"
#include "KnightUlt.h"

namespace
{
	const float MAX_CAMERA_TOP = -0.1f;		//�J�����̏�����̍ő�l
	const float MAX_CAMERA_UNDER = 0.6f;	//�J�����̉������̍ő�l

	const float TARGETPOS_YUP = 45.0f;
	const float TARGETPOS_ULT_YUP = 50.0f;

	///�����_���王�_�܂ł̃x�N�g����ݒ�B
	const float CAMERA_POS_X = -160.0f;
	const float CAMERA_POS_Y = 80.0f;

	const float KNIGHT_CAMERA_POS_X = 80.0f;
	const float KNIGHT_CAMERA_POS_Y = 0.0f;

	const float KNIGHT_ULT_POS_X = -130.0f;
	const float KNIGHT_ULT_POS_Y = 100.0f;
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

	//�΂˃J�����̏������B
	m_springCamera.Init(
		*g_camera3D,		//�΂˃J�����̏������s���J�������w�肷��B
		1000.0f,			//�J�����̈ړ����x�̍ő�l�B
		false,				//�J�����ƒn�`�Ƃ̂����蔻�����邩�ǂ����̃t���O�Btrue���Ƃ����蔻����s���B
		1.0f				//�J�����ɐݒ肳��鋅�̃R���W�����̔��a�B��R������true�̎��ɗL���ɂȂ�B
	);

	
	//�ŏ��ɃL�����̔w�����f���悤�ɂ���
	CameraTarget(CAMERA_POS_X, CAMERA_POS_Y);
	m_springCamera.Refresh();

	return true;
}

void GameCamera::Update()
{
	//�|�[�Y�X�e�[�g�̂Ƃ��͏��������Ȃ�
	if (m_cameraState == enPauseState) {
		return;
	}

	//�v���C���[�����ꂽ��@��true�̊ԃJ���������̏�œ������Ȃ�
	


	//�����v���C���[���K�E�Z��ł�����(7=�K�E�Z�X�e�[�g)
	if (player->CharGetState() == 7)
	{
		m_springCamera.Refresh();
		KnightUltMoveFlag = true;
	}

	//�K�E�Z�t���O����������
	if (KnightUltMoveFlag)
	{
		knightUlt = FindGO<KnightUlt>("knightUlt");
		m_timer += g_gameTime->GetFrameDeltaTime();
		if (m_timer < 1.45) {
			KnightUltCamera();
		}
		//knightUlt����������Ă����
		else if (knightUlt!=nullptr)
		{
			//�J�������G�t�F�N�g��ǂ��悤�ɂ���
			ChaseUltEff();
		}
		else 
		{
			m_springCamera.Refresh();
			//�S�ă��Z�b�g
			KnightUltMoveFlag = false;
			SetCameraCharFrontFlag = false;
			m_timer = 0;
			CameraTarget(CAMERA_POS_X, CAMERA_POS_Y);
		}
		
		return;
	}


	//Y�{�^���������ꂽ��
	//�J�����̎��_���ŏ��̏�Ԃɖ߂�
	if (g_pad[0]->IsTrigger(enButtonY))
	{
		CameraTarget(CAMERA_POS_X, CAMERA_POS_Y);
	}
	//����������Ă��Ȃ��Ȃ�
	else
	{
		//�J�����̎��_��ݒ�
		FollowThePlayer();
	}

}

void GameCamera::FollowThePlayer()
{
	////�����_�̌v�Z
	//Vector3 TargetPos;
	TargetPos = player->GetCharPosition();

	TargetPos.y += TARGETPOS_YUP;

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
	m_springCamera.SetTarget(TargetPos);
	m_springCamera.SetPosition(newCamPos);

	//�J�����̍X�V�B
	m_springCamera.Update();
}

/// <summary>
/// �J�����̎��_���v���C���[�̔w���𑨂�����̂ɕύX����
/// </summary>
void GameCamera::CameraTarget(float X, float Y)
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
	toCameraPosXZ *= (X);
	//�����_���王�_�Ɍ������x�N�g����Y�����̐ݒ�
	Vector3 toCameraPosY=Vector3::AxisY;
	//�����_���王�_�Ɍ������x�N�g����Y������������
	toCameraPosY.y *= Y;

	//���_���v�Z����
	Vector3 newCameraPos = toCameraPosXZ + toCameraPosY;

	//���_(m_toCameraPos)������������
	m_toCameraPos.Set(newCameraPos);

	//�����_�̌v�Z
	Vector3 TargetPos;
	TargetPos = player->GetCharPosition();
	TargetPos.y += TARGETPOS_YUP;
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
	m_springCamera.SetTarget(TargetPos);
	m_springCamera.SetPosition(newCamPos);

	//�J�����̍X�V�B
	m_springCamera.Update();
}

/// <summary>
/// ���m���K�E�Z��ł������̃J�������[�N�@�@���Ȃ��牓��������
/// </summary>
void GameCamera::KnightUltCamera()
{
	//�J���������m�̐��ʂɃZ�b�g���Ă��Ȃ��Ȃ�
	if (SetCameraCharFrontFlag == false)
	{
		CameraTarget(KNIGHT_CAMERA_POS_X, KNIGHT_CAMERA_POS_Y);
		SetCameraCharFrontFlag = true;
	}
	//���m�ɍ��킹�ăJ��������]������
	else
	{
		
		////�����_�̌v�Z
	//Vector3 TargetPos;
		TargetPos = player->GetCharPosition();

		TargetPos.y += TARGETPOS_ULT_YUP;

		Vector3 toCameraPosOld = m_toCameraPos;

		//�p�b�h�̓��͂��g���ăJ�������񂷁Bx2.8
		float x = 2.4f;
		float y = 0.1f;

		//Y������̉�]
		Quaternion qRot;
		qRot.SetRotationDeg(Vector3::AxisY, x);
		qRot.Apply(m_toCameraPos);

		//X������̉�]�B
		Vector3 axisX;
		axisX.Cross(Vector3::AxisY, m_toCameraPos);
		axisX.Normalize();
		qRot.SetRotationDeg(axisX, -1.3f * y);
		qRot.Apply(m_toCameraPos);

		//m_toCameraPos.z -= 2.0f;

		//�J�����̈ʒu�̏Փˉ�������
		Vector3 newCamPos;
		m_cameraCollisionSolver.Execute(
			newCamPos,
			TargetPos + m_toCameraPos,
			TargetPos
		);

		//���_�ƒ����_��ݒ�
		m_springCamera.SetTarget(TargetPos);
		m_springCamera.SetPosition(newCamPos);

		//�J�����̍X�V�B
		m_springCamera.Update();
	}
}

void GameCamera::ChaseUltEff()
{
	//�v���C���[�̑O�������擾
	Vector3 toCameraPosXZ = knightUlt->GetForward();
	//���K��
	toCameraPosXZ.Normalize();
	//�����_���王�_�Ɍ������x�N�g����XZ������������
	toCameraPosXZ *= (KNIGHT_ULT_POS_X);
	//�����_���王�_�Ɍ������x�N�g����Y�����̐ݒ�
	Vector3 toCameraPosY = Vector3::AxisY;
	//�����_���王�_�Ɍ������x�N�g����Y������������
	toCameraPosY.y *= KNIGHT_ULT_POS_Y;

	//���_���v�Z����
	Vector3 newCameraPos = toCameraPosXZ + toCameraPosY;

	//���_(m_toCameraPos)������������
	m_toCameraPos.Set(newCameraPos);

	//�����_�̌v�Z
	Vector3 TargetPos;
	TargetPos = knightUlt->GetPosition();
	TargetPos.y += 10.0f;
	//���_���璍���_�ւ̃x�N�g�������߂�
	newCameraPos += TargetPos;

	//���_�ƒ����_��ݒ�
	m_springCamera.SetTarget(TargetPos);
	m_springCamera.SetPosition(newCameraPos);

	//�J�����̍X�V�B
	m_springCamera.Update();



}

