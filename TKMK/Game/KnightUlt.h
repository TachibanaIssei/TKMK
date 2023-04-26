#pragma once
class Game;
class GameCamera;
class Actor;

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
	void Render(RenderContext& rc);

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
	void SetCollision();

	/// <summary>
	/// ���f���A�G�t�F�N�g�̐���
	/// </summary>
	void MakeUlt();

	/// <summary>
	/// ���W���擾
	/// </summary>
	/// <returns>���W</returns>
	const Vector3& GetPosition() const
	{
		return m_position;
	}

	/// <summary>
	/// �v���C���[�̑O�������擾�B
	/// </summary>
	const Vector3& GetForward() const
	{
		return m_forward;
	}

	/// <summary>
	/// �����蔻�肪�����ł��邩���ׂ�
	/// </summary>
	/// <returns></returns>
	bool MakeCheck();

	/// <summary>
	/// �L�����̃��x�����擾����
	/// </summary>
	/// <param name="level">�K�E�Z��ł����L�����̃��x��</param>
	void GetCharLevel(int level)
	{
		CharLevel = level;
	}

	void SetDeletetime()
	{
		if (CharLevel < 6)
		{
			DeleteTime = 1;
		}
		//���x����7�ȉ��Ȃ�
		//�K�E�Z��i�K����
		else if (CharLevel < 8)
		{
			DeleteTime = 1;
		}
		//���x����10�ȉ��Ȃ�
		//�K�E�Z��i�K����
		else if (CharLevel <= 10)
		{
			DeleteTime = 1;
		}
	}

	void SetGame(Game* game)
	{
		m_game = game;
	}
	
	/// <summary>
	/// �����҂�ݒ�
	/// </summary>
	void SetActor(Actor* actor) {
		m_actor = actor;
	}

private:
	ModelRender model;

	Vector3 m_firstposition = Vector3::Zero;
	Vector3 m_position = Vector3::Zero;
	Vector3 m_Checkposition = Vector3::Zero;

	Quaternion m_rotation;
	Vector3 m_scale;
	Vector3	m_moveSpeed;
	Vector3 m_forward;
	EffectEmitter* Ulteffect;
	//�G�t�F�N�g�̓����蔻��
	CollisionObject* UltCollision;
	//���̓����蔻�肪�ǂɓ������������
	CollisionObject* UltDeleteJudgeCollision;

	Game* m_game;
	RigidBody				m_rigidBody;						//���́B
	SphereCollider			m_sphereCollider;							//�R���C�_�[�B

	//char m_collisionName;

	float m_timer = 0.0f;

	//�����蔻�����������
	float DeleteTime;

	//���������L�����̃��x��
	int CharLevel = 0;
	//bool Ultflag = true;

	// ������
	Actor* m_actor = nullptr;
	// �Q�[���J����
	GameCamera* m_gameCamera = nullptr;
};

