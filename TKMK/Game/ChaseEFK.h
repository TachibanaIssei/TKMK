#pragma once
#include "Effect.h"
class Actor;

class ChaseEFK : public IGameObject
{
public:
	void Update();

	/// <summary>
	/// エフェクトを設定＆再生
	/// </summary>
	/// <param name="effect">種類</param>
	/// <param name="target">追尾対象</param>
	void SetEffect(EnEFK effect, Actor* target, Vector3 scale = Vector3::One);

	EffectEmitter* GetEffect()
	{
		return m_effect;
	}

	void DeleteEffect() {
		m_effect->Stop();
		DeleteGO(m_effect);
	}

	void AutoDelete(bool flag) {
		autoDelete = flag;
	}

private:
	EffectEmitter* m_effect = nullptr;
	Actor* m_targetActor = nullptr;

	bool autoDelete = true;
};

