#pragma once
#include "CameraCollisionSolver.h"
#include "SpringCamera.h"

class KnightPlayer;
class WizardPlayer;
class Player;
class Game;
class KnightUlt;
class Actor;
class WizardUlt;

class GameCamera:public IGameObject
{
public:
	GameCamera();
	~GameCamera();
	bool Start();
	void Update();
	void FollowThePlayer();
	void StateControl();
	void NomarlCamera();
	void UltRotCamera();
	void ChaseCamera();

	/// <summary>
	/// 画面分割時に画面のどちらを移すカメラか
	/// </summary>
	enum EnSplitCameraLR
	{
		enSplitCamera_Left,
		enSplitCamera_Right,
		enSplitCamera_Solo
	};

	enum EnCameraState {
		enNormalCameraState,
		enUltRotCameraState,
		enChaseCameraState
	};	

	//カメラのステート
	enum CameraState
	{
		enGameState,
		enPauseState,
	};

	enum PlayerNumber
	{
		enPlayerNumber_1P = 0,
		enPlayerNumber_2P = 1
	};

	/// <summary>
	/// カメラの視点を最初の状態に戻す
	/// </summary>
	void CameraTarget(float targrtYUp, float X,float Y,Actor*actor, bool cameraCollisionSolverFlag);

	/// <summary>
	/// 剣士が必殺技を打った時のカメラワーク
	/// </summary>
	void KnightUltCamera(Actor* actor, bool reset);

	/// <summary>
	/// 剣士の斬撃エフェクトを追いかける
	/// </summary>
	void ChaseUltEff();

	/// <summary>
	/// カメラを揺らす
	/// </summary>
	void CameraShake(bool UpDown);

	/// <summary>
	/// 雷が落ちた時に画面を揺らす
	/// </summary>
	void ThunderCameraShake();

	void ThunderCameraShakeForPlayer();

	void ChangeCameraShakeFlag(bool flag)
	{
		m_cameraShakeFlag = flag;
	}

	

	void SetKnight(KnightPlayer* knightplayer)
	{
		m_knightplayer = knightplayer;
	}

	/// <summary>
	/// カメラのステートを変更
	/// </summary>
	/// <param name="gamescene">変更したいステートの名前</param>
	void SetCameraState(CameraState gamescene) {
		if (this == nullptr) return;
		m_cameraState = gamescene;

	}

	void ChangeMoveCameraState(EnCameraState state) {
		m_enCameraState = state;
	}

	/// <summary>
	/// 画面分割時に画面のどちらを移すカメラかを設定する
	/// </summary>
	/// <param name="cameraLR"></param>
	void SetSplitCameraLR(EnSplitCameraLR cameraLR)
	{
		m_splitCameraLR = cameraLR;
	}

	/// <summary>
	/// 画面分割時に画面のどちらを移すカメラかを取得する
	/// </summary>
	/// <returns></returns>
	EnSplitCameraLR GetSplitCameraLR()
	{
		return m_splitCameraLR;
	}

	/// <summary>
	/// 剣士の必殺技のインスタンスを代入する
	/// </summary>
	/// <param name="ultobj">剣士の必殺技クラス</param>
	void SetKnightUlt(KnightUlt* ultobj)
	{
		knightUlt = ultobj;
	}

	void ChangeTunderCameraFlag(bool flag) {
		TunderCameraFlag = flag;
	}

	//bool UltTime(bool UltMoveFlag);

	// 必殺カメラの終了
	void GameCameraUltEnd();

	Vector3 GetCameraForword()
	{
		return newCamPos;
	}

	SpringCamera* GetSpringCamera() {
		return &m_springCamera;
	}

	void SetPlayerShakeFlag(bool flag)
	{
		PlayerShakeFlag = flag;
	}

	bool GetPlayerShakeFlag()
	{
		return PlayerShakeFlag;
	}

	void setShakeMulPower(int revel);

private:
	EnCameraState  m_enCameraState = enNormalCameraState;
	EnSplitCameraLR m_splitCameraLR = enSplitCamera_Solo;
	CameraState m_cameraState = enGameState;
	PlayerNumber m_playerNumber = enPlayerNumber_1P;

	CameraCollisionSolver	m_cameraCollisionSolver;
	SpringCamera			m_springCamera;

	KnightPlayer*			m_knightplayer	= nullptr;
	WizardPlayer* wizardPlayer = nullptr;
	Player* player = nullptr;
	Game* game = nullptr;

	WizardUlt* wizardUlt = nullptr;

	KnightUlt* knightUlt = nullptr;
	//必殺技を打ったキャラ
	Actor* ultactor = nullptr;
	//プレイヤー
	Actor* player_actor = nullptr;
	//必殺技を打たれたキャラ
	Actor* victim_actor = nullptr;
	
	Vector3					m_keepDiff				= Vector3::Zero;
	Vector3					m_CameraFromActorDiff   = Vector3::One;

	Vector3					m_toCameraPos			= Vector3::Zero;		//カメラ位置から注視点に向かうベクトル
	Vector3					m_position				= Vector3::Zero;		//カメラ座標
	Vector3					m_target				= Vector3::Zero;		//カメラ注視点
	Quaternion				m_rotation				= Quaternion::Identity;	//回転

	//注視点の計算
	Vector3 TargetPos = Vector3::Zero;
	//Quaternion oldgetRot;
	//カメラの最終的な視点
	Vector3 newCamPos = Vector3::Zero;

	bool PlayerCameraSet = false;

	bool ok = false;
	float rotamount = 0;

	//剣士のフラグ
	bool KnightUltFlag = false;
	bool SetCameraCharFrontFlag = false;
	//必殺技の溜めが始まったらtrue
	bool UltChargeFlag = false;

	float m_timer = 0.0f;
	const char* player_name = nullptr;

	float sita = 0.0f;

	bool m_cameraShakeFlag = false;
	bool m_ShakeMoveFlag = false;
	float m_cameraShakeTimer = 0.1f;

	///////////////////////////////////////
	//キャラが雷を打たれているなら
	bool TunderCameraFlag = false;
	//雷に打たれたキャラの名前を格納
	const char* Damege_actor_Name = nullptr;

	//false= true=
	bool ShakeLeftAndLightFlag = false;
	//////////////////////////////////////
	
	//前フレームのプレイヤーがリスポーン待機中かのフラグ
	bool PlayerRespawnFlag = false;

	//プレイヤーが必殺技を受けたときにカメラを揺らすフラグ
	bool PlayerShakeFlag = false;

	//アクターの情報
	std::vector<Actor*> m_actors;

	float shakePower = 0.0f;

	float shakeMulPower = 0.0f;
};

