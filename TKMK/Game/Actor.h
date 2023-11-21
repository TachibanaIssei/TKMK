#pragma once
#include "Status.h"
#include "Level3DRender.h"
#include "Effect.h"
#include "ChaseEFK.h"
#include "Sounds.h"
#include "Game.h"

class Neutral_Enemy;
class Player;
class Fade;
class Game;

class Actor :public IGameObject
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

	enum EnPlayerNumber
	{
		enPlayerNumber_1P,
		enPlayerNumber_2P,
		enPlayerNumber_3P,
		enPlayerNumber_4P,
		enPlayerNumber_Num,
		enPlayerNumber_AI
	};

	// 必殺技の終了
	void UltSkillEnd() {
		m_charState = enCharState_Idle;
	}

	// 必殺技終了用の純粋仮想関数
	virtual void UltEnd() = 0;

	virtual void ChaseEffectDelete() = 0;
	void OnDestroy() override;
protected:
	//void AttackUP();

	/*void AttackUPEnd() {
		m_status.m_attackPower -= PowerUp;

		if (PowerUpEfk != nullptr)
		{
			PowerUpEfk->DeleteEffect();
			DeleteGO(PowerUpEfk);
			PowerUpEfk = nullptr;
		}
	}*/

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

	EnCharState GetCharState()
	{
		return m_charState;
	}
	//ゲームクラスの現在の状態を示すステート
	enum EnGameState
	{
		enStart,
		enGame,
		enPause,
		enEnd
	};
	EnGameState m_GameState = enGame;

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
	/// <param name="number">プレイヤーの番号</param>
	void Move(Vector3& position, CharacterController& charcon, Status& status, Vector3 stickL, int number);

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
	virtual void Dameged(int damege, Actor* CharGivePoints) = 0;

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
	void CoolTime(float m_skillCoolTime, bool& skillEndFlag, float& timer);

	/// <summary>
	/// 現在のレベルを返す
	/// </summary>
	/// <returns></returns>
	inline int& GetLevel() { return m_lv; }

	/// <summary>
	/// レベルアップの処理
	/// </summary>
	/// <param name="lus">レベルアップ時に増加するステータス</param>
	/// <param name="nowStatus">現在のステータス</param>
	/// <param name="Level">現在のレベル</param>
	void LevelUp(int& Level);

	/// <summary>
	/// レベルダウンの処理
	/// </summary>
	/// <param name="lus">レベルアップ時に増加するステータス</param>
	/// <param name="nowStatus">現在のステータス</param>
	/// <param name="Level">現在のレベル</param>
	void levelDown(int& Level, int downLevel);

	/// <summary>
	/// リスポーンしたときのレベルによって経験値を変更する
	/// </summary>
	/// <param name="m_lv">現在のレベル</param>
	/// <param name="getExp">経験値</param>
	void ExpReset(int& m_lv, int& getExp);

	/// <summary>
	/// 経験値テーブルを変更する
	/// </summary>
	/// <param name="m_lv">現在のレベル</param>
	/// <param name="expTable">経験値テーブル</param>
	void ExpTableChamge(int& m_lv, int& expTable);

	void LevelDownProcess(int downlevel);

	/// <summary>
	/// ポイントを増やす
	/// </summary>
	/// <param name="point">倒された相手の現在のポイント</param>
	/// <param name="level">自分の現在のレベル</param>
	void PointProcess(int level)
	{
		int AddPoint = level;
		//レベルが5以上なら
		if (AddPoint > 5)
		{
			AddPoint = 5;
		}
		/*else
		{
			AddPoint = level;
		}*/

		if (m_game->GetMinutesTimer() == 0)
		{
			int Rank = m_game->GetNowRank(this);
			if (Rank == 2)
			{
				AddPoint = (int)((float)AddPoint * 1.2f);
			}
			else if (Rank == 3)
			{
				AddPoint = (int)((float)AddPoint * 1.5f);
			}
			else if (Rank == 4)
			{
				AddPoint = (int)((float)AddPoint * 2.0f);
			}
		}
		//レベル分ポイントを増やす
		m_point += AddPoint;
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
	void SetRespawnPos(Vector3 pos, Quaternion rot, int number) {
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

	Quaternion& GetRotation()
	{
		return m_rotation;
	}
	/// <summary>
	/// 攻撃力を返す
	/// </summary>
	int GetAtk() const
	{
		return m_status.GetAttackPower();
	}

	/// <summary>
	/// HPを返す
	/// </summary>
	int GetHP() const
	{
		return m_status.GetHp();
	}

	/// <summary>
	/// HP回復させる処理
	/// </summary>
	/// <param name="HpUp">HPの回復量</param>
	void HpUp(int HpUp)
	{
		int hp = m_status.GetHp();
		hp += HpUp;
		m_status.SetHp(hp);
		// エフェクトがあるなら消す
		if (GetHoimi != nullptr) {
			GetHoimi->DeleteEffect();
		}

		GetHoimi = NewGO<ChaseEFK>(3);
		GetHoimi->SetEffect(EnEFK::enEffect_Knight_GetHoimi, this, Vector3::One * 30.0f);
		//回復したあとのHPが現在のレベルの最大ヒットポイントより大きかったら
		if (m_status.GetHp() > m_status.GetMaxHp())
		{
			m_status.SetHp(GetMaxHp());
		}
	}

	/// <summary>
	/// HPの上限を超えた時上限までに戻す処理
	/// </summary>
	/// /// <param name="HpReset">MaxHpの値を入れる</param>	
	void HpReset(int HpReset)
	{
		m_status.SetHp(HpReset);
	}

	//MaxHpを渡す
	int GetMaxHp()const { return m_status.GetMaxHp(); };
	//今のHpを渡す
	int GetHp()const { return m_status.GetHp(); };

	/// <summary>
	/// 中立の敵を倒したときの経験値の処理
	/// </summary>
	/// <param name="m_getExp">中立の敵の経験値</param>
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
	int GetPoint()const
	{
		return m_point;
	}

	/// <summary>
	/// ステータスを返す
	/// </summary>
	Status& GetStatus()
	{
		return m_status;
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
	int GetExperience()const
	{
		return m_getExp;
	}

	/// <summary>
	/// 現在の経験値テーブルを返す
	/// </summary>
	/// <returns></returns>
	int GetExpTable()const
	{
		return m_expTable;
	}

	int GetSaveEXP() const
	{
		return m_saveEXP;
	}

	void SetSaveEXP(int num)
	{
		m_saveEXP = num;
	}


	/// <summary>
	/// レベルに応じた経験値テーブルを返す
	/// </summary>
	/// <param name="Level"></param>
	/// <returns></returns>
	int GetExpTableForLevel(int Level)
	{
		if (Level >= 3) {
			return 10;
		}
		else
		{
			return 5;
		}
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
		Vector3 m_center = Vector3::One;
		Vector3 biff = m_center - m_position;
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
	/// やられたときからリスポーンするまでのフラグ
	/// </summary>
	/// <returns></returns>
	bool GetRespawnFlag() const
	{
		return m_DeathToRespwanFlag;
	}

	virtual void SetRespawnNumber(int number) = 0;

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
	bool DeathToRespawnTimer(bool& DeathToRespwanFlag, Fade* fade, bool fadeFlag);

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

	/// <summary>
	/// 地上に降りているかのフラグ
	/// </summary>
	/// <returns></returns>
	bool GetIsGroundFlag()const
	{
		return IsGroundFlag;
	}

	/// <summary>
	/// スキルのクールタイムのタイマーを返す
	/// </summary>
	float GetSkillTimer()
	{
		return m_skillTimer;
	}

	/// <summary>
	/// 必殺技の攻撃対象のアクターがいないかのフラグを返す
	/// </summary>
	bool GetNoTargetActor()
	{
		return m_NoTargetActor;
	}

	/// <summary>
	/// 自身をカメラで見ているかのフラグを変える AI用
	/// </summary>
	/// <param name=""></param>
	void ChangeChaseCamera(bool flag)
	{
		ChaseCameraFlag = flag;
	}

	bool GetChaseCameraFlag()
	{
		return ChaseCameraFlag;
	}

	/// <summary>
	/// 必殺技で攻撃対象のアクターを返す
	/// </summary>
	/// <returns></returns>
	std::vector<Actor*>& GetDamegeUltActor() {
		return DamegeUltActor;
	}

	/// <summary>
	/// 必殺技を食らったアクターのデータをリストから削除する
	/// </summary>
	/// <param name="">必殺技を食らったアクター</param>
	/// <returns></returns>
	void EraseDamegeUltActor(Actor* targetActor) {
		std::vector<Actor*>::iterator it = std::find(
			DamegeUltActor.begin(), // アクターのリストの最初
			DamegeUltActor.end(),   // アクターのリストの最後
			targetActor                     // 消したいアクター
		);
		if (*it != nullptr) {
			DamegeUltActor.erase(it);
		}
	}

	void  DamegeUltActorClear() {
		DamegeUltActor.clear();
	}

	//必殺技を打ったかどうかのフラグを返す
	//打っている間はtrue
	bool GetmUseUltimaitSkillFlag()
	{
		return m_UseUltimaitSkillFlag;
	}

	/// <summary>
	/// 必殺技を打って画面を暗くするかのフラグを返す
	/// </summary>
	/// <returns></returns>
	bool GetUltimateDarknessFlag() const
	{
		return UltimateDarknessFlag;
	}

	void SetDarwFlag(bool flag)
	{
		DarwFlag = flag;
	}

	bool IsActorGroundChack();

private:
	Level3DRender m_respawnLevel;


protected:
	int									m_lv = 1;						//レベル
	int									m_attackSpeed = 0;				//攻撃速度
	float								m_skillCoolTime = 0;			//スキルのクールタイム
	float								m_avoidanceCoolTime = 0;		//回避のクールタイム
	bool								m_isSkillReady = false;			//スキルが使用可能かどうかの判定
	int									m_point = 0;					//敵を倒して手に入れたポイント
	int									m_getExp = 0;					//中立の敵を倒したときの経験値
	int									m_expTable = 0;					//経験値テーブル
	int									m_oldExpTable = 0;				//前のレベルの経験値テーブル
	int									m_respawnNumber;				//リスポーンする座標の番号
	bool								m_invincibleFlag = false;		//無敵時間フラグ
	int									m_saveEXP = 0;					//UIに表示するために経験値を保存する
	float								m_skillTimer = 0;				//スキルのクールタイムを計算するタイマー
	float								m_avoidanceTimer = 0;			//回避のクールタイムを計算するタイマー
	float								m_invincibleTimer = 0;			//無敵時間を計算するタイマー
	bool								m_spriteFlag = true;

	EnPlayerNumber	m_enPlayerNumber = enPlayerNumber_AI;
	Vector3			m_respawnPos[4];						//リスポーンする座標の配列
	Quaternion		m_respawnRotation[4];
	Vector3			m_moveSpeed = Vector3::Zero;			//移動量

	Vector3			m_Forward = Vector3::Zero;			    //プレイヤーの前方向
	Vector3			m_forwardNow = Vector3::Zero;           //現在の正面ベクトル

	Vector3			m_position = Vector3::Zero;             //座標
	ModelRender		m_modelRender;                          //モデルレンダー
	Quaternion		m_rotation = Quaternion::Identity;      //回転
	Status			m_status;                               //ステータス
	Status			m_InitialStatus;                        //初期ステータス
	
	LvUpStatus LvUPStatus = { 30,5,5 };		//レベルアップ時に増加するステータス

	//スポーン、リスポーンして塔から地上に降りたか
	//空中にいるかの判定
	bool m_AirFlag = false;
	//ジャンプフラグ
	bool m_RespawnJumpFlag = false;

	//自分をターゲットしてるアクターのリスト
	std::vector<Actor*> be_target;

	EnAIorPlayer m_AIorPlayer = enAI;

	int Count = 0;

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
	Game* m_game = nullptr;
	Actor* m_targetActor = nullptr;
	Actor* m_escapeActor = nullptr;					// 今逃げているアクター
	Actor* m_escapeActorBackup = nullptr;			// 今逃げているアクター（逃げタイマー用）
	Fade* m_fade = nullptr;

	Neutral_Enemy* m_targetEnemy = nullptr;			// 今追いかけているエネミー     
	std::vector<Actor*> DamegeUltActor;             //必殺技で攻撃対象のアクター

	ChaseEFK* GetHoimi = nullptr;
	ChaseEFK* LevelUp_efk = nullptr;
	ChaseEFK* LevelDown_efk = nullptr;

	RigidBody				m_rigidBody;						//剛体。
	BoxCollider			m_boxCollider;							//コライダー。

	float PowerUpTimer = 0.0f;
	int PowerUp = 0;

	//自身を描画するかのフラグ trueのとき描画
	bool DarwFlag = true;

	//必殺技を打った時に画面を暗くするするフラグ
	bool UltimateDarknessFlag = false;
	//必殺技使用時にカメラで見ている間true AI用
	bool ChaseCameraFlag = false;
	////////////////////////////////////////////////
	// 雷を打つときに使う変数
	////////////////////////////////////////////////
	//必殺技を使ったときに立てるフラグ(使用者のフラグ)
	bool m_UseUltimaitSkillFlag = false;
	//必殺技を打たれたときに立てるフラグ(被害者のフラグ)
	bool m_DamegeUltimaitSkillaFlag = false;
	//グラウンドに降りているかチェックするフラグ
	//bool m_GroundChackFlag = false;
	bool m_NoTargetActor = false;
	//必殺技を打つ間隔を計るタイマー
	float m_UltshootTimer = 0.9f;

	//地上に降りたらtrueにする
	bool IsGroundFlag = false;
	////////////////////////////////////////////////
};

