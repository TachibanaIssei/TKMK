#pragma once
#include "KnightBase.h"

#include "Status.h"

class Game;
class Neutral_Enemy;
class KnightUlt;
class GameUI;

class KnightPlayer:public KnightBase
{
public:
	KnightPlayer();
	~KnightPlayer();
	void Update();

	void Attack();

	void Avoidance();
	void Render(RenderContext& rc);
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

	inline Vector3 GetPosition() { return m_position; }

	void AvoidanceSprite();

	/// <summary>
	/// 通常攻撃の当たり判定の処理
	/// </summary>
	void AtkCollisiton() {
		//コリジョンオブジェクトを作成する。
		auto collisionObject = NewGO<CollisionObject>(0);
		Vector3 collisionPosition = m_position;
		//座標をプレイヤーの少し前に設定する。
		//collisionPosition += forward * 50.0f;
		//ボックス状のコリジョンを作成する。
		collisionObject->CreateBox(collisionPosition, //座標。
			Quaternion::Identity, //回転。
			Vector3(70.0f, 15.0f, 15.0f) //大きさ。
		);
		collisionObject->SetName("player_attack");

		//「Sword」ボーンのワールド行列を取得する。
		Matrix matrix = m_modelRender.GetBone(m_swordBoneId)->GetWorldMatrix();

		//matrix.MakeRotationZ(90.0f);
		//「Sword」ボーンのワールド行列をコリジョンに適用する。
		collisionObject->SetWorldMatrix(matrix);
	}

	/// <summary>
/// レベル4で必殺技を使用したときの処理
/// </summary>
	void UltimateSkill()
	{
		//レベルを3下げる
		levelDown(LvUpStatus, m_Status, Lv, 3);
		//経験値をリセット
		ExpReset(Lv, GetExp);
		//レベルの経験値テーブルにする
		ExpTableChamge(Lv, ExpTable);

		////レベルに合わせてレベルの画像を変更する
		//m_gameUI->LevelFontChange(Lv);

		m_playerState = enKnightState_UltimateSkill;

	}

	/// <summary>
	/// 必殺技の当たり判定生成する
	/// </summary>
	void MakeUltSkill();

	/// <summary>
	/// スキルを発動したときに範囲内で一番近い敵をねらう処理
	/// </summary>
	//void SkillTarget()
	//{
	//	m_neutral_Enemys = FindGOs<Neutral_Enemy>("Neutral_Enemy");

	//	Vector3 nearPos = Vector3::Zero;
	//	//一番近い距離
	//	float Near = nearPos.Length();
	//	for (auto enemy : m_neutral_Enemys)
	//	{
	//		Vector3 toEnemy = enemy->GetPosition() - m_position;
	//		//エネミーとの距離を計算する
	//		float newNear = toEnemy.Length();
	//		//計算した距離が一番近い距離より小さいなら上書き
	//		if (Near > newNear) {
	//			Near = newNear;
	//		}
	//	}
	//	if (Near < 300) {

	//	}
	//}

	/// <summary>
	/// �摜�\���t���O��擾����
	/// </summary>
	/// <returns>false�Ȃ�\�����Ȃ�</returns>
	const bool GetSpriteFlag() const
	{
		return m_spriteFlag;
	}

	/// <summary>
	/// 座標を設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}

	/// <summary>
	/// 座標を取得
	/// </summary>
	/// <returns>座標</returns>
	const Vector3& GetPosition() const
	{
		return m_position;
	}

	/// <summary>
	/// 引数に攻撃力を代入させる
	/// </summary>
	/// <param name="attack">剣士の攻撃力を代入したい変数</param>
	/// <returns></returns>
	int SetKnightAtk(/*int& attack*/) {
		return m_Status.Atk;
	}

private:
	Game* m_game=nullptr;
	
	GameUI* m_gameUI = nullptr;
	
	//CollisionObject* collisionObject;                     //コリジョン
	Vector3 AnimEndPos = Vector3::Zero;
	Vector3 OldPos = Vector3::Zero;
	Vector3 UltPos = Vector3::Zero;

	Vector2 Avoidance_FlamePos = Vector2::Zero;
	Vector2 Avoidance_BarPos = Vector2::Zero;

	float UltimateSkillTimer = 0;

	enum AtkTimingState
	{
		FirstAtk_State,
		SecondAtk_State,
		SecondAtkStart_State,
		LastAtk_State,
		Num_State,

	};
	AtkTimingState m_AtkTmingState = Num_State;

	//bool AtkCollistionFlag = false;
	
	bool UltimateSkillFlag = false;
	FontRender Skillfont;
	SpriteRender m_Avoidance_flameRender;
	SpriteRender m_Avoidance_barRender;

	//スキルを使った時の移動速度
	float SkillSpeed = 270.0f;
	//回避を使った時の移動速度
	float AvoidanceSpeed = 170.0f;

	bool m_spriteFlag = true;

	//std::vector<Neutral_Enemy*> m_neutral_Enemys;
	Neutral_Enemy* m_Neutral_Enemy = nullptr; //中立の敵
};

