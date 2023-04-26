#include "stdafx.h"
#include "GameCamera.h"

#include "Game.h"
#include "KnightBase.h"
#include "KnightPlayer.h"
#include "WizardPlayer.h"
#include "Player.h"
#include "KnightUlt.h"
#include "Actor.h"

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
	player_name = player->GetName();

	m_actors = game->GetActors();
	for (auto player : m_actors)
	{
		if (player->IsMatchName(player_name) == true)
		{
			player_actor = player;
		}
	}

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
	CameraTarget(CAMERA_POS_X, CAMERA_POS_Y, player_actor);

	

	return true;
}

void GameCamera::Update()
{
	//�|�[�Y�X�e�[�g�̂Ƃ��͏��������Ȃ�
	if (m_cameraState == enPauseState) {
		return;
	}

	StateControl();
}

void GameCamera::StateControl()
{
	switch (m_enCameraState)
	{
	case m_enNomarlCameraState:
		NomarlCamera();
		break;
	case m_enUltRotCameraState:
		UltRotCamera();
		break;
	case m_enChaseCameraState:
		ChaseCamera();
		break;
	default:
		break;
	}
}

void GameCamera::NomarlCamera()
{
	for (auto actor : m_actors) {
		//�����v���C���[���K�E�Z��ł�����(7=�K�E�Z�X�e�[�g)
		if (actor->NowCharState() == Actor::enCharState_UltimateSkill && KnightUltFlag==false)
		{
			KnightUltFlag = true;
			//�J�����X�e�[�g����]�X�e�[�g�Ɉڂ�
			m_enCameraState = m_enUltRotCameraState;
			ultactor = actor;

			return;
		}
	}


	//Y�{�^���������ꂽ��
	//�J�����̎��_���ŏ��̏�Ԃɖ߂�
	if (g_pad[0]->IsTrigger(enButtonY))
	{
		CameraTarget(CAMERA_POS_X, CAMERA_POS_Y, player_actor);
	}
	//����������Ă��Ȃ��Ȃ�
	else
	{
		//�J�����̎��_��ݒ�
		FollowThePlayer();
	}


}

void GameCamera::UltRotCamera()
{
	//�J���������m�̐��ʂɃZ�b�g���Ă��Ȃ��Ȃ�
	if (SetCameraCharFrontFlag == false)
	{
		m_springCamera.Refresh();
		CameraTarget(KNIGHT_CAMERA_POS_X, KNIGHT_CAMERA_POS_Y, ultactor);
		//�L�����̐��ʂ���J�����Ɍ������x�N�g��
		m_keepDiff = m_toCameraPos;

		SetCameraCharFrontFlag = true;

		KnightUltCamera(ultactor,true);
	}

	//�v���C���[����J�����Ɍ������x�N�g�����v�Z����
	Vector3 start = m_keepDiff;
	Vector3 end = m_CameraFromActorDiff;
	start.y = 0.0f;
	end.y = 0.0f;
	//����
	float nai;
	nai = start.Dot(end);
	//�����p
	float Start = start.Length();
	float End = end.Length();

	float cos_sita = nai / (Start * End);
	sita = acos(cos_sita);
	sita = Math::RadToDeg(sita);

	if (sita < 175.0f) {
		KnightUltCamera(ultactor,false);
	}
	else
	{
		m_springCamera.Refresh();
		m_enCameraState = m_enChaseCameraState;
	}


}

void GameCamera::ChaseCamera()
{
	knightUlt = FindGO<KnightUlt>("knightUlt");
	//knightUlt����������Ă����
	    if (knightUlt != nullptr)
		{
			//�J�������G�t�F�N�g��ǂ��悤�ɂ���
			ChaseUltEff();
		}
		else
		{
			//m_springCamera.Refresh();
			////�S�ă��Z�b�g
			//SetCameraCharFrontFlag = false;
			//KnightUltFlag = false;

			////�K�E�Z��������Actor�̃X�e�[�g��߂�
			////ultactor->UltSkillEnd();

			////CameraTarget(CAMERA_POS_X, CAMERA_POS_Y, ultactor);
			////�v���C���[�̃J���������Z�b�g����
			//CameraTarget(CAMERA_POS_X, CAMERA_POS_Y, player_actor);

			//m_enCameraState = m_enNomarlCameraState;
		
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
void GameCamera::CameraTarget(float X, float Y,Actor*actor)
{

	//�v���C���[�̑O�������擾
	Vector3 toCameraPosXZ = actor->GetForward();
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
	TargetPos = actor->GetPosition();
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
void GameCamera::KnightUltCamera(Actor*actor, bool reset)
{
	
	//���m�ɍ��킹�ăJ��������]������
		
		////�����_�̌v�Z
	//Vector3 TargetPos;
		TargetPos = actor->GetPosition();

		TargetPos.y += TARGETPOS_ULT_YUP;

		Vector3 toCameraPosOld = m_toCameraPos;

		//�p�b�h�̓��͂��g���ăJ�������񂷁Bx2.8
		float x = 1.9f;
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

		//�A�N�^�[����J�����Ɍ������x�N�g��
		m_CameraFromActorDiff = newCamPos - TargetPos;

		if (reset == false) {
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

void GameCamera::GameCameraUltEnd() {

	m_springCamera.Refresh();
	//�S�ă��Z�b�g
	SetCameraCharFrontFlag = false;
	KnightUltFlag = false;

	//�K�E�Z��������Actor�̃X�e�[�g��߂�
	//ultactor->UltSkillEnd();

	//CameraTarget(CAMERA_POS_X, CAMERA_POS_Y, ultactor);
	//�v���C���[�̃J���������Z�b�g����
	CameraTarget(CAMERA_POS_X, CAMERA_POS_Y, player_actor);

	m_enCameraState = m_enNomarlCameraState;

}