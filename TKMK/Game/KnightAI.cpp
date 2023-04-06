#include "stdafx.h"
#include "KnightAI.h"
#include "Game.h"
#include "Neutral_Enemy.h"
#include "Actor.h"
#include "KnightBase.h"

KnightAI::KnightAI()
{
	//���m�̃X�e�[�^�X
	m_Status.Init("Knight");
	SetModel();
	//�A�j���[�V�����C�x���g�p�̊֐���ݒ肷��B
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});
	//���X�|�[��������W0�Ԃ̎擾
	GetRespawnPos();
	respawnNumber = 1;        //���X�|�[��������W�̔ԍ�
	m_respawnPos[respawnNumber].y;
	//���X�|�[��������W�̃Z�b�g
	//�L�����R��
	m_charCon.SetPosition(m_respawnPos[respawnNumber]);
	//���m
	m_modelRender.SetPosition(m_respawnPos[respawnNumber]);
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
void KnightAI::Collition()
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