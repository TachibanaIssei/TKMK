#include "stdafx.h"
#include "Neutral_Enemy.h"
#include "Game.h"
#include <time.h>
#include <stdlib.h>
#include "KnightPlayer.h"
#include "GameCamera.h"
#include "KnightAI.h"
#include "Player.h"
#include "MagicBall.h"
#include "Actor.h"
//#include <vector>
//#include <algorithm>

namespace {
	const float HP_WINDOW_WIDTH = 1152.0f;
	const float HP_WINDOW_HEIGHT = 648.0f;
	const float HP_BER_WIDTH = 178.0f;
	const float HP_BER_HEIGHT = 22.0f;
	const Vector3 HP_BER_SIZE = Vector3(HP_BER_WIDTH, HP_BER_HEIGHT, 0.0f);
	const float RADIUS = 100.0f;
}
Neutral_Enemy::Neutral_Enemy()
{

}

Neutral_Enemy::~Neutral_Enemy()
{
	m_game = FindGO<Game>("game");
	//m_game->Enemys();
	if (m_game != nullptr) {
		m_game->RemoveEnemyFromList(this);
	}
}

bool Neutral_Enemy::Start()
{
	//�A�j���[�V������ǂݍ��ށB
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/Neutral_Enemy/Idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Run].Load("Assets/animData/Neutral_Enemy/Run.tka");
	m_animationClips[enAnimationClip_Run].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Attack].Load("Assets/animData/Neutral_Enemy/Attack.tka");
	m_animationClips[enAnimationClip_Attack].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Death].Load("Assets/animData/Neutral_Enemy/Death.tka");
	m_animationClips[enAnimationClip_Death].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Damage].Load("Assets/animData/Neutral_Enemy/Damage.tka");
	m_animationClips[enAnimationClip_Damage].SetLoopFlag(false);

	//モデルを読み込む。
	enemyColorRam = rand() % 10;

	if (enemyColorRam <= 5)
	{
		//白
		m_modelRender.Init("Assets/modelData/character/Neutral_Enemy/Ghost_White/Ghost_White.tkm", m_animationClips, enAnimationClip_Num);
		m_enemyKinds = enEnemyKinds_White;
	}
	else if (enemyColorRam <= 7)
	{
		//緑
		m_modelRender.Init("Assets/modelData/character/Neutral_Enemy/Neutral_Enemy.tkm", m_animationClips, enAnimationClip_Num);
		m_enemyKinds = enEnemyKinds_Green;
	}
	else if (enemyColorRam <= 9)
	{
		//赤
		m_modelRender.Init("Assets/modelData/character/Neutral_Enemy/Ghost_Red/Ghost_Red.tkm", m_animationClips, enAnimationClip_Num);
		m_enemyKinds = enEnemyKinds_Red;
	}

	//��W��ݒ�
	m_modelRender.SetPosition(m_position);
	//��]��ݒ肷��B
	m_modelRender.SetRotation(m_rot);
	//�傫����ݒ肷��B
	m_modelRender.SetScale(m_scale);
	//�傫������
	//�L�����N�^�[�R���g���[���[���B
	m_charaCon.Init(
		9.0f,			//���a�B
		20.0f,			//�����B
		m_position		//��W�B
	);
	//�X�t�B�A�R���C�_�[���B
	m_sphereCollider.Create(1.0f);

	//���̃{�[����ID��擾����
	m_AttackBoneId = m_modelRender.FindBoneID(L"HeadTipJoint");

	m_HPBar.Init("Assets/sprite/zako_HP_bar.DDS", HP_BER_WIDTH, HP_BER_HEIGHT);
	//m_HPBar.SetPivot(PIVOT);

	m_HPBack.Init("Assets/sprite/zako_HP_background.DDS", HP_WINDOW_WIDTH, HP_WINDOW_HEIGHT);

	m_HPFrame.Init("Assets/sprite/HP_flame_mushroom.DDS", HP_WINDOW_WIDTH, HP_WINDOW_HEIGHT);

	//�A�j���[�V�����C�x���g�p�̊֐���ݒ肷��B
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	//�������B
	srand((unsigned)time(NULL));
	m_forward = Vector3::AxisY;
	m_rot.Apply(m_forward);

	//�X�e�[�^�X��ǂݍ���
	m_Status.Init("Enemy");

	//����p�̃p�X��ǂݍ���
	m_EnemyPoslevel.Init("Assets/level3D/enemyPos.tkl", [&](LevelObjectData& objData) {

		if (objData.ForwardMatchName(L"Pos") == true) {
			SetPatrolPos(objData.position, objData.number);
			RadiusPos = objData.position;

			return true;
		}

	});

	return true;
}

void Neutral_Enemy::Update()
{
	// �|�[�Y���͉����Ȃ�
	if (m_Neutral_EnemyState == enNeutral_Enemy_Pause) {
		return;
	}

	//�T����B
	//Search();
	//�ǐՏ����B
	//Chase();

	//��]�����B
	Rotation();
	//�����蔻��B
	Collision();
	//�U�������B
	//Attack();
	//�A�j���[�V�����̍Đ��B
	PlayAnimation();
	//�X�e�[�g�̑J�ڏ����B
	ManageState();
	HPBar();
	//���f���̍X�V�B
	m_modelRender.Update();
}

void Neutral_Enemy::Move()
{
	Vector3 diff = m_forward;
	diff.Normalize();
	////�ړ����x��ݒ肷��B
	m_moveSpeed = diff * m_Status.Speed;
	m_forward.Normalize();
	Vector3 moveSpeed = m_forward * m_Status.Speed + m_hagikiPower;
	if (m_hagikiPower.Length() < 10.0f) {
		m_hagikiPower *= 0.99f;
	}
	else {
		m_hagikiPower = Vector3::Zero;
	}
	m_position = m_charaCon.Execute(moveSpeed, g_gameTime->GetFrameDeltaTime());
	m_modelRender.SetPosition(m_position);

}

void Neutral_Enemy::Rotation()
{
	
	if (fabsf(m_moveSpeed.x) < 0.001f
		&& fabsf(m_moveSpeed.z) < 0.001f) {
		//m_moveSpeed.x��m_moveSpeed.z�̐�Βl���Ƃ��0.001�ȉ��Ƃ������Ƃ�
		//���̃t���[���ł̓L�����͈ړ����Ă��Ȃ��̂Ő�񂷂�K�v�͂Ȃ��B
		return;
	}
	//atan2��tan�Ƃ̒l��p�x(���W�A���P��)�ɕϊ����Ă����֐��B
	//m_moveSpeed.x / m_moveSpeed.z�̌��ʂ�tan�ƂɂȂ�B
	//atan2��g�p���āA�p�x��߂Ă���B
	//���ꂪ��]�p�x�ɂȂ�B
	float angle = atan2(-m_moveSpeed.x, m_moveSpeed.z);
	//atan���Ԃ��Ă���p�x�̓��W�A���P�ʂȂ̂�
	//SetRotationDeg�ł͂Ȃ�SetRotation��g�p����B
	m_rot.SetRotationY(-angle);

	//��]��ݒ肷��B
	m_modelRender.SetRotation(m_rot);

	//�v���C���[�̑O�x�N�g����v�Z����B
	m_forward = Vector3::AxisZ;
	m_rot.Apply(m_forward);
}

//�Փ˂����Ƃ��ɌĂ΂��֐��I�u�W�F�N�g(�Ǘp)
struct SweepResultWall :public btCollisionWorld::ConvexResultCallback
{
	bool isHit = false;						//�Փ˃t���O�B

	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		//�ǂƂԂ���ĂȂ������B
		if (convexResult.m_hitCollisionObject->getUserIndex() != enCollisionAttr_Wall) {
			//�Փ˂����͕̂ǂł͂Ȃ��B
			return 0.0f;
		}

		//�ǂƂԂ������B
		//�t���O��true�ɁB
		isHit = true;
		return 0.0f;
	}
};

void Neutral_Enemy::Chase()
{
	//�ǐՃX�e�[�g�łȂ��Ȃ�A�ǐՏ����͂��Ȃ��B
	if (m_Neutral_EnemyState != enNeutral_Enemy_Chase)
	{
		return;
	}
	//Vector3 diff = player->GetCharPosition() - m_position;

	Vector3 diff = m_targetActor->GetPosition() - m_position;
	diff.Normalize();
	//�ړ����x��ݒ肷��B
	m_moveSpeed = diff * m_Status.Speed;
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	if (m_charaCon.IsOnGround()) {
		//�n�ʂɂ����B
		m_moveSpeed.y = 0.0f;
	}
	Vector3 modelPosition = m_position;
	//�����Ƃ������f���̍�W�����B
	modelPosition.y += 2.5f;
	m_modelRender.SetPosition(modelPosition);

}

void Neutral_Enemy::Collision()
{
	//�U�����A�f�X���͓����蔻��̏�����s��Ȃ�
	if (m_Neutral_EnemyState == enNeutral_Enemy_ReceiveDamage || m_Neutral_EnemyState == enNeutral_Enemy_Death)
	{
		return;
	}

	//�G�̍U���p�̃R���W������擾����
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("player_attack");
	//�q���W�����̔z���for���ŉ�
	for (auto collision : collisions)
	{
		if (collision->IsHit(m_charaCon))
		{
			//���̃R���W����������A�N�^�[���
			m_lastAttackActor = FindGO<Actor>(collision->GetCreatorName());

			//HP��炷
			m_Status.Hp -= m_lastAttackActor->GetAtk();

			//HP��0�ɂȂ����
			if (m_Status.Hp <= 0)
			{
				//����Ɍo���l��n��
				m_lastAttackActor->ExpProcess(Exp);
				//Deathflag = true;
				//���S�X�e�[�g�ɑJ�ڂ���B
				m_Neutral_EnemyState = enNeutral_Enemy_Death;
				m_Neutral_Enemy = nullptr;
			}
			else {
				//��_���[�W�X�e�[�g�ɑJ�ڂ���B
				m_Neutral_EnemyState = enNeutral_Enemy_ReceiveDamage;
				//��ʉ��Đ�
			}
		}
	}
	//�G�̍U���p�̃R���W������擾����
	const auto& Ultcollisions = g_collisionObjectManager->FindCollisionObjects("player_UltimateSkill");
	//�q���W�����̔z���for���ŉ�
	for (auto collision : Ultcollisions)
	{
		if (collision->IsHit(m_charaCon))
		{
			//���̃R���W����������A�N�^�[���
			m_lastAttackActor = FindGO<Actor>(collision->GetCreatorName());

			//hp��炷
			m_Status.Hp -= 100;
			if (m_Status.Hp < 0)
			{
				//����Ɍo���l��n��
				m_lastAttackActor->ExpProcess(Exp);
				//���S�X�e�[�g�ɑJ�ڂ���B
				m_Neutral_EnemyState = enNeutral_Enemy_Death;
				m_Neutral_Enemy = nullptr;
			}
			else {
				//��_���[�W�X�e�[�g�ɑJ�ڂ���B
				m_Neutral_EnemyState = enNeutral_Enemy_ReceiveDamage;
				//��ʉ��Đ�
			}
		}
	}
	//�U�����A�f�X���͓����蔻��̏�����s��Ȃ�
	if (m_Neutral_EnemyState == enNeutral_Enemy_ReceiveDamage || m_Neutral_EnemyState == enNeutral_Enemy_Death)
	{
		return;
	}

	//�G�̍U���p�̃R���W������擾����
	const auto& AIcollisions = g_collisionObjectManager->FindCollisionObjects("player_attack");
	//�q���W�����̔z���for���ŉ�
	for (auto AIcollision : AIcollisions)
	{
		if (AIcollision->IsHit(m_charaCon))
		{
			//���̃R���W����������A�N�^�[���
			m_lastAttackActor = FindGO<Actor>(AIcollision->GetCreatorName());

			//�v���C���[�̍U���͂�擾
			//���̂�m_knightAI��null
			//HP��炷
			m_Status.Hp -= m_lastAttackActor->GetAtk();
			//m_Status.Hp -= m_knightAI->SetKnightAIAtk();


			//HP��0�ɂȂ����
			if (m_Status.Hp <= 0)
			{
				//����Ɍo���l��n��
				m_lastAttackActor->ExpProcess(Exp);
				//���m�Ɍo���l��n��
				//m_knightAI->ExpProcess(Exp);
				//倒した時の報酬を倒した人に渡す
				// 赤…攻撃力を50あげる 緑…体力を上げる　白…何もしない
				//緑の場合
				if (m_enemyKinds == enEnemyKinds_Green)
				{
					m_lastAttackActor->HpUp(HpPass);
					if (m_lastAttackActor->GetMaxHp() < m_lastAttackActor->GetHp())
					{
						m_lastAttackActor->HpReset(m_lastAttackActor->GetMaxHp());
					}
				}
				//赤の場合
				else if (m_enemyKinds == enEnemyKinds_Red)
				{
					m_lastAttackActor->AtkUp(AtkPass);
				}
				//Deathflag = true;
				//���S�X�e�[�g�ɑJ�ڂ���B
				m_Neutral_EnemyState = enNeutral_Enemy_Death;
			}
			else {
				//��_���[�W�X�e�[�g�ɑJ�ڂ���B
				m_Neutral_EnemyState = enNeutral_Enemy_ReceiveDamage;
				//��ʉ��Đ�
			}
		}
	}

	//�U�����A�f�X���͓����蔻��̏�����s��Ȃ�
	if (m_Neutral_EnemyState == enNeutral_Enemy_ReceiveDamage || m_Neutral_EnemyState == enNeutral_Enemy_Death)
	{
		return;
	}
	//���@�g���̍U���p�̃R���W������擾����
	const auto& Wizardcollisions = g_collisionObjectManager->FindCollisionObjects("Wizard_MagicBall");
	//�R���W�����̔z���for���ŉ�
	for (auto Wizardcollision : Wizardcollisions)
	{
		if (Wizardcollision->IsHit(m_charaCon))
		{
			//���̃R���W����������A�N�^�[���
			m_lastAttackActor = FindGO<Actor>(Wizardcollision->GetCreatorName());
			//magicBall = FindGO<MagicBall>("magicBall");
			//���@�g���̍U���͂�擾
			//HP��炷
			m_Status.Hp -= m_lastAttackActor->GetAtk();

			//HP��0�ɂȂ����
			if (m_Status.Hp <= 0)
			{
				//player = FindGO<Player>("player");
				//����Ɍo���l��n��
				m_lastAttackActor->ExpProcess(Exp);
				//���@�g���Ɍo���l��n��
				//player->CharSetExpProcess(Exp);
				//Deathflag = true;
				//���S�X�e�[�g�ɑJ�ڂ���B
				m_Neutral_EnemyState = enNeutral_Enemy_Death;
			}
			else {
				//��_���[�W�X�e�[�g�ɑJ�ڂ���B
				m_Neutral_EnemyState = enNeutral_Enemy_ReceiveDamage;
				//��ʉ��Đ�
			}
		}
	}
}

void Neutral_Enemy::Attack()
{
	//�U���X�e�[�g�ł͂Ȃ������
	if (m_Neutral_EnemyState != enNeutral_Enemy_Attack)
	{
		return;
	}

}

bool Neutral_Enemy::Search()
{
	//�S�Ă�Actor�𒲂ׂ�
	for (Actor* actor : m_game->GetActors()) {

		Vector3 ActorPosition = actor->GetPosition();
		Vector3 diff = ActorPosition - m_position;
		diff.Normalize();
		float angle = acosf(diff.Dot(m_forward));
	
		//Actor�����E��ɋ�����B
		if (Math::PI * 0.8f > fabsf(angle) && (ActorPosition - m_position).LengthSq() <= 100.0f * 100.0f)
		{
			m_targetActor = actor;

			return true;
		}
	}

	//�ǂ�Actor���E��ɂ��Ȃ��B
	m_targetActor = nullptr;

	return false;
}

void Neutral_Enemy::MakeAttackCollision()
{
	//�U������p�̃R���W�����I�u�W�F�N�g��쐬����B
	auto collisionObject = NewGO<CollisionObject>(0);
	//���̃{�[���̃��[���h�s���擾����B
	Matrix matrix = m_modelRender.GetBone(m_AttackBoneId)->GetWorldMatrix();
	//�{�b�N�X��̃R���W������쐬����B
	collisionObject->CreateSphere(m_position, Quaternion::Identity,20.0f);
	collisionObject->SetWorldMatrix(matrix);
	collisionObject->SetName("enemy_attack");
	collisionObject->SetCreatorName(GetName());
}

void Neutral_Enemy::ProcessCommonStateTransition()
{
	
	//�e�^�C�}�[���B
	m_idleTimer = 0.0f;
	m_chaseTimer = 0.0f;

	if (m_targetActor!=nullptr)
	{
		Vector3 diff = m_targetActor->GetPosition() - m_position;

		diff.Normalize();
		//�ړ����x��ݒ肷��B
		m_moveSpeed = diff;
		

		//�U���ł��鋗���Ȃ�B
		if (CanAttack() == true)
		{
			//�����ɂ��āA�U�����邩�ҋ@�����邩��肷��B	
			{
				//�����ɂ��āA�U�����邩�ҋ@�����邩��肷��B	
		/*		int ram = rand() % 100;
				if (ram > 50)*/

				//�U���X�e�[�g�Ɉڍs����B
				if (m_Neutral_EnemyState == enNeutral_Enemy_Attack) {
					m_Neutral_EnemyState = enNeutral_Enemy_Chase;
				}
				else {
					m_Neutral_EnemyState = enNeutral_Enemy_Attack;
				}
				m_UnderAttack = false;
				return;
			}
		}
		//�U���ł��Ȃ������Ȃ�B
		else
		{

			//�ǐՃX�e�[�g�Ɉڍs����B
			m_Neutral_EnemyState = enNeutral_Enemy_Chase;
			return;

		}
	}
	//�G������Ȃ���΁B
	else
	{
		m_Neutral_EnemyState = enNEutral_Enemy_Patrol;
	}
}

void Neutral_Enemy::ProcessIdleStateTransition()
{
	m_idleTimer += g_gameTime->GetFrameDeltaTime();

	//�ҋ@���Ԃ������x�o�߂�����B
	if (m_idleTimer >= 0.5f)
	{
		//���̃X�e�[�g�֑J�ڂ���B
		ProcessCommonStateTransition();

	}

}
void Neutral_Enemy::ProcessRunStateTransition()
{
	//���̃X�e�[�g�֑J�ڂ���B
	ProcessCommonStateTransition();
}

void Neutral_Enemy::ProcessChaseStateTransition()
{
	Chase();

	if (Search() == false) {
		m_Neutral_EnemyState = enNEutral_Enemy_Patrol;
	}
	else {
		//�U���ł��鋗���Ȃ�B
		if (CanAttack())
		{
			//���̃X�e�[�g�֑J�ڂ���B
			ProcessCommonStateTransition();
			return;
		}
	}

	m_chaseTimer += g_gameTime->GetFrameDeltaTime();
	//�ǐՎ��Ԃ������x�o�߂�����B
	if (m_chaseTimer >= 0.5f)
	{
		//���̃X�e�[�g�֑J�ڂ���B
		ProcessCommonStateTransition();
	}
}

void Neutral_Enemy::ProcessAttackStateTransition()
{
	Attack();
	//�U���A�j���[�V�����̍Đ����I������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//���̃X�e�[�g�֑J�ڂ���B
		ProcessCommonStateTransition();
	}
}

void Neutral_Enemy::ProcessReceiveDamageStateTransition()
{
	//��_���[�W�A�j���[�V�����̍Đ����I������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		if (m_lastAttackActor == nullptr) {
			//�Ώۂ����Ȃ��̂ŏ��񂷂�B
			m_Neutral_EnemyState = enNEutral_Enemy_Patrol;

			return;
		}

		//�U�����ꂽ�狗���֌W�����ɁA��芸�����ǐՂ�����B
		m_Neutral_EnemyState = enNeutral_Enemy_Chase;
		//Vector3 diff = player->GetCharPosition() - m_position;
		//diff.Normalize();
		//�ړ����x��ݒ肷��B
		//m_moveSpeed = diff * m_Status.Speed;
		m_targetActor = m_lastAttackActor;
	}
}

void Neutral_Enemy::ProcessDeathStateTransition()
{
	//�_�E���A�j���[�V�����̍Đ����I������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//m_game->GetNeutral_EnemyContaier().erase(std::remove(m_game->GetNeutral_EnemyContaier().begin(),
		//	m_game->GetNeutral_EnemyContaier().end(), this), m_game->GetNeutral_EnemyContaier().end()/*std::cend(m_game->GetNeutral_EnemyContaier()*/);
		m_game->SubNeutral_EnemyContaier();
		//���g��폜����B
		DeleteGO(this);
	}
}
void Neutral_Enemy::ProcessPatrolStateTransition()
{
	//�G�l�~�[�����̏���擾����
	std::vector<Neutral_Enemy*>& enemys = m_game->GetNeutral_Enemys();
	for (auto Enemys : enemys)
	{
	/*	if (Enemys == this) {
			continue;
		}*/
		//�擾�����G�l�~�[�����̍�W��擾
		Vector3 enemyPos = Enemys->GetPosition();
		Vector3 diff = m_position - enemyPos;
		if (diff.Length() < 50.0f)
		{
			diff.Normalize();
			m_hagikiPower += diff * 20.0f;
		}
	}
	if (P < 0 || P > 8)
	{
		Vector3 newForward = m_patrolPos[0] - m_position;
		Vector3 distance = newForward;
		newForward.Normalize();
		m_forward = newForward;
		Move();
		if (distance.Length() <= 100.0f)
		{
			//1����ɂ��������+�P����
			int ram = rand() % 100 /*+ 1*/;
			if (ram >= 0)
			{
				P = 1;
			}
			if (ram > 25)
			{
				P = 3;
			}
			if (ram > 50)
			{
				P = 5;
			}
			if (ram > 75)
			{
				P = 7;
			}
		}

	}
	if (P == 0)
	{
		Vector3 newForward = m_patrolPos[0] - m_position;
		Vector3 distance = newForward;
		newForward.Normalize();
		m_forward = newForward;
		Move();
		if (distance.Length() <= 10.0f)
		{

			//1����ɂ��������+�P����
			

		}

	}
	if (P == 1)
	{
		Vector3 newForward2 = m_patrolPos[1] - m_position;
		Vector3 distance2 = newForward2;
		newForward2.Normalize();
		m_forward = newForward2;
		Move();
		if (distance2.Length() <= 10.0f)
		{

			int ram = rand() % 100;
			if (ram < 50)
			{
				P = 2;
			}
			if (ram > 50)
			{
				P = 9;
			}

		}

	}
	if (P == 2)
	{
		Vector3 newForward3 = m_patrolPos[2] - m_position;
		Vector3 distance3 = newForward3;
		newForward3.Normalize();
		m_forward = newForward3;
		Move();
		if (distance3.Length() <= 10.0f)
		{

			int ram = rand() % 100;
			if (ram < 50)
			{
				P = 1;
			}
			if (ram > 50)
			{
				P = 3;
			}

		}

	}
	if (P == 3)
	{
		Vector3 newForward4 = m_patrolPos[3] - m_position;
		Vector3 distance4 = newForward4;
		newForward4.Normalize();
		m_forward = newForward4;
		Move();
		if (distance4.Length() <= 10.0f)
		{

			int ram = rand() % 100;
			if (ram > 0)
			{
				P = 2;
			}
			/*if (ram > 33)
			{
				P = 0;
			}*/
			if (ram > 66)
			{
				P = 4;
			}

		}
	}
	if (P == 4)
	{
		Vector3 newForward5 = m_patrolPos[4] - m_position;
		Vector3 distance5 = newForward5;
		newForward5.Normalize();
		m_forward = newForward5;
		Move();
		if (distance5.Length() <= 10.0f)
		{

			int ram = rand() % 100;
			if (ram < 50)
			{
				P = 3;
			}
			if (ram > 50)
			{
				P = 5;
			}

		}
	}
	if (P == 5)
	{
		Vector3 newForward6 = m_patrolPos[5] - m_position;
		Vector3 distance6 = newForward6;
		newForward6.Normalize();
		m_forward = newForward6;
		Move();
		if (distance6.Length() <= 10.0f)
		{

			int ram = rand() % 100;
			if (ram > 0)
			{
				P = 4;
			}
			/*if (ram > 33)
			{
				P = 0;
			}*/
			if (ram > 66)
			{
				P = 6;
			}

		}
	}
	if (P == 6)
	{
		Vector3 newForward7 = m_patrolPos[6] - m_position;
		Vector3 distance7 = newForward7;
		newForward7.Normalize();
		m_forward = newForward7;
		Move();
		if (distance7.Length() <= 10.0f)
		{
			int ram = rand() % 100;
			if (ram < 50)
			{
				P = 5;
			}
			if (ram > 50)
			{
				P = 7;
			}

		}
	}
	if (P == 7)
	{
		Vector3 newForward8 = m_patrolPos[7] - m_position;
		Vector3 distance8 = newForward8;
		newForward8.Normalize();
		m_forward = newForward8;
		Move();
		if (distance8.Length() <= 10.0f)
		{

			int ram = rand() % 100;
			if (ram > 0)
			{
				P = 6;
			}
			//if (ram > 33)
			//{
			   // P = 0;
			//}
			if (ram > 66)
			{
				P = 8;
			}

		}
	}
	if (P == 8)
	{
		Vector3 newForward9 = m_patrolPos[8] - m_position;
		Vector3 distance9 = newForward9;
		newForward9.Normalize();
		m_forward = newForward9;
		Move();
		if (distance9.Length() <= 10.0f)
		{

			int ram = rand() % 100;
			if (ram < 50)
			{
				P = 7;
			}
			if (ram > 50)
			{
				P = 1;
			}

		}
	}

	//�Ώۂ�T��
	if (Search())
	{
		m_Neutral_EnemyState = enNeutral_Enemy_Chase;
	}
	ProcessCommonStateTransition();
}
void Neutral_Enemy::ManageState()
{
	switch (m_Neutral_EnemyState)
	{
		//�ҋ@�X�e�[�g
	case enNeutral_Enemy_Idle:
		ProcessIdleStateTransition();
		break;
		//�ǐՃX�e�[�g
	case enNeutral_Enemy_Chase:
		ProcessChaseStateTransition();
		break;
		//�U���X�e�[�g
	case enNeutral_Enemy_Attack:
		ProcessAttackStateTransition();
		break;
		//��_���[�W�X�e�[�g
	case enNeutral_Enemy_ReceiveDamage:
		ProcessReceiveDamageStateTransition();
		break;
		//���S�X�e�[�g
	case enNeutral_Enemy_Death:
		ProcessDeathStateTransition();
		break;
	case enNEutral_Enemy_Patrol:
		ProcessPatrolStateTransition();
		break;
	}
}

void Neutral_Enemy::PlayAnimation()
{
	m_modelRender.SetAnimationSpeed(1.5f);
	switch(m_Neutral_EnemyState)
	{
		//�ҋ@�X�e�[�g
	case enNeutral_Enemy_Idle:
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.5f);
		break;
		//�ǐՃX�e�[�g
	case enNeutral_Enemy_Chase:
		m_modelRender.PlayAnimation(enAnimationClip_Run, 0.5f);
		break;
		//�U���X�e�[�g
	case enNeutral_Enemy_Attack:
		m_modelRender.SetAnimationSpeed(1.0f);
		m_modelRender.PlayAnimation(enAnimationClip_Attack, 0.5f);
		break;
		//��_���[�W�X�e�[�g
	case enNeutral_Enemy_ReceiveDamage:
		m_modelRender.PlayAnimation(enNeutral_Enemy_ReceiveDamage, 0.5f);
		break;
		//���S�X�e�[�g
	case enNeutral_Enemy_Death:
		m_modelRender.PlayAnimation(enNeutral_Enemy_Death, 0.5f);
		break;
	case enNEutral_Enemy_Patrol:
		m_modelRender.PlayAnimation(enNEutral_Enemy_Patrol, 0.5f);
		break;
	}
}

//
void Neutral_Enemy::HPBar()
{
	if (m_Status.Hp < 0)
	{
		m_Status.Hp = 0;
	}

	Vector3 scale = Vector3::One;
	scale.x = float(m_Status.Hp) / float(m_Status.MaxHp);
	m_HPBar.SetScale(scale);

	Vector3 BerPosition = m_position;
	BerPosition.y += 75.0f;
	//��W��ϊ�����
	g_camera3D->CalcScreenPositionFromWorldPosition(m_HPBerPos, BerPosition);
	g_camera3D->CalcScreenPositionFromWorldPosition(m_HPWindowPos, BerPosition);
	g_camera3D->CalcScreenPositionFromWorldPosition(m_HPBackPos, BerPosition);

	//HP�o�[�摜��񂹂ɕ\������
	Vector3 BerSizeSubtraction = HPBerSend(HP_BER_SIZE, scale);	//�摜�̌��̑傫��
	m_HPBerPos.x -= BerSizeSubtraction.x;

	m_HPBar.SetPosition(Vector3(m_HPBerPos.x, m_HPBerPos.y, 0.0f));
	m_HPFrame.SetPosition(Vector3(m_HPWindowPos.x, m_HPWindowPos.y, 0.0f));
	m_HPBack.SetPosition(Vector3(m_HPBackPos.x, m_HPBackPos.y, 0.0f));

	m_HPBar.Update();
	m_HPFrame.Update();
	m_HPBack.Update();
}
Vector3 Neutral_Enemy::HPBerSend(Vector3 size, Vector3 scale)
{
	Vector3 hpBerSize = size;								//�摜�̌��̑傫��
	Vector3 changeBerSize = Vector3::Zero;					//�摜��X�P�[���ϊ��������Ƃ̑傫��
	Vector3 BerSizeSubtraction = Vector3::Zero;				//�摜�̌��ƕϊ���̍�

	changeBerSize.x = hpBerSize.x * scale.x;
	BerSizeSubtraction.x = hpBerSize.x - changeBerSize.x;
	BerSizeSubtraction.x /= 2.0f;

	return BerSizeSubtraction;
}
bool Neutral_Enemy::DrawHP()
{
	Vector3 toCameraTarget = g_camera3D->GetTarget() - g_camera3D->GetPosition();
	Vector3 toMush = m_position - g_camera3D->GetPosition();
	toCameraTarget.y = 0.0f;
	toMush.y = 0.0f;
	toCameraTarget.Normalize();
	toMush.Normalize();

	float cos = Dot(toCameraTarget, toMush);
	float angle = acos(cos);

	//�J�����̌��ɂ���Ȃ�`�悵�Ȃ�
	Vector3 diff = m_player->GetPosition() - m_position;

	//�v���C���[�Ɍ������v�Z����
	float playerdistance = diff.Length();

	if (fabsf(angle) < Math::DegToRad(45.0f)&& playerdistance<800.0f)
	{
		return true;
	}
	else
	{
		return false;
	}
}
void Neutral_Enemy::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	(void)clipName;
	//�L�[�̖��O��Attack_start�̎�
	if (wcscmp(eventName, L"Attack_start") == 0) {
		//�U�����̔����true�ɂ���
		m_UnderAttack = true;
		//�U���G�t�F�N�g�𔭐�������
		
		//�U���p�̃R���W������쐬
		MakeAttackCollision();

		//�傫����ݒ肷��B

		//��W�𒲐�

		//�G�t�F�N�g�Đ�


		//��ʉ��Đ�
		//�U���̐�
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(21);
		se->Play(false);
		se->SetVolume(0.5f);

		//��ʉ���Đ�����


	}
	//�L�[�̖��O��attack_end�̎�
	else if (wcscmp(eventName,L"Attack_end")==0){
		m_UnderAttack = false;

		//�X�e�[�g��؂�ւ���
		m_Neutral_EnemyState = enNeutral_Enemy_Chase;
	}
}

const bool Neutral_Enemy::CanAttack()const
{
	//�����̓G����v���C���[�Ɍ�x�N�g����v�Z����
	Vector3 diff = m_targetActor->GetPosition() - m_position;
	//�������߂������
	if (diff.LengthSq() <= 50.0f * 50.0f)
	{
		//�U���ł���
		return true;
	}
	//�U���ł��Ȃ�
	return false;
}

void Neutral_Enemy::Render(RenderContext& rc)
{
	//���f����`�悷��B
	m_modelRender.Draw(rc);

	//�X�e�[�g���|�[�Y�X�e�[�g�łȂ��Ȃ�
	if (m_Neutral_EnemyState != enNeutral_Enemy_Pause) {
		//�X�v���C�g�t���O��ture�Ȃ�
		if (m_player->GetSpriteFlag())
		{
			if (DrawHP())
			{
				m_HPBack.Draw(rc);
				m_HPBar.Draw(rc);
				m_HPFrame.Draw(rc);
			}
		}
	}
}