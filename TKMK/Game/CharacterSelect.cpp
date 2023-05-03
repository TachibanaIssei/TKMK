#include "stdafx.h"
#include "CharacterSelect.h"
#include "Tittle.h"
#include "Game.h"

namespace {
	const Vector3 StatusPos = Vector3(400.0f, 250.0f, 0.0f);			//�X�e�[�^�X�̍��W
	const Vector3 Attack_IconPos = Vector3(400.0f, -150.0f, 0.0f);		//�U���A�X�L���A�K�E�Z�A�C�R���̍��W
	const Vector3 NamePos = Vector3(-600.0f, -400.0f, 0.0f);				//���O�̍��W
	const Vector3 UnderBarPos = Vector3(0.0f, -500.0f, 0.0f);			//��ʉ��̃o�[�̍��W

	const Vector3 KnightPOs = Vector3(-600.0f, 0.0f, 0.0f);
	const Quaternion KnightRot = Quaternion(0.0f, 90.0f, 0.0f, 1.0f);
}

CharacterSelect::CharacterSelect()
{

}

CharacterSelect::~CharacterSelect()
{

}

bool CharacterSelect::Start()
{
	

	Quaternion rot/*={0.0f,0.0f,0.0f,0.0f}*/;
	//rot.x = 10.0f;
	rot.Apply(m_CameraPosition);
	g_camera3D->SetPosition(m_CameraPosition);
	//g_camera3D->

	g_renderingEngine->SetAmbient(Vector4(0.5f, 0.5f, 0.5f, 1.0f));
	Vector3 dir = { 0.0f,-1.0f,0.0f };
	Vector4 color = { 0.5f,0.5f,0.5f,1.0f };
	g_renderingEngine->SetDirectionLight(0, dir, color);

	//���m�̃��f���A�A�j���[�V����
	SetKnightModel();

	//�L�����N�^�[�Z���N�g���
	m_CharacterSelectPic.Init("Assets/sprite/CharacterSelectPic.DDS", 1920.0f, 1080.0f);
	m_CharacterSelectPic.SetPosition(0.0f, 0.0f, 0.0f);
	m_CharacterSelectPic.SetScale(1.0f, 1.0f, 1.0f);
	m_CharacterSelectPic.Update();

	//���m�̉摜
	m_KnightPic.Init("Assets/sprite/Select/Knight.DDS", 300.0f, 300.0f); 
	m_KnightPic.SetPosition(-510.0f, 75.0f, 0.0f);
	m_KnightPic.SetScale(1.0f, 1.0f, 1.0f);
	m_KnightPic.Update();

	//���@�g���̉摜
	m_WizardPic.Init("Assets/sprite/Select/Wizard.DDS", 300.0f, 300.0f);
	m_WizardPic.SetPosition(-40.0f, 75.0f, 0.0f );
	m_WizardPic.SetScale(1.0f, 1.0f, 1.0f);
	m_WizardPic.Update();

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

	return true;
}

void CharacterSelect::Update()
{
	//�J�[�\��
	Cursor();
	
	//A�{�^������������
	if (g_pad[0]->IsTrigger(enButtonA))
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
	if (g_pad[0]->IsTrigger(enButtonB))
	{
		Tittle* tittle = NewGO<Tittle>(0, "tittle");
		DeleteGO(this);
	}

	m_CharacterSelectPic.Update();
	m_KnightPic.Update();
	m_WizardPic.Update();
	m_SelectCursor.Update();

	m_KnightRot.AddRotationDegY(4.0f);
	m_Knight.SetRotation(m_KnightRot);
	m_Knight.Update();
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
	m_Knight.Init("Assets/modelData/character/Knight/Knight_02.tkm", m_animationClips, enAnimationClip_Num, enModelUpAxisZ);
	m_Knight.SetPosition(-90.0f, 0.0f, 0.0f);
	m_Knight.SetScale(1.5f, 1.5f, 1.5f);
	/*Quaternion Krot;
	Krot.y = 90.0f;
	m_Knight.SetRotation(Krot);*/
	m_Knight.Update();
}

void CharacterSelect::Render(RenderContext& rc)
{
	//m_CharacterSelectPic.Draw(rc);
	//m_KnightPic.Draw(rc);
	//m_WizardPic.Draw(rc);
	//m_SelectCursor.Draw(rc);

	//���m�̃��f��
	m_Knight.Draw(rc);

	m_Status.Draw(rc);
	m_Attack_Icon.Draw(rc);
	m_name.Draw(rc);
	m_UnderBar.Draw(rc);
}

