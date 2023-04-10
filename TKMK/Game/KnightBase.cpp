#include "stdafx.h"
#include "KnightBase.h"
#include "Status.h"
#include "GameUI.h"

KnightBase::KnightBase()
{
	//�X�e�[�^�X��ǂݍ���
	m_Status.Init("Knight");
	Lv=1;                    //���x��
	AtkSpeed=20;              //�U�����x

	Cooltime=5;            //�X�L���̃N�[���^�C��
	SkillTimer = Cooltime;

	AvoidanceCoolTime = 2;     ///����̃N�[���^�C��
	AvoidanceTimer = AvoidanceCoolTime;

	Point=0;                 //�G��|���Ď�ɓ��ꂽ�|�C���g
	GetExp=0;                //�����̓G��|�����Ƃ��̌o���l
	ExpTable=5;              //�o���l�e�[�u��
	//respawnNumber = 0;        //���X�|�[��������W�̔ԍ�

	
}

KnightBase::~KnightBase()
{

}

void KnightBase::SetModel()
{
	//�v���C���[
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/Knight/Knight_idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Walk].Load("Assets/animData/Knight/Knight_Walk.tka");
	m_animationClips[enAnimationClip_Walk].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Run].Load("Assets/animData/Knight/run.tka");
	m_animationClips[enAnimationClip_Run].SetLoopFlag(true);
	m_animationClips[enAnimationClip_ChainAtk].Load("Assets/animData/Knight/Knight_ChainAttack.tka");
	m_animationClips[enAnimationClip_ChainAtk].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Skill].Load("Assets/animData/Knight/Knight_Skill.tka");
	m_animationClips[enAnimationClip_Skill].SetLoopFlag(false);
	m_animationClips[enAnimationClip_UltimateSkill].Load("Assets/animData/Knight/Knight_UltimateAttack.tka");
	m_animationClips[enAnimationClip_UltimateSkill].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Damege].Load("Assets/animData/Knight/Knight_Damege.tka");
	m_animationClips[enAnimationClip_Damege].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Death].Load("Assets/animData/Knight/Knight_Death.tka");
	m_animationClips[enAnimationClip_Death].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Avoidance].Load("Assets/animData/Knight/Knight_Avoidance.tka");
	m_animationClips[enAnimationClip_Avoidance].SetLoopFlag(false);

	//���m���f����ǂݍ���
	m_modelRender.Init("Assets/modelData/character/Knight/Knight_02.tkm", m_animationClips, enAnimationClip_Num, enModelUpAxisZ);

	//�umixamorig:RightHand�v(�E��)�{�[����ID(�ԍ�)���擾����B
	m_swordBoneId = m_modelRender.FindBoneID(L"mixamorig:RightHand");
	//�U�����̃A�j���[�V�����C�x���g���m�̍��W��ID(�ԍ�)���擾����B
	AtkEndPosId = m_modelRender.FindBoneID(L"movePos");


	m_position = { 0.0f,0.0f,0.0f };
	m_modelRender.SetPosition(m_position);
	//m_modelRender.SetScale(Vector3(0.1f, 0.1f, 0.1f));

	m_rot.SetRotationY(0.0f);
	m_modelRender.SetRotation(m_rot);

	m_charCon.Init(
		15.0f,
		35.0f,
		m_position
	);
}

/// <summary>
/// �����̓G��|�����Ƃ��̌o���l�̏���
/// </summary>
/// <param name="GetExp">�����̓G�̌o���l</param>
void KnightBase::ExpProcess(int Exp)
{
	//�������x����10(Max)�Ȃ�
	if (Lv == 10)return;
	//���g�̌o���l�ɓG��|�����Ƃ��Ɏ�ɓ����o���l�𑫂�
	GetExp += Exp;
	//��ɓ��ꂽ�o���l���o���l�e�[�u���̂ق����傫��������
	if (GetExp < ExpTable) return;      //�����o��
	else {
		//�o���l�e�[�u������ɓ��ꂽ�o���l�̂ق����傫��������
		//���x���A�b�v
		LevelUp(LvUPStatus,m_Status,Lv);
		//���x���ɍ��킹�ă��x���̉摜��ύX����
		m_gameUI->LevelFontChange(Lv);
		switch (Lv)
		{
		case 2:
			ExpTable = 10;
			break;
		case 3:
			ExpTable = 20;
			break;
		case 4:
			ExpTable = 30;
			break;
		case 5:
			ExpTable = 40;
			break;
		case 6:
			ExpTable = 50;
			break;
		case 7:
			ExpTable = 60;
			break;
		case 8:
			ExpTable = 70;
			break;
		case 9:
			ExpTable = 80;
			break;
		default:
			break;
		}
	}
}

/// <summary>
/// �ړ�����
/// </summary>

/// <summary>
/// ��]����
/// </summary>
void KnightBase::Rotation()
{
	//x��z�̈ړ����x����������(�X�e�B�b�N�̓��͂���������)�B
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		//�L�����N�^�[�̕�����ς���B
		m_rot.SetRotationYFromDirectionXZ(m_moveSpeed);
		//�G�`������ɉ�]��������B
		m_modelRender.SetRotation(m_rot);
	}
}

/// <summary>
/// �U�����̓����蔻��̏���
/// </summary>
void KnightBase::AtkCollisiton()
{
	//�R���W�����I�u�W�F�N�g���쐬����B
	auto collisionObject = NewGO<CollisionObject>(0);
	Vector3 collisionPosition = m_position;
	//���W���v���C���[�̏����O�ɐݒ肷��B
	//collisionPosition += forward * 50.0f;
	//�{�b�N�X��̃R���W�������쐬����B
	collisionObject->CreateBox(collisionPosition, //���W�B
		Quaternion::Identity, //��]�B
		Vector3(70.0f, 15.0f, 15.0f) //�傫���B
	);
	collisionObject->SetName("player_attack");
	collisionObject->SetCreatorName(GetName());

	//�uSword�v�{�[���̃��[���h�s����擾����B
	Matrix matrix = m_modelRender.GetBone(m_swordBoneId)->GetWorldMatrix();

	//matrix.MakeRotationZ(90.0f);
	//�uSword�v�{�[���̃��[���h�s����R���W�����ɓK�p����B
	collisionObject->SetWorldMatrix(matrix);
}


/// <summary>
/// �K�E�Z�������̓����蔻��̏���
/// </summary>
/// <param name="oldpostion">�O�t���[���̍��W</param>
/// <param name="position">���݂̍��W</param>
void KnightBase::UltimateSkillCollistion(Vector3& oldpostion,Vector3& position)
{	
	//	//�R���W�����I�u�W�F�N�g���쐬����B
	//UltCollision = NewGO<CollisionObject>(0);
	//UltCollision->SetName("Knight_UltimateSkill");
	////�R���W�����������Ă��Ȃ��Ȃ�
	//if (UltCollisionSetFlag == false)
	//{
	//	//�R���W�����̍��W���v���C���[�Ɠ����ɐݒ�
	//	UltCollisionPos = position;
	//	UltCollisionPos.y += 50.0f;

	//	//�O�t���[���̍��W����
	//	Vector3 oldPosition = oldpostion;
	//	//�O�t���[���̍��W���猻�݂̃t���[���Ɍ������x�N�g�����v�Z����
	//	collisionRot = position - oldPosition;

	//	//Y�����̃x�N�g����0.0f�ɂ���
	//	collisionRot.y = 0.0f;

	//	//���K��
	//	collisionRot.Normalize();

	//	Quaternion rot;
	//	//Y�����̉�]�N�H�[�^�j�I�����쐬
	//	rot.SetRotationYFromDirectionXZ(collisionRot);
	//	//�x�N�g���ɃN�H�[�^�j�I����K��
	//	rot.Apply(oldpostion);

	//	//�R���W�����I�u�W�F�N�g���쐬����B
	//	collisionObject = NewGO<CollisionObject>(0);
	//	Vector3 collitionPosition = position;
	//	collitionPosition.y += 50.0f;
	//	//collisionPosition.y += 50.0f;
	//	//�{�b�N�X��̃R���W�������쐬����B
	//	collisionObject->CreateBox(collitionPosition, //���W�B
	//		Quaternion(rot), //��]�B
	//		Vector3(300.0f, 50.0f, 15.0f) //�傫���B
	//	);
	//	collisionObject->SetIsEnableAutoDelete(false);

	//	collisionObject->SetName("player_UltimateSkill");
	//	collisionObject->SetCreatorName(GetName());

	//	UltCollisionSetFlag = true;
	//}
	//else
	//{
	//	//�ړ����x�ݒ�
	//	UltCollisionPos += collisionRot * 4.0f;
	//	//���W��ݒ�
	//	collisionObject->SetPosition(UltCollisionPos);

	//}
}

void KnightBase::Collition()
{
	//��_���[�W�A�_�E�����A�K�E�Z�A�ʏ�U�����̓_���[�W��������Ȃ��B
	if (m_knightState == enKnightState_Damege || 
		m_knightState == enKnightState_Death ||
		m_knightState == enKnightState_UltimateSkill ||
		m_knightState == enKnightState_ChainAtk ||
		m_knightState == enKnightState_Skill ||
		m_knightState == enKnightState_Avoidance)
	{
		return;
	}
	
		//�G�̍U���p�̃R���W�������擾���閼�O�ꏏ�ɂ���
		const auto& collisions = g_collisionObjectManager->FindCollisionObjects("enemy_attack");
		//�R���W�����̔z���for���ŉ�
		for (auto collision : collisions)
		{
			//�R���W���������g�̃L�����R���ɓ���������
			if (collision->IsHit(m_charCon))
			{
				//�G�l�~�[�̍U���͂�����Ă���

				//hp��10���炷
				Dameged(Enemy_atk, m_Neutral_enemy);

			}
		}
	
	//��_���[�W�A�_�E�����A�K�E�Z�A�ʏ�U�����̓_���[�W��������Ȃ��B
	if (m_knightState == enKnightState_Damege ||
		m_knightState == enKnightState_Death ||
		m_knightState == enKnightState_UltimateSkill ||
		m_knightState == enKnightState_ChainAtk ||
		m_knightState == enKnightState_Skill ||
		m_knightState == enKnightState_Avoidance)
	{
		return;
	}
	//�����̌��̓����蔻��ɓ������Ă���
	//�����蔻��̖��O��ς��Ȃ��Ƃ����Ȃ�
	//�G�̍U���p�̃R���W�������擾���閼�O�ꏏ�ɂ���
	const auto& Knightcollisions = g_collisionObjectManager->FindCollisionObjects("player_attack");
	//�R���W�����̔z���for���ŉ�
	for (auto knightcollision : Knightcollisions)
	{
		//���̃R���W������������A�N�^�[������
		m_lastAttackActor = FindGO<Actor>(knightcollision->GetCreatorName());
		//�R���W���������g�̃L�����R���ɓ���������
		if (knightcollision->IsHit(m_charCon))
		{
			//���m�̍U���͕�HP�����炷�B
			//�|���ꂽ����̃|�C���g�𑝂₷
			/*Dameged(m_lastAttackActor->GetAtk(), m_lastAttackActor);*/
		}
	}

}

/// <summary>
/// �_���[�W���󂯂��Ƃ��̏���
/// </summary>
/// <param name="damege">�G�̍U����</param>
void KnightBase::Dameged(int damege, Actor* CharGivePoints)
{
	m_Status.Hp -= damege;
	//���g��HP��0�ȉ��Ȃ�
	if (m_Status.Hp <= 0) {
		//�|���ꂽ�Ƃ��̏����ɑJ��
		//���S�X�e�[�g
		m_knightState = enKnightState_Death;
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(17);
		se->Play(false);
		se->SetVolume(0.5f);
		m_Status.Hp = 0;
		//�U�����ꂽ���肪�����̓G�ȊO�Ȃ�
		if (CharGivePoints != nullptr)
		{
			//�|���ꂽ����̃|�C���g�𑝂₷
			CharGivePoints->PointProcess(Lv);
		}
	}
	else {
		//�_���[�W�X�e�[�g
		m_knightState = enKnightState_Damege;
		SoundSource * se = NewGO<SoundSource>(0);
		se->Init(12);
		se->Play(false);
		se->SetVolume(0.5f);
		//���G���ԃt���O
		//invincibleFlag = true;
	}
}

/// <summary>
/// ���x��4�ŕK�E�Z���g�p�����Ƃ��̏���
/// </summary>
void KnightBase::UltimateSkill()
{
	//���x����3������
levelDown(LvUPStatus, m_Status, Lv, 3);
	//�o���l�����Z�b�g
	ExpReset(Lv, GetExp);
	//���x���̌o���l�e�[�u���ɂ���
	ExpTableChamge(Lv, ExpTable);

	//���x���ɍ��킹�ă��x���̉摜��ύX����
	//m_gameUI->LevelFontChange(Lv);

	m_knightState = enKnightState_UltimateSkill;

}

/// <summary>
/// ���X�|�[��������W�̃Z�b�g
/// </summary>
void KnightBase::SetRespawn()
{
	//���X�|�[��������W0�Ԃ̎擾
	GetRespawnPos();
	//���X�|�[��������W�̃Z�b�g
	//�L�����R��
	m_charCon.SetPosition(m_respawnPos[respawnNumber]);
	//���m
	m_modelRender.SetPosition(m_respawnPos[respawnNumber]);
	m_modelRender.SetRotation(m_respawnRotation[respawnNumber]);
}

/// <summary>
/// ���g���|���ꂽ�Ƃ��̏���
/// </summary>
void KnightBase::Death()
{
	////���S�X�e�[�g
	//m_knightState = enKnightState_Death;
	//���x�����P������
	levelDown(LvUPStatus, m_Status, Lv,1);
	//HP���ő�ɂ���
	m_Status.Hp = m_Status.MaxHp;
	//�o���l�����Z�b�g
	ExpReset(Lv, GetExp);
	//����̃��x���̌o���l�e�[�u���ɂ���
	ExpTableChamge(Lv,ExpTable);

	//���x���ɍ��킹�ă��x���̉摜��ύX����
	//m_gameUI->LevelFontChange(Lv);
}

/// <summary>
/// �A�j���[�V�����Đ����ɒ����ړ�����������̌���
/// </summary>
/// <param name="moveSpeed">�X�e�B�b�N�̈ړ��ʂƏ�Z���������X�s�[�h�̒l</param>
/// <param name="stickL">�X�e�B�b�N�̈ړ��̓��͗�</param>
void KnightBase::AnimationMove(float moveSpeed,Vector3 stickL)
{
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;

	//�ړ��̓��͗ʂ��Ȃ��Ȃ�
	if (stickL.x == 0.0f&& stickL.y == 0.0f) {
		//�O�Ɉړ�
		stickL.x = 0.0f;
		stickL.y = 1.0f;
	}

	m_Skill_Forward = Vector3::Zero;
	m_Skill_Right = Vector3::Zero;

	//�J�����̑O�����ƉE�����̃x�N�g���������Ă���B
	m_Skill_Forward = g_camera3D->GetForward();
	m_Skill_Right = g_camera3D->GetRight();
	//y�����ɂ͈ړ������Ȃ��B
	m_Skill_Forward.y = 0.0f;
	m_Skill_Right.y = 0.0f;

	//���X�e�B�b�N�̓��͗ʂ�status�̃X�s�[�h����Z�B
	m_Skill_Right *= stickL.x * moveSpeed;
	m_Skill_Forward *= stickL.y * moveSpeed;
}

//�����ړ�������
void KnightBase::MoveStraight(Vector3& right, Vector3& forward)
{
	//�ړ�����
	//�ړ����x�ɃX�e�B�b�N�̓��͗ʂ����Z����B
	//Vector3 m_SkillSpeed; 
	m_moveSpeed = right + forward;
	//�L�����N�^�[�R���g���[���[���g���č��W���ړ�������B
	m_position = m_charCon.Execute(m_moveSpeed, 1.0f / 60.0f);
}

/// <summary>
/// �A�j���[�V�����Đ��̏���
/// </summary>
void KnightBase::PlayAnimation()
{
	m_modelRender.SetAnimationSpeed(1.0f);

	switch (m_knightState)
	{
	case enKnightState_Idle:
		m_modelRender.PlayAnimation(enAnimationClip_Idle,0.4f);
		break;
	case enKnightState_Walk:
		m_modelRender.PlayAnimation(enAnimationClip_Walk, 0.1f);
		break;
	case enKnightState_Run:
		m_modelRender.PlayAnimation(enAnimationClip_Run,0.2f);
		break;
	case enKnightState_ChainAtk:
		m_modelRender.PlayAnimation(enAnimationClip_ChainAtk, 0.3f);
		break;
	case enKnightState_Skill:
		m_modelRender.PlayAnimation(enAnimationClip_Skill, 0.3f);
		break;
	case enKnightState_UltimateSkill:
		//���������K�v�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I
		m_modelRender.SetAnimationSpeed(1.2f);
		m_modelRender.PlayAnimation(enAnimationClip_UltimateSkill,0.1);
		break;
	case enKnightState_Avoidance:
		m_modelRender.SetAnimationSpeed(1.5f);
		m_modelRender.PlayAnimation(enAnimationClip_Avoidance, 0.1f);
		break;
	case enKnightState_Damege:
		m_modelRender.PlayAnimation(enAnimationClip_Damege, 0.4f);
		break;
	case enKnightState_Death:
		m_modelRender.PlayAnimation(enAnimationClip_Death, 0.4f);
	default:
		break;
	}
}

/// <summary>
/// �A�j���[�V�����̃X�e�[�g�̏���
/// </summary>
void KnightBase::ManageState()
{
	switch (m_knightState)
	{
	case enKnightState_Idle:
		OnProcessIdleStateTransition();
		break;
	case enKnightState_Walk:
		OnProcessIdleStateTransition();
		break;
	case enKnightState_Run:
		OnProcessRunStateTransition();
		break;
	case enKnightState_ChainAtk:
		OnProcessChainAtkStateTransition();
		break;
	case enKnightState_Skill:
		OnProcessSkillAtkStateTransition();
		break;
	case enKnightState_UltimateSkill:
		OnProcessUltimateSkillAtkStateTransition();
		break;
	case enKnightState_Avoidance:
		OnProcessAvoidanceStateTransition();
		break;
	case enKnightState_Damege:
		OnProcessDamegeStateTransition();
		break;
	case enKnightState_Death:
		OnProcessDeathStateTransition();
		break;

	}
}

/// <summary>
/// �����A�j���[�V�������Đ�����Ă���Ȃ�B
/// </summary>
void KnightBase::OnProcessCommonStateTransition()
{
	//�X�e�B�b�N�̓��͗ʂ���������
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		if (Lv < 2) {
			m_knightState = enKnightState_Walk;
		}
		else
			//����X�e�[�g
		m_knightState = enKnightState_Run;

		return;
	}
	else
	{
		//�Ȃ�������ҋ@�X�e�[�g
		m_knightState = enKnightState_Idle;
		return;
	}
}

/// <summary>
/// Idle�A�j���[�V�������Đ�����Ă���Ƃ��̏���
/// </summary>
void KnightBase::OnProcessIdleStateTransition()
{
	OnProcessCommonStateTransition();
}

/// <summary>
/// Run�A�j���[�V�������Đ�����Ă���Ƃ��̏���
/// </summary>
void KnightBase::OnProcessRunStateTransition()
{
	OnProcessCommonStateTransition();
}

/// <summary>
/// FirstAtk�A�j���[�V�������Đ�����Ă���Ƃ��̏���
/// </summary>
void KnightBase::OnProcessChainAtkStateTransition()
{
	//�`�F�C���A�^�b�N�̃A�j���[�V�����Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�ҋ@�X�e�[�g
		//�U�����n�߂����̔����false�ɂ���
		AtkState = false;
		//�{�^���v�b�V���t���O��false�ɂ���
		pushFlag = false;
		m_knightState = enKnightState_Idle;
		OnProcessCommonStateTransition();
	}
}

/// <summary>
/// �X�L���̃A�j���[�V�����Đ�����Ă���Ƃ��̏���
/// </summary>
void KnightBase::OnProcessSkillAtkStateTransition()
{
	//�X�L���̃A�j���[�V�����Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		AtkState = false;
		SkillEndFlag = true;
		//�{�^���v�b�V���t���O��false�ɂ���
		pushFlag = false;
		//�ҋ@�X�e�[�g
		m_knightState = enKnightState_Idle;
		OnProcessCommonStateTransition();
	}
}

/// <summary>
/// �K�E�Z�A�j���[�V�������Đ�����Ă���Ƃ��̏���
/// </summary>
void KnightBase::OnProcessUltimateSkillAtkStateTransition()
{
	//�K�E�Z�A�j���[�V�����Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		AtkState = false;
		//�{�^���v�b�V���t���O��false�ɂ���
		pushFlag = false;
		//�ҋ@�X�e�[�g
		m_knightState = enKnightState_Idle;
		OnProcessCommonStateTransition();
	}
}

//����̃A�j���[�V�������Đ�����Ă���Ƃ��̏���
void KnightBase::OnProcessAvoidanceStateTransition()
{
	//����̃A�j���[�V�����Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		AvoidanceFlag = false;
		AvoidanceEndFlag = true;
		//�{�^���v�b�V���t���O��false�ɂ���
		pushFlag = false;
		//�ҋ@�X�e�[�g
		m_knightState = enKnightState_Idle;
		OnProcessCommonStateTransition();
	}
}

/// <summary>
/// Damege�A�j���[�V�������Đ�����Ă���Ƃ��̏���
/// </summary>
void KnightBase::OnProcessDamegeStateTransition()
{
	//�_���[�W���󂯂��Ƃ��̃A�j���[�V�����Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�ҋ@�X�e�[�g
		m_knightState = enKnightState_Idle;
		//���G���ԃX�e�[�g
		//invincibleFlag = false;
		OnProcessCommonStateTransition();
	}
}

/// <summary>
/// Death�A�j���[�V�������Đ�����Ă���Ƃ��̏���
/// </summary>
void KnightBase::OnProcessDeathStateTransition()
{
	//�_���[�W���󂯂��Ƃ��̃A�j���[�V�����Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//���X�|�[��������W�Ɏ��g�̍��W���Z�b�g
		SetRespawn();
		Death();
		//�ҋ@�X�e�[�g
		m_knightState = enKnightState_Idle;
		OnProcessCommonStateTransition();
	}
}