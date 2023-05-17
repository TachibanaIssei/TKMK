#include "stdafx.h"
#include "ChaseEFK.h"
#include "Actor.h"
#include "Neutral_Enemy.h"

void ChaseEFK::Update() {

	if (m_targetActor == nullptr && m_targetEnemy == nullptr)
	{
		// 何も設定されていない
		DeleteGO(this);
		return;
	}
	if (m_effect == nullptr) {
		// 何も設定されていない
		DeleteGO(this);
		return;
	}

	if(m_targetActor != nullptr)
	{
		// アクター追尾
		m_effect->SetPosition(m_targetActor->GetPosition());
	}
	else if (m_targetEnemy != nullptr)
	{
		//エネミー追尾
		m_effect->SetPosition(m_targetEnemy->GetPosition());	
	}

	// 自動回転
	if (autoRot && m_targetActor != nullptr) {
		Quaternion rot = m_targetActor->GetRot();
		m_effect->SetRotation(rot);
	}

	if (m_effect != nullptr)
	{
		m_effect->Update();

	}
	
	// 自身を削除
	if (m_effect->IsPlay() == false && autoDelete == true) {
		DeleteGO(this);
	}

}

void ChaseEFK::SetEffect(EnEFK effect, Actor* target,Vector3 scale) {

	// エフェクトを作る
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
	// エフェクトを作る
	m_effect = name;
	Vector3 effectPosition = target->GetPosition();
	m_effect->SetPosition(effectPosition);

	m_targetEnemy = target;
}