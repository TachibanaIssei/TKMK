#include "stdafx.h"
#include "ChaseEFK.h"
#include "Actor.h"
#include "Neutral_Enemy.h"

void ChaseEFK::Update() {

	if (m_targetActor == nullptr)
	{
		return;
	}
	else
	{
		// �A�N�^�[�ǔ�
		m_effect->SetPosition(m_targetActor->GetPosition());

	}
	if (m_targetEnemy == nullptr)
	{
		return;
	}
	else
	{
		//�G�l�~�[�ǔ�
		m_EnemyEffect->SetPosition(m_targetEnemy->GetPosition());
		
	}
	// ���g���폜
	if (m_effect->IsPlay() == false && autoDelete == true) {
		DeleteGO(this);
	}
	if (m_EnemyEffect->IsPlay() == false)
	{
		DeleteGO(this);
	}
}

void ChaseEFK::SetEffect(EnEFK effect, Actor* target,Vector3 scale) {

	// �G�t�F�N�g�����
	m_effect = NewGO<EffectEmitter>(5);
	m_effect->Init(effect);
	m_effect->SetScale(scale);
	Vector3 effectPosition = target->GetPosition();
	m_effect->SetPosition(effectPosition);
	Quaternion rot = target->GetRot();
	m_effect->SetRotation(rot);
	m_effect->Play();

	m_targetActor = target;
};

void ChaseEFK::SetEnemyEffect(EffectEmitter*name,Neutral_Enemy* target)
{
	// �G�t�F�N�g�����
	m_EnemyEffect = name;
	Vector3 effectPosition = target->GetPosition();
	m_EnemyEffect->SetPosition(effectPosition);

	m_targetEnemy = target;
}