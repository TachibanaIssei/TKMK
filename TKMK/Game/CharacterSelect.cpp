#include "stdafx.h"
#include "CharacterSelect.h"
#include "Tittle.h"
#include "Game.h"
#include "Fade.h"

namespace {
	const Vector3 StatusPos = Vector3(400.0f, 250.0f, 0.0f);			//�X�e�[�^�X�̍��W
	const Vector3 Attack_IconPos = Vector3(400.0f, -150.0f, 0.0f);		//�U���A�X�L���A�K�E�Z�A�C�R���̍��W
	const Vector3 NamePos = Vector3(-600.0f, -400.0f, 0.0f);				//���O�̍��W
	const Vector3 UnderBarPos = Vector3(0.0f, -500.0f, 0.0f);			//��ʉ��̃o�[�̍��W

	const Vector3 Attack_explanationPos = Vector3(20.0f, 33.0f, 0.0f);
	const Vector3 Skill_explanationPos = Vector3(105.0f, 33.0f, 0.0f);
	const Vector3 Ult_explanationPos = Vector3(40.0f, 30.0f, 0.0f);

	const Vector3 KnightPos = Vector3(-150.0f, 0.0f, 0.0f);				//���m�̍��W
	const Vector3 PlatformPos = Vector3(-150.0f, -40.0f, 0.0f);				//���m�̍��W
	const Quaternion KnightRot = Quaternion(0.0f, 90.0f, 0.0f, 1.0f);

	const Vector3 CameraTargetPos = Vector3(0.0f, 90.0f, -200.0f);			//�J�����̃^�[�Q�b�g
	const Vector3 m_CameraPosition = Vector3( 0.0f, 90.0f, -248.0f );   //�J�����̍��W

	const Vector3 AttackCollision = Vector3(20.0f, 33.0f, -0.0f);
	const Quaternion AttackCollisionRot = Quaternion(180.0f, 75.0f, 0.0f, 1.0f);

	const Vector3 SkillCollision = Vector3(105.0f, 33.0f, 0.0f);

	const Vector3 UltimateCollision = Vector3(194.0f, 30.0f, 0.0f);

	const float PointerSpeed = 400.0f;

}

CharacterSelect::CharacterSelect()
{

}

CharacterSelect::~CharacterSelect()
{
	//�U���A�C�R���̓����蔻��
	Attack.Release();
	//�X�L���A�C�R���̓����蔻��
	Skill.Release();
	//�K�E�Z�A�C�R���̓����蔻��
	UltimateSkill.Release();
}

bool CharacterSelect::Start()
{
	fade = FindGO<Fade>("fade");

	//�J�����̍��W��ݒ�
	Quaternion rot;
	//rot.Apply(m_CameraPosition);
	g_camera3D->SetTarget(CameraTargetPos);
	g_camera3D->SetPosition(m_CameraPosition);
	g_camera3D->Update();

	g_renderingEngine->SetAmbient(Vector3(0.5f, 0.5f, 0.5f));
	Vector3 dir = { 0.0f,-1.0f,0.0f };
	Vector3 color = { 0.5f,0.5f,0.5f };
	g_renderingEngine->SetDirectionLight(0, dir, color);

	//���m�̃��f���A�A�j���[�V����
	SetKnightModel();
	//�U���A�X�L���A�K�E�Z�A�C�R���̓����蔻��
	SetCollision();

	//�|�C���^�[�̍�
	m_pointer_black.Init("Assets/sprite/Select/pointer_black.DDS", 220.0f, 220.0f);
	m_pointer_black.SetPosition(m_Pointerposition);
	m_pointer_black.SetScale(0.6f, 0.6f, 0.6f);
	m_pointer_black.Update();

	//�|�C���^�[�̔�
	m_pointer_white.Init("Assets/sprite/Select/pointer_white.DDS", 220.0f, 220.0f);
	m_pointer_white.SetPosition(m_Pointerposition);
	m_pointer_white.SetScale(0.6f, 0.6f, 0.6f);
	m_pointer_white.Update();

	//��̐ݒ�
	m_platform.Init("Assets/modelData/platform/platform.tkm");
	m_platform.SetPosition(PlatformPos);
	m_platform.SetScale(2.2f, 2.0f, 2.2f);
	m_platform.Update();

	//�J�[�\��
	m_SelectCursor.Init("Assets/sprite/SelectCurSor.DDS", 1200.0f, 675.0f);
	m_SelectCursor.SetPosition(curPosition);
	m_SelectCursor.SetScale(1.0f, 1.0f, 1.0f);
	m_SelectCursor.Update();

	//////////////////////////////////////////////////////////////////////////////////////
	//�X�e�[�^�X
	m_Status.Init("Assets/sprite/Select/Status.DDS", 950.0f, 350.0f);
	m_Status.SetPosition(StatusPos);
	m_Status.SetScale(1.0f, 1.0f, 1.0f);
	m_Status.Update();

	//�U���A�X�L���A�K�E�Z�A�C�R��
	m_Attack_Icon.Init("Assets/sprite/Select/Attack_Icon.DDS", 1000.0f, 450.0f);
	m_Attack_Icon.SetPosition(Attack_IconPos);
	m_Attack_Icon.SetScale(1.0f, 1.0f, 1.0f);
	m_Attack_Icon.Update();

	//���O
	m_name.Init("Assets/sprite/Select/name.DDS", 550.0f, 200.0f);
	m_name.SetPosition(NamePos);
	m_name.SetScale(1.0f, 1.0f, 1.0f);
	m_name.Update();

	//��ʉ��̃o�[
	m_UnderBar.Init("Assets/sprite/Select/underBar.DDS", 1920.0f, 300.0f);
	m_UnderBar.SetPosition(UnderBarPos);
	m_UnderBar.SetScale(1.0f, 1.0f, 1.0f);
	m_UnderBar.Update();

	//������
	{
		//�U���̐�����
		m_Attack_explanation.Init("Assets/sprite/Select/Attack_explanation.DDS", 1120.0f, 300.0f);
		m_Attack_explanation.SetPosition(Attack_explanationPos);
		m_Attack_explanation.SetScale(1.0f, 1.0f, 1.0f);
		m_Attack_explanation.Update();

		//�X�L���̐�����
		Skill_explanation.Init("Assets/sprite/Select/Skill_explanation.DDS", 1120.0f, 450.0f);
		Skill_explanation.SetPosition(Skill_explanationPos);
		Skill_explanation.SetScale(1.0f, 1.0f, 1.0f);
		Skill_explanation.Update();

		//�K�E�Z�̐�����
		Ult_explanation.Init("Assets/sprite/Select/Ult_explanation.DDS", 1700.0f, 501.0f);
		Ult_explanation.SetPosition(Ult_explanationPos);
		Ult_explanation.SetScale(1.0f, 1.0f, 1.0f);
		Ult_explanation.Update();
	}

	Poimter.Init(
		15.0f,
		1.0f,
		m_Pointerposition
	);

	return true;
}

void CharacterSelect::Update()
{
	//�|�C���^�[�̓_��
	time += g_gameTime->GetFrameDeltaTime();
	/*time++;
	if (time >= 61)
	{
		time = 0;
	}*/

	if (m_readyFlag == true)
	{
		Ready();
	}
	else
	{
		//�J�[�\��
		//Cursor();

		PointerMove();

		GhostCollision();


		//A�{�^������������
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			//�t�F�[�h�A�E�g���n�߂�
			fade->StartFadeIn(1.0f);

			m_readyFlag = true;
		}

		//B�{�^���������ꂽ��^�C�g���ɖ߂�
		if (g_pad[0]->IsTrigger(enButtonB))
		{
			Tittle* tittle = NewGO<Tittle>(0, "tittle");
			DeleteGO(this);
		}
	}

	////�J�[�\��
	////Cursor();

	//PointerMove();
	//
	//GhostCollision();

	//
	////A�{�^������������
	//if (g_pad[0]->IsTrigger(enButtonA))
	//{
	//	//�t�F�[�h�A�E�g���n�߂�
	//	fade->StartFadeOut(1.0f);

	//	m_readyFlag = true;
	//}

	////B�{�^���������ꂽ��^�C�g���ɖ߂�
	//if (g_pad[0]->IsTrigger(enButtonB))
	//{
	//	Tittle* tittle = NewGO<Tittle>(0, "tittle");
	//	DeleteGO(this);
	//}

	m_SelectCursor.Update();

	//���m����]������
	m_KnightRot.AddRotationDegY(2.0f);
	m_Knight.SetRotation(m_KnightRot);
	m_Knight.Update();

	m_platform.SetRotation(m_KnightRot);
	m_platform.Update();

	//�����蔻��̉���
	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
}

/// <summary>
/// �|�C���^�[�̈ړ�����
/// </summary>
void CharacterSelect::PointerMove()
{
	m_moveSpeed = Vector3::Zero;
	//�ړ�����
	Vector3 stickL;
	stickL.x = PointerSpeed * g_pad[0]->GetLStickXF();
	stickL.y = PointerSpeed * g_pad[0]->GetLStickYF();

	m_moveSpeed += stickL;
	
	

	m_Pointerposition = Poimter.Execute(m_moveSpeed, 1.0f/60.0f);
	Poimter.SetPosition(m_Pointerposition);

	if (m_Pointerposition.x > 1000.0f) {
		m_Pointerposition.x = 1000.0f;
	}
	if (m_Pointerposition.x < -1000.0f) {
		m_Pointerposition.x = -1000.0f;
	}

	if (m_Pointerposition.y > 500.0f) {
		m_Pointerposition.y = 500.0f;
	}
	if (m_Pointerposition.y < -500.0f) {
		m_Pointerposition.y = -500.0f;
	}
	//m_Pointerposition.z = 0.0f;
	//�摜�̍��W���ǂ����Ȃ�
	m_pointer_black.SetPosition(m_Pointerposition);
	m_pointer_white.SetPosition(m_Pointerposition);

	m_pointer_black.Update();
	m_pointer_white.Update();



}

void CharacterSelect::GhostCollision()
{
	Attack_explanationFlag = false;
	//�|�C���^�[�ƃA�^�b�N�A�C�R���̃S�[�X�g�I�u�W�F�N�g�̂����蔻����s���B
	PhysicsWorld::GetInstance()->ContactTest(Poimter, [&](const btCollisionObject& contactObject) {
		if (Attack.IsSelf(contactObject) == true) {
			//m_physicsGhostObject�ƂԂ������B
			//�t���O��true�ɂ���B
			//m_isHit = true;
			Attack_explanationFlag = true;
		}
		});

	Skill_explanationFlag = false;
	//�|�C���^�[�ƃA�^�b�N�A�C�R���̃S�[�X�g�I�u�W�F�N�g�̂����蔻����s���B
	PhysicsWorld::GetInstance()->ContactTest(Poimter, [&](const btCollisionObject& contactObject) {
		if (Skill.IsSelf(contactObject) == true) {
			//m_physicsGhostObject�ƂԂ������B
			//�t���O��true�ɂ���B
			//m_isHit = true;
			Skill_explanationFlag = true;
		}
		});

	Ult_explanationFlag = false;
	PhysicsWorld::GetInstance()->ContactTest(Poimter, [&](const btCollisionObject& contactObject) {
		if (UltimateSkill.IsSelf(contactObject) == true) {
			//m_physicsGhostObject�ƂԂ������B
			//�t���O��true�ɂ���B
			//m_isHit = true;
			Ult_explanationFlag = true;
		}
		});
}

void CharacterSelect::Cursor()
{
	//�㉺���E�ړ�
	if (g_pad[0]->IsTrigger(enButtonUp) && CursorNum > 1)
	{
		//�I����
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(5);
		se->Play(false);
		se->SetVolume(1.0f);
		//�ړ�
		CursorNum -= 2;
	}
	if (g_pad[0]->IsTrigger(enButtonDown) && CursorNum < 2)
	{
		//�I����
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(5);
		se->Play(false);
		se->SetVolume(1.0f);
		//�ړ�
		CursorNum += 2;
	}
	if (g_pad[0]->IsTrigger(enButtonLeft) && CursorNum != 0 && CursorNum != 2)
	{
		//�I����
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(5);
		se->Play(false);
		se->SetVolume(1.0f);
		//�ړ�
		CursorNum -= 1;
	}
	if (g_pad[0]->IsTrigger(enButtonRight) && CursorNum != 3 && CursorNum != 1)
	{
		//�I����
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(5);
		se->Play(false);
		se->SetVolume(1.0f);
		//�ړ�
		CursorNum += 1;
	}

	switch (CursorNum)
	{
	case 0:
		m_characterSelect = enCharacterSelect_Knight;
		curPosition = { -510.0f,75.0f,0.0f };
		break;
	case 1:
		m_characterSelect = enCharacterSelect_Wizard;
		curPosition = { -40.0f,75.0f,0.0f };
		break;
	case 2:
		m_characterSelect = enCharacterSelect_Zombie;
		curPosition = { -510.0f,-275.0f,0.0f };
		break;
	case 3:
		m_characterSelect = enCharacterSelect_Mitei2;
		curPosition = { -40.0f,-275.0f,0.0f };
		break;
	}

	m_SelectCursor.SetPosition(curPosition);
	m_SelectCursor.Update();
}

//�Q�[���ɑJ�ڂ���O�Ƀt�F�[�h�A�E�g����
void CharacterSelect::Ready()
{
	if (fade->GetCurrentAlpha() >= 1.0f)
	{
		Game* game = NewGO<Game>(0, "game");
		//�L�����N�^�[�Z���N�g��
		switch (m_characterSelect)
		{
			//���m��������
		case enCharacterSelect_Knight:
			game->SetCharacterSelect(m_characterSelect);
			break;
			//���@�g����������
		case enCharacterSelect_Wizard:
			game->SetCharacterSelect(m_characterSelect);
			break;
			//�]���r(�\��)��������
		case enCharacterSelect_Zombie:
			game->SetCharacterSelect(enCharacterSelect_Knight);
			break;
			//����
		case enCharacterSelect_Mitei2:
			game->SetCharacterSelect(enCharacterSelect_Wizard);
			break;
		}
		DeleteGO(this);

	}
	
}

/// <summary>
/// ���m�̃��f���A�A�j���[�V�����ǂݍ���
/// </summary>
void CharacterSelect::SetKnightModel()
{
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/Knight/Knight_idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Walk].Load("Assets/animData/Knight/Knight_Walk.tka");
	m_animationClips[enAnimationClip_Walk].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Run].Load("Assets/animData/Knight/run.tka");
	m_animationClips[enAnimationClip_Run].SetLoopFlag(true);
	m_animationClips[enAnimationClip_ChainAtk].Load("Assets/animData/Knight/ChainAttack/first_Attack.tka");
	m_animationClips[enAnimationClip_ChainAtk].SetLoopFlag(false);
	m_animationClips[enAnimationClip_SecondAtk].Load("Assets/animData/Knight/ChainAttack/Second_Attack.tka");
	m_animationClips[enAnimationClip_SecondAtk].SetLoopFlag(false);
	m_animationClips[enAnimationClip_lastAtk].Load("Assets/animData/Knight/ChainAttack/last_Attack.tka");
	m_animationClips[enAnimationClip_lastAtk].SetLoopFlag(false);
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
	m_animationClips[enAnimationClip_Jump].Load("Assets/animData/Knight/Knight_Jump.tka");
	m_animationClips[enAnimationClip_Jump].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Fall].Load("Assets/animData/Knight/Knight_fall2.tka");
	m_animationClips[enAnimationClip_Fall].SetLoopFlag(true);

	//���m���f����ǂݍ���
	m_Knight.Init("Assets/modelData/character/Knight/Knight_Blue2.tkm", m_animationClips, enAnimationClip_Num, enModelUpAxisZ);
	m_Knight.SetPosition(KnightPos);
	m_Knight.SetScale(2.7f, 2.7f, 2.7f);
	m_Knight.Update();
}

/// <summary>
/// �R���W�����쐬
/// </summary>
void CharacterSelect::SetCollision()
{

	/*POINTER = NewGO<CollisionObject>(0,"Pointer");
	POINTER->CreateCapsule(m_Pointerposition, Quaternion::Identity, 20.0f, 1.0f);
	POINTER->SetIsEnable(false);
	POINTER->SetIsEnableAutoDelete(false);
	POINTER->Update();*/

	//�U���A�C�R���̓����蔻��
	Attack.CreateBox(AttackCollision, AttackCollisionRot, Vector3(50.0f, 50.0f, 5.0f));
	//�X�L���A�C�R���̓����蔻��
	Skill.CreateSphere(SkillCollision, Quaternion::Identity, 30.0f);
	//�K�E�Z�A�C�R���̓����蔻��
	UltimateSkill.CreateBox(UltimateCollision, Quaternion::Identity, Vector3(50.0f, 50.0f, 5.0f));
}

void CharacterSelect::Render(RenderContext& rc)
{
	//m_SelectCursor.Draw(rc);

	//���m�̃��f��
	m_Knight.Draw(rc);
	m_platform.Draw(rc);

	m_Status.Draw(rc);
	m_Attack_Icon.Draw(rc);
	m_name.Draw(rc);
	m_UnderBar.Draw(rc);

	if (Attack_explanationFlag == true)
	{
		m_Attack_explanation.Draw(rc);
	}
	if (Skill_explanationFlag == true)
	{
		Skill_explanation.Draw(rc);
	}
	if (Ult_explanationFlag == true)
	{
		Ult_explanation.Draw(rc);
	}

	//�_�ő���todo
	if ((int)time % 2 == 0)
	{
		m_pointer_white.Draw(rc);
	}
	
	m_pointer_black.Draw(rc);
}

