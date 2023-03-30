#pragma once
class KnightUlt:public IGameObject
{
public:
	KnightUlt();
	~KnightUlt();

	//�}�W�b�N�{�[�����������L�����B
	enum EnUltSkill {
		enUltSkill_None,
		enUltSkill_Player,			//�v���C���[�B
		enUltSkill_Knight,			//kenn�B
	};
	EnUltSkill m_enUltSkill;

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
	void SetEnUlt(const EnUltSkill enMagician)
	{
		m_enUltSkill = enMagician;
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

	/// <summary>
	/// �����蔻��̐ݒ�
	/// </summary>
	void SetCollision(Vector3 collisionsize)
	{
		
		//����̃R���W�������쐬����B
		UltCollision->CreateBox(m_position, m_rotation, collisionsize);
		//�R���W�����̖��O
		UltCollision->SetName("player_UltimateSkill");

		//���菊�I�u�W�F�N�g�������ō폜����Ȃ��悤�ɂ���B
		UltCollision->SetIsEnableAutoDelete(false);
	}

private:
	Vector3 m_position = Vector3::Zero;
	Quaternion m_rotation;
	Vector3 m_scale;
	Vector3	m_moveSpeed;
	CollisionObject* UltCollision;

	//char m_collisionName;

	float m_timer = 0.0f;

};

