#include "stdafx.h"
#include "KnightAI.h"
#include "Game.h"
#include "KnightPlayer.h"
#include "Neutral_Enemy.h"
#include "Actor.h"


KnightAI::KnightAI()
{
	m_Status.Init("Knight");
	SetModel();
	//�A�j���[�V�����C�x���g�p�̊֐���ݒ肷��B
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});
	//���X�|�[��������W0�Ԃ̎擾
	GetRespawnPos();
	respawnNumber = 1;        //���X�|�[��������W�̔ԍ�
	m_respawnPos[respawnNumber].y /*+= m_position_YUp*/;
	//���X�|�[��������W�̃Z�b�g
	//�L�����R��
	m_charCon.SetPosition(m_respawnPos[respawnNumber]);
	//���m
	m_modelRender.SetPosition(m_respawnPos[respawnNumber]);
	m_knightPlayer = FindGO<KnightPlayer>("m_knightplayer");
	m_neutral_Enemys = FindGOs<Neutral_Enemy>("Neutral_Enemy");
	//�X�t�B�A�R���C�_�[���������B
	m_sphereCollider.Create(1.0f);

	m_knightAIPoslevel.Init("Assets/level3D/knightAIPos.tkl", [&](LevelObjectData& objData) {

		if (objData.ForwardMatchName(L"Pos") == true) {
			SetPatrolPos(objData.position, objData.number);
			return true;
		}
	});
}
KnightAI::~KnightAI()
{

}
void KnightAI::Move()
{
	Vector3 diff = m_forward;
	diff.Normalize();
	////�ړ����x��ݒ肷��B
	m_moveSpeed = diff * m_Status.Speed;
	m_forward.Normalize();
	Vector3 moveSpeed = m_forward * m_Status.Speed;
	m_position = m_charCon.Execute(moveSpeed, g_gameTime->GetFrameDeltaTime());
	m_modelRender.SetPosition(m_position);

}
void KnightAI::Update()
{
	
	Patrol();
	SearchEnemy();
	ChaseEnemy();
	Attack();
	//�X�e�[�g
	ManageState();
	//�A�j���[�V�����̍Đ�
	PlayAnimation();
	Collition();
	Rotation();
	m_position = m_charCon.Execute(m_moveSpeed, 0.1f / 60.0f);

	//���m��Y���W�����Ȃ̂�Y���W���グ��
	m_position.y = m_position_YUp;
	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();
}

/// <summary>
/// �U�����̓����蔻��̏���
/// </summary>
void KnightAI::AtkCollisiton()
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
void KnightAI::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//��i�ڂ̃A�^�b�N�̃A�j���[�V�������n�܂�����
	if (wcscmp(eventName, L"FirstAttack_Start") == 0)
	{
		m_AtkTmingState = FirstAtk_State;
		//���̃R���W�����𐶐�
		AtkCollistionFlag = true;
		//���P�i�ډ�
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(13);
		se->Play(false);
		se->SetVolume(0.3f);
	}
	//��i�ڂ̃A�^�b�N�̃A�j���[�V�������n�܂�����
	if (wcscmp(eventName, L"SecondAttack_Start") == 0)
	{
		m_AtkTmingState = SecondAtkStart_State;
		//���̃R���W�����𐶐�
		AtkCollistionFlag = true;
		//���Q�i�ډ�
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(14);
		se->Play(false);
		se->SetVolume(0.3f);
	}
	//�O�i�ڂ̃A�^�b�N�̃A�j���[�V�������n�܂�����
	if (wcscmp(eventName, L"LastAttack_Start") == 0)
	{
		m_AtkTmingState = LastAtk_State;
		//���̃R���W�����𐶐�
		AtkCollistionFlag = true;
		//���R�i�ډ�
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(15);
		se->Play(false);
		se->SetVolume(0.3f);
	}
	//�X�L���̃A�j���[�V�������n�܂�����
	if (wcscmp(eventName, L"SkillAttack_Start") == 0)
	{
		m_Status.Atk += 20;
		//m_AtkTmingState = LastAtk_State;
		//���̃R���W�����𐶐�
		AtkCollistionFlag = true;

		//�X�L�����𔭐�
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(11);
		se->Play(false);
		se->SetVolume(0.3f);
	}
	//////////////////////////////////////////////////////////////////////////
	//��i�ڂ̃A�^�b�N�̃A�j���[�V�����Ō���U��I�������
	if (wcscmp(eventName, L"FirstAttack_End") == 0)
	{

		//���̃R���W�����𐶐����Ȃ�
		AtkCollistionFlag = false;
	}
	///��i�ڂ̃A�^�b�N�̃A�j���[�V�������I�������
	if (wcscmp(eventName, L"FirstToIdle") == 0)
	{
		//�{�^����������Ă��Ȃ�������
		if (m_AtkTmingState != SecondAtk_State)
		{
			//�{�^���v�b�V���t���O��false�ɂ���
			pushFlag = false;
			AtkState = false;
			m_playerState = enKnightState_Idle;
			m_AtkTmingState = Num_State;
		}
	}

	//��i�ڂ̃A�^�b�N�̃A�j���[�V�����Ō���U��I�������
	if (wcscmp(eventName, L"SecondAttack_End") == 0)
	{

		//���̃R���W�����𐶐����Ȃ�
		AtkCollistionFlag = false;
		//�{�^����������Ă��Ȃ�������
		if (m_AtkTmingState != LastAtk_State)
		{
			//�{�^���v�b�V���t���O��false�ɂ���
			pushFlag = false;
			AtkState = false;
			m_playerState = enKnightState_Idle;
			m_AtkTmingState = Num_State;
		}
	}
	//�O�i�ڂ̃A�^�b�N�̃A�j���[�V�����Ō���U��I�������
	if (wcscmp(eventName, L"LastAttack_End") == 0)
	{
		m_AtkTmingState = Num_State;
		AtkState = false;
		//���̃R���W�����𐶐����Ȃ�
		AtkCollistionFlag = false;
	}
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false) {
		m_playerState = enKnightState_Idle;
		AtkState = false;
		//�{�^���v�b�V���t���O��false�ɂ���
		pushFlag = false;
	}

	//�X�L���̃A�j���[�V�����Ō���U��I�������
	if (wcscmp(eventName, L"SkillAttack_End") == 0)
	{
		m_Status.Atk -= 20;
		m_AtkTmingState = Num_State;
		AtkState = false;
		//�X�L���̈ړ����������Ȃ��悤�ɂ���
		SkillState = false;
		m_Status.Speed -= 120.0f;
		//���̃R���W�����𐶐����Ȃ�
		AtkCollistionFlag = false;
	}
	//����A�j���[�V�������I�������
	if (wcscmp(eventName, L"Avoidance_End") == 0)
	{
		//�ړ����������Ȃ��悤�ɂ���

		AvoidanceFlag = false;
		//m_AtkTmingState = Num_State;

	}
}
void KnightAI::Collition()
{
	//��_���[�W�A�_�E�����A�K�E�Z�A�ʏ�U�����̓_���[�W��������Ȃ��B
	if (m_playerState == enKnightState_Damege ||
		m_playerState == enKnightState_Death ||
		m_playerState == enKnightState_UltimateSkill ||
		m_playerState == enKnightState_ChainAtk ||
		m_playerState == enKnightState_Skill ||
		m_playerState == enKnightState_Avoidance)
	{
		return;
	}
	else
	{
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
				Dameged(Enemy_atk);

			}
		}
	}

}
void KnightAI::AvoidanceSprite()
{

}
void KnightAI::LevelMove()
{
	if (Lv >= 1 || Lv <= 3)
	{
		if (m_isSearchEnemy == true)
		{
			ChaseEnemy();
			if (m_SearchPlayer_OR_AI == true)
			{

				m_isSearchEnemy = false;
				ChasePlayer_OR_AI();
			}
		}
	}
}
void KnightAI::ChasePlayer_OR_AI()
{

}
void KnightAI::ChaseEnemy()
{
	if (m_isSearchEnemy == true)
	{
		PL = false;
			//�G�l�~�[�̍��W���擾
			Vector3 AIpos = m_Neutral_Enemy->GetPosition();
			Vector3 diff = AIpos - m_position;
			diff.Normalize();
			m_moveSpeed = diff * m_Status.Speed;
			m_position = m_charCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
			if (m_charCon.IsOnGround()) {
				//�n�ʂɂ����B
				m_moveSpeed.y = 0.0f;
			}
			Vector3 modelPosition = m_position;
			//������Ƃ������f���̍��W��������B
			modelPosition.y += 2.5f;
			m_modelRender.SetPosition(modelPosition);
		
	}
	else
	{

	}
	//Vector3 diff = m_Neutral_Enemy->GetPosition() - m_position;
	//nearPos = Vector3::Zero;
	//for (int i = 0; i <= enemyAmount; i++)
	//{
	//	if (nearPos.Length() > m_enemyPositions.AIpos[i].Length())
	//	{
	//		nearPos = m_enemyPositions.AIpos[i];
	//	}
	//}

	//nearPos.Normalize();
	////�ړ����x��ݒ肷��B
	//m_moveSpeed = nearPos * m_Status.Speed;
	//m_position = m_charCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	//if (m_charCon.IsOnGround()) {
	//	//�n�ʂɂ����B
	//	m_moveSpeed.y = 0.0f;
	//}
	//Vector3 modelPosition = m_position;
	////������Ƃ������f���̍��W��������B
	//modelPosition.y += 2.5f;
	//m_modelRender.SetPosition(modelPosition);
}
void KnightAI::Rotation()
{
	if (fabsf(m_moveSpeed.x) < 0.001f
		&& fabsf(m_moveSpeed.z) < 0.001f) {
		//m_moveSpeed.x��m_moveSpeed.z�̐�Βl���Ƃ���0.001�ȉ��Ƃ������Ƃ�
		//���̃t���[���ł̓L�����͈ړ����Ă��Ȃ��̂Ő��񂷂�K�v�͂Ȃ��B
		return;
	}
	//atan2��tan�Ƃ̒l���p�x(���W�A���P��)�ɕϊ����Ă����֐��B
	//m_moveSpeed.x / m_moveSpeed.z�̌��ʂ�tan�ƂɂȂ�B
	//atan2���g�p���āA�p�x�����߂Ă���B
	//���ꂪ��]�p�x�ɂȂ�B
	float angle = atan2(-m_moveSpeed.x, m_moveSpeed.z);
	//atan���Ԃ��Ă���p�x�̓��W�A���P�ʂȂ̂�
	//SetRotationDeg�ł͂Ȃ�SetRotation���g�p����B
	m_rot.SetRotationY(-angle);

	//��]��ݒ肷��B
	m_modelRender.SetRotation(m_rot);

	//�v���C���[�̑O�x�N�g�����v�Z����B
	m_forward = Vector3::AxisZ;
	m_rot.Apply(m_forward);
}

//�Փ˂����Ƃ��ɌĂ΂��֐��I�u�W�F�N�g(�Ǘp)
struct SweepResultWall :public btCollisionWorld::ConvexResultCallback
{
	bool isHit = false;						//�Փ˃t���O�B

	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		//�ǂƂԂ����ĂȂ�������B
		if (convexResult.m_hitCollisionObject->getUserIndex() != enCollisionAttr_Wall) {
			//�Փ˂����͕̂ǂł͂Ȃ��B
			return 0.0f;
		}

		//�ǂƂԂ�������B
		//�t���O��true�ɁB
		isHit = true;
		return 0.0f;
	}
};
void KnightAI::SearchEnemy()
{
	for (auto enemy : m_neutral_Enemys)
	{		
		//�G�l�~�[�̍��W���擾
		Vector3 AIpos = enemy->GetPosition();
		Vector3 diff = AIpos - m_position;
		diff.Normalize();
		float angle = acosf(diff.Dot(m_forward));
		//�v���C���[�����E���ɋ��Ȃ�������B
		if (Math::PI * 0.1f <= fabsf(angle))
		{
			m_isSearchEnemy = true;
			m_Neutral_Enemy = enemy;

			return;
		}
		
	}

	//m_forward = Vector3::AxisZ;
	//m_rotation.Apply(m_forward);
	//int i = 0;
	//enemyAmount = 0;
	//for (auto enemy : m_neutral_Enemys)
	//{
	//	//�G�l�~�[�̍��W���擾
	//	Vector3 AIpos = enemy->GetPosition();
	//	m_enemyPositions.AIpos[i] = enemy->GetPosition();
	//	Vector3 diff = m_enemyPositions.AIpos[i] - m_position;
	//	if (diff.Length() >= 10000.0f)
	//	{
	//		m_enemyPositions.foundFlag[i] = false;
	//		//for���ŏ��ɖ߂�B
	//		continue;
	//	}

	//	diff.Normalize();
	//	float angle = acosf(diff.Dot(m_forward));
	//	//�v���C���[�����E���ɋ��Ȃ�������B
	//	if (Math::PI * 0.35f <= fabsf(angle))
	//	{
	//		//�v���C���[�͌������Ă��Ȃ��B
	//		m_enemyPositions.foundFlag[i] = false;
	//		//return;
	//	}
	//	else {
	//		//���E���Ȃ�
	//		m_enemyPositions.foundFlag[i] = true;
	//		//PL = false;
	//	}
	//	enemyAmount++;
	//	i++;

		//}
		//btTransform start, end;
		//start.setIdentity();
		//end.setIdentity();
		////�n�_�̓G�l�~�[�̍��W�B
		//start.setOrigin(btVector3(m_position.x, m_position.y + 70.0f, m_position.z));
		////�I�_�̓v���C���[�̍��W�B
		//end.setOrigin(btVector3(AIPos.x, AIPos.y + 70.0f, AIPos.z));

		//SweepResultWall callback;
		////�R���C�_�[���n�_����I�_�܂œ������āB
		////�Փ˂��邩�ǂ����𒲂ׂ�B
		//PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);
		////�ǂƏՓ˂����I
		//if (callback.isHit == true)
		//{
		//	//�v���C���[�͌������Ă��Ȃ��B
		//	return;
		//}

		//�ǂƏՓ˂��ĂȂ��I�I
		//�v���C���[�������t���O��true�ɁB
		
	
}

void KnightAI::Patrol()
{
	if (PL == true)
	{
		if (P < 0 || P > 4)
		{
			Vector3 newForward = m_patrolPos[0] - m_position;
			Vector3 distance = newForward;
			newForward.Normalize();
			m_forward = newForward;
			Move();
			if (distance.Length() <= 100.0f)
			{
				P = 0;
			}
		}
		if (P == 0)
		{
			Vector3 newForward = m_patrolPos[0] - m_position;
			Vector3 distance = newForward;
			newForward.Normalize();
			m_forward = newForward;
			Move();
			if (distance.Length() <= 100.0f)
			{

				//1����ɂ���������+�P����
				int ram = rand() % 100 /*+ 1*/;
				if (ram >= 0)
				{
					P = 1;
				}
				if (ram > 25)
				{
					P = 2;
				}
				if (ram > 50)
				{
					P = 3;
				}
				if (ram > 75)
				{
					P = 4;
				}
			}

		}
		if (P == 1)
		{
			//position1�Ɍ������R�[�h
			//������position1�ɓ���������P=1;
			//patrol=true;
			Vector3 newForward = m_patrolPos[1] - m_position;
			Vector3 distance = newForward;
			newForward.Normalize();
			m_forward = newForward;
			Move();
			if (distance.Length() <= 100.0f)
			{

				//1����ɂ���������+�P����
				int ram = rand() % 100 /*+ 1*/;
				if (ram < 50)
				{
					P = 2;
				}
				if (ram > 50)
				{
					P = 4;
				}
			}

		}
		if (P == 2)
		{
			//position1�Ɍ������R�[�h
			//������position1�ɓ���������P=1;
			//patrol=true;
			Vector3 newForward = m_patrolPos[2] - m_position;
			Vector3 distance = newForward;
			newForward.Normalize();
			m_forward = newForward;
			Move();
			if (distance.Length() <= 100.0f)
			{

				//1����ɂ���������+�P����
				int ram = rand() % 100 /*+ 1*/;
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
			//position1�Ɍ������R�[�h
			//������position1�ɓ���������P=1;
			//patrol=true;
			Vector3 newForward = m_patrolPos[3] - m_position;
			Vector3 distance = newForward;
			newForward.Normalize();
			m_forward = newForward;
			Move();
			if (distance.Length() <= 100.0f)
			{

				//1����ɂ���������+�P����
				int ram = rand() % 100 /*+ 1*/;
				if (ram < 50)
				{
					P = 2;
				}
				if (ram > 50)
				{
					P = 4;
				}
			}

		}
		if (P == 4)
		{
			//position1�Ɍ������R�[�h
			//������position1�ɓ���������P=1;
			//patrol=true;
			Vector3 newForward = m_patrolPos[4] - m_position;
			Vector3 distance = newForward;
			newForward.Normalize();
			m_forward = newForward;
			Move();
			if (distance.Length() <= 100.0f)
			{

				//1����ɂ���������+�P����
				int ram = rand() % 100 /*+ 1*/;
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
	}
	else
	{
		ChaseEnemy();
	}
}

const bool KnightAI ::CanAttack()const
{


		//�G�l�~�[�̍��W���擾
		Vector3 AIpos = m_Neutral_Enemy->GetPosition();
		Vector3 diff = AIpos - m_position;

		if (diff.LengthSq() <= 50.0f * 50.0f)
		{
			//�U���ł���
			return true;
		}
		//�U���ł��Ȃ�
		return false;
	
		
	////�����̓G����v���C���[�Ɍ������x�N�g�����v�Z����
	//Vector3 diff = nearPos - m_position;
	////�������߂�������
	//if (diff.LengthSq() <= 50.0f * 50.0f)
	//{
	//	//�U���ł���
	//	return true;
	//}
	////�U���ł��Ȃ�
	//return false;
}
void KnightAI::Attack()
{
	if (CanAttack() == true) {
		//�A�łōU���ł��Ȃ��Ȃ�

		//��i�ڂ̃A�^�b�N�����Ă��Ȃ��Ȃ�
		if ( AtkState == false)
		{
		
			m_playerState = enKnightState_ChainAtk;

			//FirstAtkFlag = true;
			//�R���{��1���₷
			//ComboState++;
			
			AtkState = true;
		}
		//��i�ڂ̃A�^�b�N�̃A�j���[�V�������X�^�[�g�����Ȃ�
		if (m_AtkTmingState == FirstAtk_State)
		{

			//�X�e�[�g���i�ڂ̃A�^�b�N�̃A�j���[�V�����X�^�[�g�X�e�[�g�ɂ���
			m_AtkTmingState = SecondAtk_State;

		}

		if (m_AtkTmingState == SecondAtkStart_State)
		{

			//�X�e�[�g���O�i�ڂ̃A�^�b�N�̃A�j���[�V�����X�^�[�g�X�e�[�g�ɂ���
			m_AtkTmingState = LastAtk_State;

		}

	}
		//�X�L���𔭓����鏈��
		//B�{�^���������ꂽ��
		if (pushFlag == false && SkillEndFlag == false && SkillState == false && g_pad[0]->IsTrigger(enButtonB))
		{

			//�ړ����x���グ��
			m_Status.Speed += 120.0f;

			/*AnimationMove(SkillSpeed);*/
			pushFlag = true;
			SkillState = true;
			//AtkCollistionFlag = true;
		}

		//�K�E�Z�𔭓����鏈��
		//X�{�^���������ꂽ��
		if (pushFlag == false && Lv >= 4 && g_pad[0]->IsTrigger(enButtonX))
		{
			pushFlag = true;
			//�A�j���[�V�����Đ��A���x�����R
			UltimateSkill();



			//�A���e�B���b�gSE
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(16);
			se->Play(false);
			se->SetVolume(0.3f);

			//�K�E�Z�����t���O���Z�b�g
			UltimateSkillFlag = true;
		}

		//�K�E�Z�����t���O���Z�b�g����Ă���Ȃ�
		if (UltimateSkillFlag == true)
		{
			UltimateSkillTimer += g_gameTime->GetFrameDeltaTime();
			//�K�E�Z�^�C�}�[��3.0f�܂ł̊�
			if (UltimateSkillTimer <= 3.0f)
			{
				//�R���W�����̍쐬�A�ړ�����
				UltimateSkillCollistion(OldPosition, m_position);
			}
			else
			{
				//�U�����L���Ȏ��Ԃ����Z�b�g
				UltimateSkillTimer = 0;
				//�K�E�Z�����t���O�����Z�b�g
				UltimateSkillFlag = false;
				//�R���W�����폜
				DeleteGO(collisionObject);
				//�R���W�����쐬�t���O�����Z�b�g
				UltCollisionSetFlag = false;
			}
		}

		//�U�����X�L�����g�p���Ă���Ȃ�
		//�R���W�����쐬
		if (AtkCollistionFlag == true) AtkCollisiton();
	
}

void KnightAI::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);

}

