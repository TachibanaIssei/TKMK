#pragma once

#include "Level3DRender.h"

class Actor:public IGameObject
{
public:
	Actor();
	virtual ~Actor();

protected:

	/// <summary>
	/// 初期ステータス
	/// </summary>
	struct Status {
		int MaxHp;                 //最大ヒットポイント
		int Hp;                    //ヒットポイント(体力)
		int Atk;                   //攻撃力
		float Speed;               //移動速度
	};

	/// <summary>
	/// レベルアップ時に増加するステータス
	/// </summary>
	struct LvUpStatus {
		int LvHp;                  //レベルアップ時のヒットポイント
		int LvAtk;                 //レベルアップ時の攻撃力
		float LvSpeed;             //レベルアップ時の移動速度
	};

public:
	/// <summary>
	/// リスポーンする座標を取得する
	/// </summary>
	void GetRespawnPos();

	virtual void SetRespawn() = 0;

	/// <summary>
	/// 攻撃の処理,純粋仮想関数なので派生クラスで実装
	/// </summary>
	virtual void Attack() = 0;

	/// <summary>
	/// 攻撃を受けたときの処理
	/// </summary>
	/// <param name="damege">敵の攻撃力</param>
	virtual void Dameged(int damege) = 0;

	/// <summary>
	/// スキルを打つ処理
	/// </summary>
	virtual void Skill() = 0;

	/// <summary>
	/// 必殺技を打つ処理
	/// </summary>
	virtual void UltimateSkill() = 0;

	/// <summary>
	/// スキルが使用された後のクールタイムの処理
	/// </summary>
	/// <param name="SkillCooltimer">スキルのクールタイム</param>
	/// <param name="skillstate">スキルを使用したかの判定</param>
	void COOlTIME(float SkillCooltimer,bool skillstate);
	
	/// <summary>
	/// 現在のレベルを取得する関数
	/// </summary>
	/// <returns></returns>
	inline int GetLevel() { return Lv; }

	/// <summary>
	/// レベルアップの処理
	/// </summary>
	/// <param name="lus">レベルアップ時に増加するステータス</param>
	/// <param name="nowStatus">現在のステータス</param>
	/// <param name="Level">現在のレベル</param>
	void LevelUp(LvUpStatus& lus, Status& nowStatus,int& Level);

	/// <summary>
	/// レベルダウンの処理
	/// </summary>
	/// /// <param name="lus">レベルアップ時に増加するステータス</param>
	/// <param name="nowStatus">現在のステータス</param>
	/// <param name="Level">現在のレベル</param>
	void levelDown(LvUpStatus& lus, Status& nowStatus, int& Level);

	/// <summary>
	/// 死んだときの処理
	/// </summary>
	virtual void Death() = 0;

	/// <summary>
		/// リスポーンする座標を設定する
		/// </summary>
		/// <param name="pos">リスポーンする座標</param>
		/// <param name="number">座標をセットする番号</param>
	void SetRespawnPos(Vector3 pos, int number) {
		m_respawnPos[number] = pos;
	};

private:
    Level3DRender m_respawnLevel;

	float timer = 0.0f;

protected:
	int Lv;                    //レベル
	int AtkSpeed;              //攻撃速度
	float Cooltime;            //スキルのクールタイム
	bool SkillState = false;     //スキルが使用可能かどうかの判定
	int Point;                 //敵を倒して手に入れたポイント
	int GetExp;                //中立の敵を倒したときの経験値
	int ExpTable;              //経験値テーブル
	int respawnNumber;         //リスポーンする座標の番号
	bool isDeath = false;      //死んだかどうかの判定
	Vector3 m_respawnPos[4];    //リスポーンする座標の配列


};

