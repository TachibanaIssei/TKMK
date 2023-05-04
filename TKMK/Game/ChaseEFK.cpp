#include "stdafx.h"
#include "ChaseEFK.h"
#include "Actor.h"

void ChaseEFK::Update() {

	if (m_targetActor == nullptr) {
		return;
	}

	// 追尾
	m_effect->SetPosition(m_targetActor->GetPosition());

	// 自身を削除
	if (m_effect->IsPlay() == false && autoDelete == true) {
		DeleteGO(this);
	}

}

void ChaseEFK::SetEffect(EnEFK effect, Actor* target, Vector3 scale) {

	// エフェクトを作る
	m_effect = NewGO<EffectEmitter>(5);
	m_effect->Init(effect);
	m_effect->SetScale(scale);
	Vector3 effectPosition = target->GetPosition();
	m_effect->SetPosition(effectPosition);
	m_effect->Play();

	m_targetActor = target;
};
