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
	/// 座標の補正量を設定
	/// </summary>
	void SetAddPos(Vector3 addpos) {
		addPos = addpos;
	}

	/// <summary>
	/// 自動で回転させるかどうか
	/// </summary>
	void AutoRot(bool flag) {
		autoRot = flag;
	}
	/// <summary>
	/// 回転の加算量
	/// AutoRot中のみ有効
	/// </summary>
	void SetAutoRotAddY(float addY) {
		autoRot_AddY_Rot = addY;
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

	/// <summary>
	/// エフェクトの削除準備（エラー防止）
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
	float autoRot_AddY_Rot = 0.0f;	// エフェクトのY軸回り回転量（AutoRotがtrueの時のみ有効）
	Vector3 addPos = Vector3::Zero;	// エフェクトの座標加算量

};

