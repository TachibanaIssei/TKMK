#include "stdafx.h"
#include "ChaseEFK.h"
#include "Actor.h"
#include "Neutral_Enemy.h"

void ChaseEFK::Update() {

	if (m_targetActor == nullptr && m_targetEnemy == nullptr)
	{
		// �����ݒ肳��Ă��Ȃ�
		DeleteGO(this);
		return;
	}
	if (m_effect == nullptr) {
		// �����ݒ肳��Ă��Ȃ�
		DeleteGO(this);
		return;
	}

	if(m_targetActor != nullptr)
	{
		// �A�N�^�[�ǔ�
		m_effect->SetPosition(m_targetActor->GetPosition());
	}
	else if (m_targetEnemy != nullptr)
	{
		//�G�l�~�[�ǔ�
		m_effect->SetPosition(m_targetEnemy->GetPosition());	
	}

	// ������]
	if (autoRot && m_targetActor != nullptr) {
		Quaternion rot = m_targetActor->GetRot();
		m_effect->SetRotation(rot);
	}

	if (m_effect != nullptr)
	{
		m_effect->Update();

	}
	
	// ���g���폜
	if (m_effect->IsPlay() == false && autoDelete == true) {
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

	m_effect->Play();

	m_targetActor = target;
};

void ChaseEFK::SetEnemyEffect(EffectEmitter*name,Neutral_Enemy* target)
{
	// �G�t�F�N�g�����
	m_effect = name;
	Vector3 effectPosition = target->GetPosition();
	m_effect->SetPosition(effectPosition);

	m_targetEnemy = target;
}