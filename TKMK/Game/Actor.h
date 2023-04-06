#pragma once
#include "Status.h"
#include "Level3DRender.h"

class Actor:public IGameObject
{
public:
	Actor();
	virtual ~Actor();
	
protected:

	/// <summary>
	/// レベルアップ時に増加するステータス
	/// </summary>
	struct LvUpStatus {
		int LvHp;                  //レベルアップ時のヒットポイント
		int LvAtk;                 //レベルアップ時の攻撃力
		float LvSpeed;             //レベルアップ時の移動速度
	};

public:

	//ゲームクラスの現在の状態を示すステート
	enum EnGameState
	{
		enStart,
		enGame,
		enPause,
		enEnd
	};
	EnGameState m_GameState= enGame;

	/// <summary>
	/// 移動処理
	/// </summary>
	/// <param name="position">キャラクターの現在の座標</param>
	/// <param name="charcon">キャラコン</param>
	/// <param name="status">キャラのステータス</param>
	/// <param name="stickL">移動の入力量</param>
	void Move(Vector3& position,CharacterController& charcon, Status& status,Vector3 stickL);

	/// <summary>
	/// 移動処理したくないアニメーションのステートを書く
	/// </summary>
	/// <returns>移動処理しないアニメーションのステート</returns>
	virtual bool IsEnableMove()const = 0;

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
	/// 必殺技を打つ処理
	/// </summary>
	virtual void UltimateSkill() = 0;

	/// <summary>
	/// スキルが使用された後のクールタイムの処理
	/// </summary>
	/// <param name="SkillCooltimer">スキルのクールタイム</param>
	/// <param name="skillstate">スキルを使用したかの判定</param>
	/// <param name="timer">クールタイムを計算する変数</param>
	void COOlTIME(float SkillCooltimer,bool& skillstate,float& timer);
	
	/// <summary>
	/// 現在のレベルを返す　KnightBaseのを持ってくるto do
	/// </summary>
	/// <returns></returns>
	inline int& GetLevel() { return Lv; }

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
	/// <param name="lus">レベルアップ時に増加するステータス</param>
	/// <param name="nowStatus">現在のステータス</param>
	/// <param name="Level">現在のレベル</param>
	void levelDown(LvUpStatus& lus, Status& nowStatus, int& Level,int downLevel);

	/// <summary>
    /// リスポーンしたときのレベルによって経験値を変更する
    /// </summary>
    /// <param name="Lv">現在のレベル</param>
	/// <param name="getExp">経験値</param>
	void ExpReset(int& Lv, int& getExp);

	/// <summary>
    /// 経験値テーブルを変更する
    /// </summary>
    /// <param name="Lv">現在のレベル</param>
    /// <param name="expTable">経験値テーブル</param>
	void ExpTableChamge(int& Lv,int& expTable);

	/// <summary>
	/// ポイントを増やす処理
	/// </summary>
	/// <param name="point">現在のポイント</param>
	void PointProcess(int& point)
	{
		//ポイントを1増やす
		point++;
	}

	/// <summary>
	/// 死んだときの処理
	/// </summary>
	virtual void Death() = 0;

	/// <summary>
	/// リスポーンする座標を設定する
	/// </summary>
	/// <param name="pos">リスポーンする座標</param>
	/// <param name="number">座標をセットする番号</param>
	void SetRespawnPos(Vector3 pos,Quaternion rot, int number) {
		m_respawnPos[number] = pos;
		m_respawnRotation[number] = rot;
	};

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
/// knightPlayer
/// </summary>
/// <returns>false???\???????</returns>
	const bool GetSpriteFlag() const
	{
		return m_spriteFlag;
	}

	/// <summary>
	/// 引数に攻撃力を代入させる
	/// </summary>
	int GetAtk() {
		return m_Status.Atk;
	}

	/// <summary>
	/// 中立の敵を倒したときの経験値の処理
	/// </summary>
	/// <param name="GetExp">中立の敵の経験値</param>
	void ExpProcess(int Exp);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="gameState"></param>
	void ChangeGameState(EnGameState gameState)
	{
		m_GameState = gameState;
	}

	/// <summary>
	/// 現在のポイント返す
	/// </summary>
	int GetPoint() {
		return Point;
	}

private:
    Level3DRender m_respawnLevel;

	

protected:
	int Lv;                    //レベル
	int AtkSpeed;              //攻撃速度
	float Cooltime;            //スキルのクールタイム
	float AvoidanceCoolTime;    //回避のクールタイム
	bool SkillState = false;     //スキルが使用可能かどうかの判定
	int Point;                 //敵を倒して手に入れたポイント
	int GetExp;                //中立の敵を倒したときの経験値
	int ExpTable;              //経験値テーブル
	int respawnNumber;         //リスポーンする座標の番号
	bool isDeath = false;      //死んだかどうかの判定
	bool invincibleFlag = false;     //無敵時間フラグ
	int Ponit = 0;                  //獲得したポイント
	//Status m_Status;           //ステータス
	Vector3 m_respawnPos[4];    //リスポーンする座標の配列
	Quaternion m_respawnRotation[4];
	Vector3 m_moveSpeed = Vector3::Zero;      //移動量

	Vector3 m_Forward;          //プレイヤーの前方向

	Vector3 m_position = Vector3::Zero;                   //座標
	ModelRender m_modelRender;                            //モデルレンダー
	Quaternion m_rot = Quaternion::Identity;              //回転
	bool m_spriteFlag = true;
	Status m_Status;                                      //ステータス
	//レベルアップ時に増加するステータス
	LvUpStatus LvUPStatus = { 30,5,10.0f };
};

