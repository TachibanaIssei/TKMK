#pragma once
#include "WizardBase.h"

#include "Status.h"

class Game;
class Neutral_Enemy;
class MagicBall;

class WizardPlayer:public WizardBase
{
public:
	WizardPlayer();
	~WizardPlayer();

	void Update();
	void Attack();
	void Avoidance();
	void Render(RenderContext& rc);

	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

	inline Vector3 GetPosition() { return m_position; }

	/// <summary>
	/// マジックボールの生成
	/// </summary>
	void MakeMagicBall();

	/// <summary>
	/// アタックの当たり判定
	/// </summary>
	/// <param name="stickL">攻撃する方向のベクトル</param>
	/// 
	//void AttackCollision(Vector3& position/*,Vector3& stickL*/)
	//{
	//	//コリジョン生成していないなら
	//	if (AtkCollisionSetFlag == false)
	//	{
	//		AtkCollisionPos = position;
	//		AtkCollisionPos.y += 50.0f;
	//		//Vector3 stickL;
	//		AtkCollisionVec.x = g_pad[0]->GetLStickXF();
	//		AtkCollisionVec.y = g_pad[0]->GetLStickYF();
	//		//正規化
	//		AtkCollisionVec.Normalize();

	//		//Quaternion rot;
	//		////Y軸回りの回転クォータニオンを作成
	//		//rot.SetRotationYFromDirectionXZ(AtkCollisionVec);
	//		////ベクトルにクォータニオンを適応
	//		//rot.Apply(position);

	//		//コリジョンオブジェクトを作成する。
	//		AtkCollision = NewGO<CollisionObject>(0);
	//		Vector3 collitionPosition = position;
	//		collitionPosition.y += 50.0f;
	//		//collisionPosition.y += 50.0f;
	//		//ボックス状のコリジョンを作成する。
	//		AtkCollision->CreateBox(collitionPosition, //座標。
	//			Quaternion::Identity/*(rot)*/, //回転。
	//			Vector3(150.0f, 50.0f, 15.0f) //大きさ。
	//		);
	//		AtkCollision->SetIsEnableAutoDelete(false);
	//		AtkCollision->SetName("wizard_Attack");

	//		AtkCollisionSetFlag = true;
	//	}
	//	else
	//	{
	//		//移動速度設定
	//		AtkCollisionPos += AtkCollisionVec * 4.0f;
	//		//座標を設定
	//		collisionObject->SetPosition(AtkCollisionPos);

	//	}
	//}


	//void AvoidanceSprite();





private:
	Game* m_game = nullptr;
	Neutral_Enemy* m_Neutral_Enemy = nullptr; //中立の敵
	//MagicBall* magicBall = nullptr;

	CollisionObject* AtkCollision;                   //アタックのコリジョン
	Vector3 AtkCollisionPos = Vector3::Zero;
	Vector3 AtkCollisionVec = Vector3::Zero;
	//アタックのコリジョンを作成したかの判定
	bool AtkCollisionSetFlag = false;
	bool AttackFlag = false;
	float AttackTimer = 0.0f;


	//スキルを使った時の移動速度
	float SkillSpeed = 270.0f;
	//回避を使った時の移動速度
	float AvoidanceSpeed = 170.0f;

	bool m_spriteFlag = true;

	int exp = 5;

};

