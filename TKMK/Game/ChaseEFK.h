#pragma once
#include "Effect.h"
class Actor;
class Neutral_Enemy;

class ChaseEFK : public IGameObject
{
public:
	void Update();

	/// <summary>
	/// エフェクトを設定＆再生
	/// </summary>
	/// <param name="effect">種類</param>
	/// <param name="target">追尾対象</param>
	void SetEffect(EnEFK effect, Actor* target,Vector3 scale = Vector3::One);

	void SetEnemyEffect(EffectEmitter* name,Neutral_Enemy* target);

	/// <summary>
	/// 自動で回転させるかどうか
	/// </summary>
	void AutoRot(bool flag) {
		autoRot = flag;
	}

	// エフェクトを取得
	EffectEmitter* GetEffect()
	{
		return m_effect;
	}

	// エフェクトを削除
	void DeleteEffect() {
		m_effect->Stop();
		m_effect = nullptr;
		DeleteGO(m_effect);
	}

	// 自動で消えないようにする
	void AutoDelete(bool flag) {
		autoDelete = flag;
	}

	void ResetTarget() {
		m_targetActor = nullptr;
		m_targetEnemy = nullptr;
	}

private:
	EffectEmitter* m_effect = nullptr;
	Actor* m_targetActor = nullptr;
	Neutral_Enemy* m_targetEnemy = nullptr;
	bool autoDelete = true;
	bool autoRot = false;
};

