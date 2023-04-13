#include "stdafx.h"
#include "Actor.h"

Actor::Actor()
{

}

Actor::~Actor()
{

}

void Actor::Move(Vector3& position, CharacterController& charcon,Status& status,Vector3 stickL)
{
	//����̃A�j���[�V�������Đ����Ȃ�
	if (IsEnableMove() == false)
	{
		//�����o���@�ړ��������s��Ȃ�
		return;
	}

	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;

	//�J�����̑O�����ƉE�����̃x�N�g���������Ă���B
	Vector3 forward = g_camera3D->GetForward();
	Vector3 right = g_camera3D->GetRight();
	//y�����ɂ͈ړ������Ȃ��B
	forward.y = 0.0f;
	right.y = 0.0f;
	forward.Normalize();
	//���X�e�B�b�N�̓��͗ʂ�status�̃X�s�[�h����Z�B
	right *= stickL.x * status.Speed;
	forward *= stickL.y * status.Speed;

	//�v���C���[�̑O�����̏����X�V
	//x��z�̈ړ����x����������(�X�e�B�b�N�̓��͂���������)�B
	if (fabsf(forward.x) >= 0.001f || fabsf(forward.z) >= 0.001f)
	{
		m_Forward = forward+ right;
	}

	//�ړ����x�ɃX�e�B�b�N�̓��͗ʂ����Z����B
	m_moveSpeed += right + forward;
	//�d�͂�t�^����
	m_moveSpeed.y -= 980.0f * g_gameTime->GetFrameDeltaTime();

	//�L�����N�^�[�R���g���[���[���g���č��W���ړ�������B
	position = charcon.Execute(m_moveSpeed, 1.0f / 60.0f);
	//�n�ʂɂ����B
	if (charcon.IsOnGround()) {
		m_moveSpeed.y = 0.0f;
	}
	//�L�����N�^�[�R���g���[���[���g���č��W���ړ�������B�����Ȃ��Ă��܂�
	//position = charcon.Execute(m_moveSpeed, 1.0f / 60.0f);
	//m_modelRender.SetPosition(position);

}

//���X�|�[��������W��ݒ肷��
void Actor::GetRespawnPos()
{
	m_respawnLevel.Init("Assets/level3D/CharRespawn2Level.tkl", [&](LevelObjectData& objData) {

		if (objData.ForwardMatchName(L"CharPos") == true) {
			//����̍��W
			if (objData.number == 0) {
				SetRespawnPos(objData.position, objData.rotation, objData.number);
				return true;
			}
			//�E��̍��W
			if (objData.number == 1) {
				SetRespawnPos(objData.position, objData.rotation, objData.number);
				return true;
			}
			//�E���̍��W
			if (objData.number == 2) {
				SetRespawnPos(objData.position, objData.rotation, objData.number);
				return true;
			}
			//�����̍��W
			if (objData.number == 3) {
				SetRespawnPos(objData.position, objData.rotation, objData.number);
				return true;
			}
		}
		return true;
		});
}

/// <summary>
/// ���x���A�b�v�̏���
/// </summary>
/// <param name="lus">���x���A�b�v���ɑ�������X�e�[�^�X</param>
/// <param name="nowStatus">���݂̃X�e�[�^�X</param>
/// <param name="Level">���݂̃��x��</param>
void Actor::LevelUp(LvUpStatus& lus,Status& nowStatus,int& Level)
{
	nowStatus.MaxHp+= lus.LvHp;
	nowStatus.Hp += lus.LvHp;
	nowStatus.Atk += lus.LvAtk;
	nowStatus.Speed += lus.LvSpeed;
	Level++;
}

/// <summary>
/// ���x���_�E���̏���
/// </summary>
/// /// <param name="lus">���x���A�b�v���ɑ�������X�e�[�^�X</param>
/// <param name="nowStatus">���݂̃X�e�[�^�X</param>
/// <param name="Level">���݂̃��x��</param>
/// <param name="downLevel">�����郌�x���̐�</param>
void Actor::levelDown(LvUpStatus& lus, Status& nowStatus, int& Level, int downLevel)
{
	Level-= downLevel;
	//�������x����0�Ȃ�1�ɂ���
	if (Level == 0) {
		Level = 1; 
		return;
	}

	nowStatus.MaxHp-= downLevel*lus.LvHp;
	//����HP��MaxHp������Ȃ�
	if (nowStatus.Hp > nowStatus.MaxHp)
		//HP��MaxHp�𓯂��ɂ���
		nowStatus.Hp = nowStatus.MaxHp;

	nowStatus.Atk -= downLevel*lus.LvAtk;
	nowStatus.Speed -= downLevel*lus.LvSpeed;
}
/// <summary>
/// �����̓G��|�����Ƃ��̌o���l�̏���
/// </summary>
/// <param name="GetExp">�����̓G�̌o���l</param>
void Actor::ExpProcess(int Exp)
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
		LevelUp(LvUPStatus, m_Status, Lv);
		//���x���ɍ��킹�ă��x���̉摜��ύX����
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
/// ���X�|�[�������Ƃ��̃��x���ɂ���Čo���l��ύX����
/// </summary>
/// <param name="Lv">���݂̃��x��</param>
/// <param name="getExp">�o���l</param>
void Actor::ExpReset(int& Lv, int& getExp)
{
	//�o���l�����Z�b�g
	switch (Lv)
	{
	case 1:
		getExp = 0;
		break;
	case 2:
		getExp = 5;
		break;
	case 3:
		getExp = 10;
		break;
	case 4:
		getExp = 20;
		break;
	default:
		break;
	}
}

/// <summary>
/// �o���l�e�[�u����ύX����
/// </summary>
/// <param name="Lv">���݂̃��x��</param>
/// <param name="expTable">�o���l�e�[�u��</param>
void Actor::ExpTableChamge(int& Lv, int& expTable)
{
	switch (Lv)
	{
	case 1:
		expTable = 5;
		break;
	case 2:
		expTable = 10;
		break;
	case 3:
		expTable = 20;
		break;
	case 4:
		expTable = 30;
		break;

	default:
		break;
	}
}

/// <summary>
/// �X�L�����g�p������̃N�[���^�C���̏���
/// </summary>
/// <param name="SkillCooltimer">�X�L���̃N�[���^�C��</param>
/// <param name="skillstate">�X�L�����g�p�������̔���</param>
/// <param name="timer">�N�[���^�C�����v�Z����ϐ�</param>
void Actor::COOlTIME(float SkillCooltimer, bool& skillstate,float& timer)
{

	//�X�L���̃A�j���[�V�����Đ����I�������
	if (skillstate==true)
	{
		if (timer <= 0)
		{
			//�X�L���g�p�\
			skillstate = false;
			timer = SkillCooltimer;

		}
		else timer -= g_gameTime->GetFrameDeltaTime();   //timer��i�߂�

		//timer���X�L���̃N�[���^�C�����傫���Ȃ�����B
		//if (timer >= SkillCooltimer)
		//{
		//	//�X�L���g�p�\
		//	skillstate = false;
		//	timer = 0;
		//}
		//else timer += g_gameTime->GetFrameDeltaTime();   //timer��i�߂�
	}
	
}

/// <summary>
/// ���X�|�[�������Ƃ��ɓ������э~��鏈��
/// </summary>
void Actor::RespawnMove(Vector3& position, Quaternion& rotation, CharacterController& charCon)
{
	float jump = 10.0f;
	//��э~���
	//�W�����v����
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		m_RespawnJumpFlag = true;
	}
	
	if (m_RespawnJumpFlag == true)
	{
		if (Count < 20)
		{
			m_moveSpeed = Vector3::AxisZ;
			rotation.Apply(m_moveSpeed);
			//��ԕ��������߂�
			m_moveSpeed *= 400.0f;

			position.y += jump;
			position += m_moveSpeed * g_gameTime->GetFrameDeltaTime();

			Count++;
		}
	}
	
	charCon.SetPosition(position);
	

	//m_TowerToGroundFlag = true;
}


