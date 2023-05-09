#pragma once
#include "Status.h"
#include "Level3DRender.h"
#include "Effect.h"
#include "ChaseEFK.h"

class Neutral_Enemy;
class Player;
class Fade;

class Actor:public IGameObject
{
public:
	Actor();
	virtual ~Actor();
	
	//キャラの状態
	enum EnCharState
	{
		enCharState_Idle,
		enCharState_Walk,
		enCharState_Run,
		enCharState_Attack,
		enCharState_SecondAttack,
		enCharState_LastAttack,
		enCharState_Damege,
		enCharState_Death,
		enCharState_Skill,
		enCharState_Ult_liberation,
		enCharState_UltimateSkill,
		enCharState_Avoidance,
		enCharState_Pause,
		enCharState_Jump,
		enCharState_Fall,
		enCharState_Num,
	};

	// 必殺技の終了
	void UltSkillEnd() {
		m_charState = enCharState_Idle;
	}

	// 必殺技終了用の純粋仮想関数
	virtual void UltEnd() = 0;

protected:
	void AttackUP();

	void AttackUPEnd() {
		m_Status.Atk -= PowerUp;

		if (PowerUpEfk != nullptr)
		{
			PowerUpEfk->DeleteEffect();
			DeleteGO(PowerUpEfk);
			PowerUpEfk = nullptr;
		}
	}

	/// <summary>
	/// レベルアップ時に増加するステータス
	/// </summary>
	struct LvUpStatus {
		int LvHp;                  //レベルアップ時のヒットポイント
		int LvAtk;                 //レベルアップ時の攻撃力
		float LvSpeed;             //レベルアップ時の移動速度
	};

	enum EnJumpState
	{
		enIsGround,
		enIsAir
	};
	EnJumpState m_JumpState;

	EnCharState m_charState = enCharState_Idle;

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

	//AIかプレイヤーか
	enum EnAIorPlayer
	{
		enAI,
		enPlayer
	};
	void SetAIorPlayer(EnAIorPlayer aiorplayer)
	{
		m_AIorPlayer = aiorplayer;
	}
	EnAIorPlayer GetAIorPlayer()
	{
		return m_AIorPlayer;
	}

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
	/// ダメージを受けたときの処理
	/// </summary>
	/// <param name="damege">敵の攻撃力</param>
	/// <param name="playerGivePoints">ポイントを与えるキャラ</param>
	virtual void Dameged(int damege,Actor* CharGivePoints) = 0;

	/// <summary>
	/// 必殺技を打つ処理
	/// </summary>
	virtual void UltimateSkill() = 0;

	/// <summary>
	/// クールタイムの処理
	/// </summary>
	/// <param name="SkillCooltimer">クールタイム</param>
	/// <param name="skillstate">スキルや回避が終わったかの判定</param>
	/// <param name="timer">クールタイムを計算する変数</param>
	void COOlTIME(float Cooltime, bool& skillEndFlag, float& timer);
	
	/// <summary>
	/// 現在のレベルを返す
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
	/// ポイントを増やす
	/// </summary>
	/// <param name="point">倒された相手の現在のポイント</param>
	/// <param name="level">自分の現在のレベル</param>
	void PointProcess(int& level)
	{
		//レベルが5以上なら
		if (level > 5)
		{
			level = 5;
		}
		int AddPoint = level;
		//レベル分ポイントを増やす
		Point += AddPoint;
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
	/// リスポーンしたときに塔から飛び降りる処理
	/// </summary>
	void RespawnMove();

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
	/// プレイヤーの前方向を取得。
	/// </summary>
	const Vector3& GetForward() const
	{
		return m_forwardNow;
	}

	/// <summary>
	/// m_spriteFlagを返す
	/// </summary>
	/// <returns>判定</returns>
	const bool GetSpriteFlag() const
	{
		return m_spriteFlag;
	}

	/// <summary>
	/// 攻撃力を返す
	/// </summary>
	int GetAtk() {
		return m_Status.Atk;
	}

	/// <summary>
	/// HPを返す
	/// </summary>
	int GetHP()
	{
		return m_Status.Hp;
	}

	/// <summary>
	/// 中立の赤色の敵を倒した時攻撃を上げる処理
	/// </summary>
	/// <param name="AtkUp">増加する攻撃力</param>
	void AtkUp(int AtkUp)
	{
		//パワーUPじゃないとき
		if (PowerUpTimer <= 0.0f)
		{
			PowerUp = AtkUp;
			m_Status.Atk += PowerUp;

			PowerUpEfk  = NewGO<ChaseEFK>(3);
			PowerUpEfk->SetEffect(EnEFK::enEffect_Knight_PowerUP, this, Vector3::One * 15.0f);

			PowerUpEfk->AutoDelete(false);
			PowerUpEfk->GetEffect()->AutoDelete(false);
		}		
		PowerUpTimer = 15.0f;
	}


	/// <summary>
	/// HP回復させる処理
	/// </summary>
	/// <param name="HpUp">HPの回復量</param>
	void HpUp(int HpUp)
	{
		m_Status.Hp += HpUp;
		GetHoimi = NewGO<ChaseEFK>(3);
		GetHoimi->SetEffect(EnEFK::enEffect_Knight_GetHoimi, this, Vector3::One * 30.0f);
		//回復したあとのHPが現在のレベルの最大ヒットポイントより大きかったら
		if (m_Status.Hp > m_Status.MaxHp)
		{
			m_Status.Hp = m_Status.MaxHp;
		}
	}

	/// <summary>
	/// HPの上限を超えた時上限までに戻す処理
	/// </summary>
	/// /// <param name="HpReset">MaxHpの値を入れる</param>	
	void HpReset(int HpReset)
	{
		m_Status.Hp = HpReset;
	}

	//MaxHpを渡す
	int GetMaxHp() { return m_Status.MaxHp; };
	//今のHpを渡す
	int GetHp() { return m_Status.Hp; };

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

	/// <summary>
	/// ステータスを返す
	/// </summary>
	Status& GetStatus()
	{
		return m_Status;
	}

	/// <summary>
	/// レベルアップ時に増加するステータスを返す
	/// </summary>
	LvUpStatus& GetLvUPStatus()
	{
		return LvUPStatus;
	}

	/// <summary>
	/// 現在の経験値を返す
	/// </summary>
	/// <returns></returns>
	int GetExperience()
	{
		return GetExp;
	}

	/// <summary>
	/// 現在の経験値テーブルを返す
	/// </summary>
	/// <returns></returns>
	int GetExpTable()
	{
		return ExpTable;
	}

	/// <summary>
	/// 前のレベルのの経験値テーブルを返す
	/// </summary>
	/// <returns></returns>
	int GetOldExpTable()
	{
		return m_oldExpTable;
	}

	/// <summary>
	/// キャラコンが空中にあるか判定する
	/// </summary>
	/// <param name="">キャラコン</param>
	/// <returns>空中にあるか判定したフラグ</returns>
	EnJumpState IsAir(CharacterController& charcon)
	{
		if (charcon.IsJump())
		{
			m_JumpState = enIsAir;
		}
		else
		{
			m_JumpState = enIsGround;
		}
		return m_JumpState;
	}

	/// <summary>
	/// リスポーン時に前方向を設定する(カメラの座標を決めるときに使う)
	/// </summary>
	void ForwardSet()
	{
		Vector3 center = Vector3::One;
		Vector3 biff = center - m_position;
		biff.y = 0.0f;
		biff *= -1.0f;
		m_Forward = biff;
		m_forwardNow = biff;
	}

	/// <summary>
	/// キャラの現在のステート
	/// </summary>
	/// <returns></returns>
	EnCharState NowCharState() const
	{
		return m_charState;
	}

	/// <summary>
	/// やられたときの
	/// </summary>
	/// <returns></returns>
	bool RespawnFlag() const
	{
		return m_DeathToRespwanFlag;
  }

	virtual void SetRespawnNumber(int number)=0;

	/// <summary>
	/// アクターの情報を追加する
	/// </summary>
	/// <param name="actor">加えたいアクター</param>
	void AddActorFromList(Actor* actor) {
		be_target.push_back(actor);
	}

	/// <summary>
	/// アクターの情報をリストから削除する
	/// </summary>
	/// <param name="actor">消したいアクター</param>
	void RemoveActorFromList(Actor* actor)
	{
		std::vector<Actor*>::iterator it = std::find(
			be_target.begin(), // アクターのリストの最初
			be_target.end(),   // アクターのリストの最後
			actor                     // 消したいアクター
		);

		if (it != be_target.end()) {
			be_target.erase(it);
		}
	}

	/// <summary>
	/// 中立の敵が死亡したときに呼ばれる
	/// 今狙っている敵が死亡した場合
	/// </summary>
	/// <param name="enemy"></param>
	void ClearEnemyTarget(Neutral_Enemy* enemy) {

		// 狙っている敵が死んだ！
		if (m_targetEnemy == enemy) {
			m_targetEnemy = nullptr;
			//評価値を再計算する
			EvalTimer = 0.0f;
		}
	}

	/// <summary>
	/// キャラがやられてからリスポーンするまでの時間を計る
	/// </summary>
	/// <param name="DeathToRespwanFlag"></param>
	bool DeathToRespawnTimer(bool DeathToRespwanFlag,Fade* fade);

	/// <summary>
	/// リスポーン待機時間を返す
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	float GetRespawnTimer()const
	{
		return m_respwanTimer;
	}

	float SoundSet(Player* player, float Max, float Min);

	/// <summary>
	/// 効果音を上げたり下げたりする
	/// </summary>
	void SetSE(float addSE)
	{
		//SEVolume += addSE;
		MaxVolume += addSE;
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
	int m_oldExpTable = 0;         //前のレベルの経験値テーブル
	int respawnNumber;         //リスポーンする座標の番号
	bool isDeath = false;      //死んだかどうかの判定
	bool invincibleFlag = false;     //無敵時間フラグ
	int Ponit = 0;                  //獲得したポイント
	//Status m_Status;           //ステータス
	Vector3 m_respawnPos[4];    //リスポーンする座標の配列
	Quaternion m_respawnRotation[4];
	Vector3 m_moveSpeed = Vector3::Zero;      //移動量

	Vector3 m_Forward;          //プレイヤーの前方向
	Vector3 m_forwardNow = Vector3::Zero;                 //現在の正面ベクトル

	Vector3 m_position = Vector3::Zero;                   //座標
	ModelRender m_modelRender;                            //モデルレンダー
	Quaternion m_rot = Quaternion::Identity;              //回転
	bool m_spriteFlag = true;
	Status m_Status;                                      //ステータス
	Status m_InitialStatus;                                //初期ステータス
	//レベルアップ時に増加するステータス
	LvUpStatus LvUPStatus = { 30,5,20.0f };

	//スポーン、リスポーンして塔から地上に降りたか
	//空中にいるかの判定
	bool m_AirFlag = false;
	//ジャンプフラグ
	bool m_RespawnJumpFlag = false;

	//自分をターゲットしてるアクターのリスト
	std::vector<Actor*> be_target;
	
	EnAIorPlayer m_AIorPlayer = enAI;

	int Count=0;

	//やられた後のリスポーンするまで時間を計る処理をするかのフラグ
	//falseでしない、trueでする
	bool m_DeathToRespwanFlag = false;
	//やられた後にもう一度復帰するまでの時間2
	float m_respwanTimer = 0.0f;
	///////////////////////////////
	//////ここから下はAI専用///////
	///////////////////////////////
	//評価値計算のタイマー
	float EvalTimer = 0.0f;

	//効果音
	float SEVolume = 0.0f;
	float MaxVolume = 2.0f;
	const float MinVolume = 0.0f;

	Player* m_player = nullptr;
	Actor* m_targetActor = nullptr;
	Actor* m_escapeActor = nullptr;					// 今逃げているアクター
	Actor* m_escapeActorBackup = nullptr;			// 今逃げているアクター（逃げタイマー用）
	Fade* m_fade = nullptr;

	Neutral_Enemy* m_targetEnemy = nullptr;			// 今追いかけているエネミー     

	ChaseEFK* PowerUpEfk = nullptr;
	ChaseEFK* GetHoimi = nullptr;
	float PowerUpTimer = 0.0f;
	int PowerUp = 0;

};

