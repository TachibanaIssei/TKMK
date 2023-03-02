#pragma once

#include "Actor.h"


class KnightBase:public Actor,public IGameObject
{
public:
	KnightBase();
	~KnightBase();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void Attack();
	void Dameged(int damege);
	void Skill();
	void UltimateSkill();
	void Death();
	void Rotation();
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

	enum EnAnimationClip {
		enAnimationClip_Idle,
		//enAnimationClip_Run,
		//enAnimationClip_Jump,
		enAnimationClip_Num,
	};

	Vector3 firstposition;                                //最初の座標
	Vector3 m_position = Vector3::Zero;                   //座標
	Vector3 m_forward = Vector3::AxisZ;                   //正面ベクトル
	Vector3 m_moveSpeed;                                  //移動速度
	CharacterController m_charCon;                        //キャラクターコントロール
	Quaternion m_rot = Quaternion::Identity;              //クォータニオン
	ModelRender m_modelRender;                            //モデルレンダー
	AnimationClip m_animationClips[enAnimationClip_Num]; //アニメーションクリップ


};

