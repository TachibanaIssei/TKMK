#pragma once

class Player;
class KnightAI;
class Actor;

class WizardUlt:public IGameObject
{
public:
	WizardUlt();
	~WizardUlt();

	//�K�E�Z���������L�����B
	enum EnUltimateSkillian {
		enUltimateSkill_None,
		enUltimateSkill_Player,			//�v���C���[�B
		enUltimateSkill_Wizard,			//���@�g���B
	};
	EnUltimateSkillian m_ultSkillian;

	//���@�g���̕K�E�Z��ł��ꂽ�L����
	enum EnDamegedChar
	{
		enDamegedChar_Plater,
		enDamegedChar_KnightAI,
		enDamegedChar_WizardAI,
		enDamegedChar_ZombieAI,
		enDamegedChar_MonsterAI
	};
	EnDamegedChar m_DamegedChar;

	bool Start();
	void Update();
	void Move();

	void Damege();

	/// <summary>
		/// ���W��ݒ肷��B
		/// </summary>
		/// <param name="position">���W�B</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	/// <summary>
	/// ��]��ݒ肷��B
	/// </summary>
	/// <param name="rotation">��]�B</param>
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}
	/// <summary>
	/// �傫����ݒ肷��B
	/// </summary>
	/// <param name="scale">�傫���B</param>
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}
	/// <summary>
	/// �p�҂�ݒ肷��B
	/// </summary>
	/// <param name="enMagician">�p�ҁB</param>
	void SetEnMagician(const EnUltimateSkillian enMagician)
	{
		m_ultSkillian = enMagician;
	}

	/// <summary>
	/// ���̃N���X��������I�u�W�F�N�g�̖��O
	/// </summary>
	/// <param name="collisionname">���O</param>
	void SetCreatorName(const char* creatorname)
	{
		//�R���W�����I�u�W�F�N�g���쐬����B
		UltCollision = NewGO<CollisionObject>(0);
		//���̃N���X��������I�u�W�F�N�g�̖��O
		UltCollision->SetCreatorName(creatorname);
	}

	void CreatCollision()
	{
		UltCollision->CreateSphere(m_position, Quaternion::Identity, 20.0f);
		UltCollision->SetName("Wizard_UltSkill");
		//���菊�I�u�W�F�N�g�������ō폜����Ȃ��悤�ɂ���B
		UltCollision->SetIsEnableAutoDelete(false);
	}
	
	/// <summary>
	/// �A�N�^�[�̃I�u�W�F�N�g�����擾����
	/// </summary>
	/// <param name="character">�I�u�W�F�N�g�̖��O</param>
	const void SetActor(const char* character)
	{
		m_targrtName = character;
	}

	/// <summary>
	/// �A�N�^�[�̃I�u�W�F�N�g����Ԃ�
	/// </summary>
	/// <returns>�I�u�W�F�N�g�̖��O</returns>
	const char* SetTargetActorName()
	{
		return m_targrtName;
	}

private:
	Player* player = nullptr;
	KnightAI* knightAI = nullptr;

	Actor* m_targetActor = nullptr;
	Actor* m_CreatMeActor = nullptr;
	Actor* m_GivePointActor = nullptr;

	Vector3 m_position = Vector3::Zero;
	Quaternion m_rotation;
	Vector3 m_scale;
	Vector3	m_moveSpeed;
	CollisionObject* UltCollision;

	float m_timer = 0.0f;
	const char* m_targrtName = nullptr;
};

