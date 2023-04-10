#include "stdafx.h"
#include "KnightAI.h"
#include "Game.h"
#include "KnightPlayer.h"
#include "Neutral_Enemy.h"
#include "CharUltFlag.h"
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
	charUltFlag = FindGO<CharUltFlag>("charUltFlag");
	//�X�t�B�A�R���C�_�[���������B
	m_sphereCollider.Create(1.0f);
	m_position = m_charCon.Execute(m_moveSpeed, 0.1f / 60.0f);
	
}
KnightAI::~KnightAI()
{

}
int KnightAI::CalculateTargetAI(Actor* actor)
{
	std::vector<Actor*>& actors = m_game->GetActors();
	for (auto actor : actors)
	{
		int actorlv = actor->GetLevel();
		if (actorlv >=3)
		{
			return 3;
		}
		else if (actorlv >=6)
		{
			return 2;
		}
		else
		{
			return 1;
		}
	}
	for (auto actor : actors)
	{
		int actorhp = actor->GetHP();
		if (m_Status.Hp-actorhp>=90)
		{
			return 2;
		}
		else if (m_Status.Hp - actorhp <= 60)
		{
			return 1;
		}
	}
}
void KnightAI::LotNextTargetAI()
{
	std::vector<Actor*>& actors = m_game->GetActors();
	std::vector<int> evals;
	for (auto actor : actors) {
		// �eActor�]���l���v�Z����
		int eval = CalculateTargetAI(actor);
		evals.push_back(eval);
	}
	// ��ԕ]���l������Actor��_��
	int targetEnemyNo = 0;
	int maxEval = 0;
	for (int i = 0; i < evals.size(); i++) {
		if (maxEval < evals[i]) {
			// �����̕����]���l������
			maxEval = evals[i];
			targetEnemyNo = i;
		}
	}
	AIget = true;
	AItargetPos = actors[targetEnemyNo]->GetPosition();
}
int KnightAI::CalculateTargetEnemy(Neutral_Enemy* enemy)
{
	if (m_Status.Hp - m_Status.MaxHp >= 50)
	{
		
	}
	return 0;
}
void KnightAI::LotNextTargetEnemy()
{
	std::vector<Neutral_Enemy*>& enemys = m_game->GetNeutral_Enemys();
	std::vector<int> evals;
	for (auto enemy : enemys) {
		// �e�G�l�~�[�̕]���l���v�Z����
		int eval = CalculateTargetEnemy(enemy);
		evals.push_back(eval);
	}
	// ��ԕ]���l�������G�l�~�[��_��
	int targetEnemyNo = 0;
	int maxEval = 0;
	for (int i = 0;  i < evals.size(); i++) {
		if (maxEval < evals[i]) {
			// �����̕����]���l������
			maxEval = evals[i];
			targetEnemyNo = i;
			
		}
	}
	Enemyget = true;
	EnemytargePos = enemys[targetEnemyNo]->GetPosition();
}
int KnightAI::CalculatAIAttackEvaluationValue()
{
	std::vector<Actor*>& actors = m_game->GetActors();
	for (auto actor : actors)
	{
		int actorlv = actor->GetLevel();
		int level = Lv - actorlv;
		if (level <= 1)
		{
			return 4;
		}
		else if (level <= 2)
		{
			return 3;
		}
		else
		{
			return 1;
		}
	}
}
int KnightAI::CalculatEnemyAttackEvaluationValue()
{
	//�G�l�~�[�����̏����擾����
	std::vector<Neutral_Enemy*>& enemys = m_game->GetNeutral_Enemys();
	Neutral_Enemy* m_neutral_enemy = nullptr;
	//��ԏ�����
	float minenemyDistance = INFINITY;
	for (auto enemy : enemys)
	{
			//�擾�����G�l�~�[�����̍��W���擾
			Vector3 enemyPos = enemy->GetPosition();
			//�����̍��W�����G�l�~�[�̍��W
			Vector3 diff = m_position - enemyPos;
			diff.y = 0.0f;
			float mindistance = diff.Length();

			if (mindistance < minenemyDistance)
			{
				minenemyDistance = mindistance;
				m_neutral_enemy = enemy;
			}
	}
	
	//�A�N�^�[�����̏����擾����
	std::vector<Actor*>& actors = m_game->GetActors();
	Actor* m_actor = nullptr;
	float minactorDistance = INFINITY;
	for (auto actor : actors)
	{
		if (actor == this) {
			//for���̈�ԍŏ��ɖ߂�
			continue;
		}
		//�擾�����G�l�~�[�����̍��W���擾
		Vector3 actorPos = actor->GetPosition();
		Vector3 diff = m_position - actorPos;
		float distance = diff.Length();
		if (distance < minactorDistance)
		{

			minactorDistance = distance;
			m_actor = actor;
		}

	}
	Vector3 diff = m_neutral_enemy->GetPosition() - m_position;
	Vector3 diff2 = m_actor->GetPosition() - m_position;
	float enemypos = diff.Length();
	float actorpos = diff2.Length();
	if (enemypos < actorpos)
	{
		return 3;
	}
}
void KnightAI::LotNextAction()
{
	// �`�h��_�����G��_�������肷��
	int aiAttackEvaluationValue = CalculatAIAttackEvaluationValue();
	int enemyAttackEvaluationValue = CalculatEnemyAttackEvaluationValue();
	if (aiAttackEvaluationValue > enemyAttackEvaluationValue) {
		// AI��_���]���l����������
		// �ǂ�AI��_�������I
		LotNextTargetAI();
	}
	else {
		// �G��_��
		LotNextTargetEnemy();
	}
}
void KnightAI::Update()
{
	// ���̃A�N�V�����𒊑I
	LotNextAction();
	SearchEnemy();
	Attack();
	//�X�e�[�g
	ManageState();
	//�A�j���[�V�����̍Đ�
	PlayAnimation();
	Collition();
	Rotation();
	m_position.y = m_position_YUp;
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetRotation(m_rot);
	m_modelRender.Update();
}
void KnightAI::SearchEnemy()
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
	if (AIget == true)
	{
		Vector3 diff = AItargetPos - m_position;
		Vector3 diff2 = diff * -1;
		
	
		//�ړ����x��ݒ肷��B
		if (diff2.Length() < 45.0f) {
			diff2.Normalize();
			
			m_moveSpeed = diff2 * m_Status.Speed;
			m_position = m_charCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
		}
		if (diff.Length() > 40.0f)
		{
			diff.Normalize();
			m_moveSpeed = diff * m_Status.Speed;
			m_position = m_charCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
		}
	}
	if(Enemyget == true)
	{
		Vector3 diff = EnemytargePos - m_position;
		diff.Normalize();
		//�ړ����x��ݒ肷��B
		m_moveSpeed = diff * m_Status.Speed;
		m_position = m_charCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	}
}
const bool KnightAI::CanAttackenemy()
{
	//�G�l�~�[�����̏����擾����
	std::vector<Neutral_Enemy*>& enemys = m_game->GetNeutral_Enemys();

	for (auto Enemys : enemys)
	{
		//�擾�����G�l�~�[�����̍��W���擾
		Vector3 enemyPos = Enemys->GetPosition();
		//�擾�������W�������̂ƈ���
		Vector3 diff = enemyPos - m_position;

		if (diff.LengthSq() <= 70.0f * 70.0f)
		{
			m_targetEnemy = Enemys;

			return true;
		}
	}
	m_targetEnemy = nullptr;
	Enemyget = false;
	return false;
}
const bool KnightAI::CanAttackActor()
{
	//�A�N�^�[�����̏����擾����
	std::vector<Actor*>& actors = m_game->GetActors();
	for (auto Actors : actors)
	{
		if (Actors == this) {
			//for���̈�ԍŏ��ɖ߂�
			continue;
		}

		Vector3 actorPos = Actors->GetPosition();
		Vector3 diff = actorPos - m_position;

		if (diff.LengthSq() <= 70.0f * 70.0f)
		{
			m_targetActor = Actors;
			return true;
		}
	}
	m_targetActor = nullptr;
	AIget = false;
	return false;
}
void KnightAI::Attack()
{
	
	if (CanAttackenemy() || CanAttackActor()) {
		//�_������ς���
		Vector3 targetPos = TargetChange();
		//�A�łōU���ł��Ȃ��Ȃ�

		//��i�ڂ̃A�^�b�N�����Ă��Ȃ��Ȃ�
		if (AtkState == false)
		{
			Vector3 diff = targetPos - m_position;
			m_rot.SetRotationYFromDirectionXZ(diff);
			m_knightState = enKnightState_ChainAtk;

			//FirstAtkFlag = true;
			//�R���{��1���₷
			//ComboState++;

			AtkState = true;
		}
		//��i�ڂ̃A�^�b�N�̃A�j���[�V�������X�^�[�g�����Ȃ�
		if (m_AtkTmingState == FirstAtk_State)
		{
			Vector3 diff = targetPos - m_position;
			m_rot.SetRotationYFromDirectionXZ(diff);
			//�X�e�[�g���i�ڂ̃A�^�b�N�̃A�j���[�V�����X�^�[�g�X�e�[�g�ɂ���
			m_AtkTmingState = SecondAtk_State;

		}

		if (m_AtkTmingState == SecondAtkStart_State)
		{
			Vector3 diff = targetPos - m_position;
			m_rot.SetRotationYFromDirectionXZ(diff);
			//�X�e�[�g���O�i�ڂ̃A�^�b�N�̃A�j���[�V�����X�^�[�g�X�e�[�g�ɂ���
			m_AtkTmingState = LastAtk_State;

		}

	}
	//�X�L���𔭓����鏈��
	//B�{�^���������ꂽ��
	if (pushFlag == false && SkillEndFlag == false && SkillState == false )
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
	if (pushFlag == false && Lv >= 4&& UltFlug==true)
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
			m_knightState = enKnightState_Idle;
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
			m_knightState = enKnightState_Idle;
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
		m_knightState = enKnightState_Idle;
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
void KnightAI::AvoidanceSprite()
{

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

const Vector3 KnightAI::TargetChange()
{
	if (m_targetEnemy == nullptr && m_targetActor == nullptr) {
		abort();	//�ĂԂ�
	}

	if (m_targetEnemy != nullptr && m_targetActor == nullptr) {
		//�G�l�~�[�̍��W��n��
		return m_targetEnemy->GetPosition();
	}
	if (m_targetActor != nullptr && m_targetEnemy == nullptr) {
		//�A�N�^�[�̍��W��n��
		UltFlug = true;
		return m_targetActor->GetPosition();
	}
	//�����̓G�ƃA�N�^�[�N���X�̓G��������ꍇ�́A
	//���A�N�^�[�N���X�̓G��������背�x���Ⴂ�������̓G��|���ɍs��
	if (m_targetActor->GetLevel() > Lv) {
		return m_targetEnemy->GetPosition();
	}

	return m_targetActor->GetPosition();

}
void KnightAI::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);

}

