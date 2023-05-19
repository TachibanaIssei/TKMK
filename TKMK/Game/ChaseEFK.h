#pragma once
#include "Effect.h"
class Actor;
class Neutral_Enemy;

class ChaseEFK : public IGameObject
{
public:
	void Update();

	/// <summary>
	/// �G�t�F�N�g��ݒ聕�Đ�
	/// </summary>
	/// <param name="effect">���</param>
	/// <param name="target">�ǔ��Ώ�</param>
	void SetEffect(EnEFK effect, Actor* target,Vector3 scale = Vector3::One);

	void SetEnemyEffect(EffectEmitter* name,Neutral_Enemy* target);

	/// <summary>
	/// ���W�̕␳�ʂ�ݒ�
	/// </summary>
	void SetAddPos(Vector3 addpos) {
		addPos = addpos;
	}

	/// <summary>
	/// �����ŉ�]�����邩�ǂ���
	/// </summary>
	void AutoRot(bool flag) {
		autoRot = flag;
	}
	/// <summary>
	/// ��]�̉��Z��
	/// AutoRot���̂ݗL��
	/// </summary>
	void SetAutoRotAddY(float addY) {
		autoRot_AddY_Rot = addY;
	}

	// �G�t�F�N�g���擾
	EffectEmitter* GetEffect()
	{
		return m_effect;
	}

	// �G�t�F�N�g���폜
	void DeleteEffect() {
		m_effect->Stop();
		m_effect = nullptr;
		DeleteGO(m_effect);
	}

	// �����ŏ����Ȃ��悤�ɂ���
	void AutoDelete(bool flag) {
		autoDelete = flag;
	}

	/// <summary>
	/// �G�t�F�N�g�̍폜�����i�G���[�h�~�j
	/// </summary>
	void ResetTarget() {
		m_targetActor = nullptr;
		m_targetEnemy = nullptr;
		m_effect = nullptr;
	}

private:
	EffectEmitter* m_effect = nullptr;
	Actor* m_targetActor = nullptr;
	Neutral_Enemy* m_targetEnemy = nullptr;
	bool autoDelete = true;
	bool autoRot = false;
	float autoRot_AddY_Rot = 0.0f;	// �G�t�F�N�g��Y������]�ʁiAutoRot��true�̎��̂ݗL���j
	Vector3 addPos = Vector3::Zero;	// �G�t�F�N�g�̍��W���Z��

};

