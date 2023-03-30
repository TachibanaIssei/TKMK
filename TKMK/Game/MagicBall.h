#pragma once
#include "Status.h"
class Status;

class MagicBall:public IGameObject
{
public:
	MagicBall();
	~MagicBall();

	//�}�W�b�N�{�[�����������L�����B
	enum EnMagician {
		enMagician_None,
		enMagician_Player,			//�v���C���[�B
		enMagician_Wizard,			//���@�g���B
	};
	EnMagician m_enMagician;

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
	void SetEnMagician(const EnMagician enMagician)
	{
		m_enMagician = enMagician;
	}


	/// <summary>
	/// ���@�g���̍U���͂��擾
	/// </summary>
	/// <param name="status">���@�g���̍U����</param>
	void GetWizardAttack(Status status)
	{
		m_Status.Atk = status.Atk;
	}

	/// <summary>
	/// ���@�g���̍U���͂�Ԃ�
	/// </summary>
	/// <returns>���@�g���̍U����</returns>
	int SetWizardAttack()const
	{
		return m_Status.Atk;
	}

private:
	Status m_Status;

	Vector3 m_position = Vector3::Zero;
	Quaternion m_rotation;
	Vector3 m_scale;
	Vector3	m_moveSpeed;
	CollisionObject* BallCollision;

	float m_timer = 0.0f;

	int WizardAtk = 0;

};

