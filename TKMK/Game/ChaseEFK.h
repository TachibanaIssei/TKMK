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
	// �G�t�F�N�g���擾
	EffectEmitter* GetEffect()
	{
		return m_effect;
	}

	// �G�t�F�N�g���폜
	void DeleteEffect() {
		m_effect->Stop();
		DeleteGO(m_effect);
	}

	// �����ŏ����Ȃ��悤�ɂ���
	void AutoDelete(bool flag) {
		autoDelete = flag;
	}

private:
	EffectEmitter* m_effect = nullptr;
	Actor* m_targetActor = nullptr;
	EffectEmitter* m_EnemyEffect = nullptr;
	Neutral_Enemy* m_targetEnemy = nullptr;
	bool autoDelete = true;
};

