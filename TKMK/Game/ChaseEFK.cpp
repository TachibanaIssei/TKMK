#include "stdafx.h"
#include "ChaseEFK.h"
#include "Actor.h"

void ChaseEFK::Update() {

	if (m_targetActor == nullptr) {
		return;
	}

	// �ǔ�
	m_effect->SetPosition(m_targetActor->GetPosition());

	// ���g���폜
	if (m_effect->IsPlay() == false && autoDelete == true) {
		DeleteGO(this);
	}

}

void ChaseEFK::SetEffect(EnEFK effect, Actor* target, Vector3 scale) {

	// �G�t�F�N�g�����
	m_effect = NewGO<EffectEmitter>(5);
	m_effect->Init(effect);
	m_effect->SetScale(scale);
	Vector3 effectPosition = target->GetPosition();
	m_effect->SetPosition(effectPosition);
	m_effect->Play();

	m_targetActor = target;
};
