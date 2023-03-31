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

private:
	Player* player = nullptr;
	KnightAI* knightAI = nullptr;
	Actor* m_targetActor = nullptr;

	Vector3 m_position = Vector3::Zero;
	Quaternion m_rotation;
	Vector3 m_scale;
	Vector3	m_moveSpeed;
	CollisionObject* UltCollision;

	float m_timer = 0.0f;

};

