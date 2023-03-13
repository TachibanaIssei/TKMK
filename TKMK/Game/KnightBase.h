#pragma once

#include "Actor.h"


//class Status;
class Game;
class GameUI;

class KnightBase:public Actor
{
public:
	KnightBase();
	virtual ~KnightBase();

	/// <summary>
	/// ƒ‚ƒfƒ‹‚ÌInitAƒLƒƒƒ‰ƒRƒ“‚Ì‰Šú‰»
	/// </summary>
	/// <param name="Model"></param>
	/// <param name="charCon"></param>
	/// bool Start()
	void SetModel();

	/// <summary>
	/// ’†—§‚Ì“G‚ğ“|‚µ‚½‚Æ‚«‚ÌŒoŒ±’l‚Ìˆ—
	/// </summary>
	/// <param name="GetExp">’†—§‚Ì“G‚ÌŒoŒ±’l</param>
	void ExpProcess(int Exp);

	/// <summary>
	/// 
	/// </summary>
	virtual void Attack()=0;

	/// <summary>
	/// ©g‚Ì“–‚½‚è”»’è
	/// </summary>
	void Collition();

	/// <summary>
	/// ƒ_ƒ[ƒW‚ğó‚¯‚½‚Æ‚«‚Ìˆ—
	/// </summary>
	/// <param name="damege">“G‚ÌUŒ‚—Í</param>
	void Dameged(int damege);

	/// <summary>
	/// ©g‚ª“|‚³‚ê‚½‚Æ‚«‚Ìˆ—
	/// </summary>
	void Death();

	/// <summary>
	/// ƒXƒLƒ‹‚ğg—p‚µ‚½‚Æ‚«‚Ìˆ—
	/// </summary>
	//void Skill(Vector3& right,Vector3& forward);

	/// <summary>
	/// •KE‹Z‚ğ”­“®‚µ‚½‚Æ‚«‚Ìˆ—
	/// </summary>
	void UltimateSkill();

	/// <summary>
	/// ƒŠƒXƒ|[ƒ“‚·‚éÀ•W‚ÌƒZƒbƒg
	/// </summary>
	void SetRespawn();

	/// <summary>
	/// ©g‚ª“|‚³‚ê‚½‚Æ‚«‚Ìˆ—
	/// </summary>
	void Rotation();

	/// <summary>
	/// ’ÊíUŒ‚‚Ì“–‚½‚è”»’è‚Ìˆ—
	/// </summary>
	void AtkCollisiton();

	/// <summary>
	/// •KE‹Z‚Ì“–‚½‚è”»’è‚Ìˆ—
	/// </summary>
	void UltimateSkillCollistion(Vector3& oldpostion, Vector3& position);

	/// <summary>
	/// ƒAƒjƒ[ƒVƒ‡ƒ“‚ÌƒXƒe[ƒg‚ÌŠÇ—
	/// </summary>
	void ManageState();

	/// <summary>
	/// 
	/// </summary>
	void AnimationMove();

	/// <summary>
	/// 
	/// </summary>
	/// <param name="right"></param>
	/// <param name="forward"></param>
	void MoveStraight(Vector3& right, Vector3& forward);

	/// <summary>
	/// ƒAƒjƒ[ƒVƒ‡ƒ“ƒCƒxƒ“ƒg
	/// </summary>
	/// <param name="clipName"></param>
	/// <param name="eventName"></param>
	virtual void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)=0;

	/// <summary>
	/// À•W‚ÌƒZƒbƒg
	/// </summary>
	/// <param name="PS"></param>
	inline void SetPosition(Vector3 PS) { m_position = PS; }

	/// <summary>
	/// Œ•m‚ÌÀ•W‚ğ•Ô‚è’l‚Æ‚µ‚Ä•Ô‚·
	/// </summary>
	virtual Vector3 GetPosition() = 0;

	void SetSGame(Game* Cgame)
	{
		m_game = Cgame;
	}
	Game* GetSGame()
	{
		return m_game;
	}
	//
	void SetGameUI(GameUI* gameUI)
	{
		m_gameUI = gameUI;
	}
	GameUI* GetSGameUI()
	{
		return m_gameUI;
	}

	/// <summary>
	/// “Á’è‚ÌƒAƒjƒ[ƒVƒ‡ƒ“‚ªÄ¶’†‚È‚çfalse‚ğ•Ô‚·
	/// </summary>
	/// <returns></returns>
	bool IsEnableMove() const
	{
		return m_playerState != enKnightState_ChainAtk &&
			m_playerState != enKnightState_UltimateSkill &&
			m_playerState != enKnightState_Skill &&
			m_playerState != enKnightState_Avoidance &&
			m_playerState != enKnightState_Damege&&
			m_playerState != enKnightState_Death;
	}

	/// <summary>
	/// Œ»İ‚ÌƒŒƒxƒ‹‚ğ•Ô‚·
	/// </summary>
	/// <returns>Œ»İ‚ÌƒŒƒxƒ‹</returns>
	int& SetLevel(){
		return Lv;
	}

	/// <summary>
	/// Œ»İ‚Ìƒqƒbƒgƒ|ƒCƒ“ƒg‚ğ•Ô‚·
	/// </summary>
	/// <returns>Œ»İ‚ÌHP</returns>
	int& SetHp() {
		return m_Status.Hp;
	}

	/// <summary>
	/// Œ»İ‚Ìƒ}ƒbƒNƒXƒqƒbƒgƒ|ƒCƒ“ƒg‚ğ•Ô‚·
	/// </summary>
	/// <returns></returns>
	int& SetMaxHp() {
		return m_Status.MaxHp;
	}

	/// <summary>
	/// ï¿½vï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½[ï¿½ÌƒXï¿½eï¿½[ï¿½gï¿½ï¿½ÏX
	/// </summary>
	/// <param name="gamescene">ï¿½ÏXï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Xï¿½eï¿½[ï¿½gï¿½Ì–ï¿½ï¿½O</param>
	void SetPlayerState(PlayerState gamescene) {
		m_playerState = gamescene;
	}

protected:
	void PlayAnimation();
	//‹¤’Ê‚ÌƒXƒe[ƒg‚Ì‘JˆÚˆ—
	void OnProcessCommonStateTransition();
	//‘Ò‹@‚ÌƒXƒe[ƒg‚Ì‘JˆÚˆ—
	void OnProcessIdleStateTransition();
	//•à‚«‚ÌƒXƒe[ƒg‚Ì‘JˆÚˆ—
	void OnProcessRunStateTransition();
	//ƒ`ƒFƒCƒ“ƒAƒ^ƒbƒN‚ÌƒXƒe[ƒg‚Ì‘JˆÚˆ—
	void OnProcessChainAtkStateTransition();
	//ƒXƒLƒ‹‚ÌƒXƒe[ƒg‚Ì‘JˆÚˆ—
	void OnProcessSkillAtkStateTransition();
	//‰ñ”ğ‚ÌƒXƒe[ƒg‚Ì‘JˆÚˆ—
	void OnProcessAvoidanceStateTransition();
	//•KE‹Z‚ÌƒXƒe[ƒg‚Ì‘JˆÚˆ—
	void OnProcessUltimateSkillAtkStateTransition();
	//ƒ_ƒ[ƒW‚ğó‚¯‚½‚Æ‚«‚ÌƒXƒe[ƒg‚Ì‘JˆÚˆ—
	void OnProcessDamegeStateTransition();
	//HP‚ª0‚É‚È‚Á‚½‚Æ‚«‚ÌƒXƒe[ƒg‚Ì‘JˆÚˆ—
	void OnProcessDeathStateTransition();

	enum PlayerState {
		enKnightState_Idle,
		enKnightState_Run,
		enKnightState_ChainAtk,
		enKnightState_Damege,
		enKnightState_Death,
		enKnightState_Skill,
		enKnightState_UltimateSkill,
		enKnightState_Avoidance,
		enKnightState_Num,
		enKnightState_GameScene,        //ƒQ[ƒ€‚Ìó‘Ô‚ğó‚¯æ‚é
	};
	enum EnAnimationClip {
		enAnimationClip_Idle,
		enAnimationClip_Run,
		enAnimationClip_ChainAtk,
		enAnimationClip_Damege,
		enAnimationClip_Death,
		enAnimationClip_Skill,
		enAnimationClip_UltimateSkill,
		enAnimationClip_Avoidance,
		enAnimationClip_Num,
	};

	Game* m_game=nullptr;
	GameUI* m_gameUI = nullptr;

	//‰ŠúƒXƒe[ƒ^ƒX Å‘åHPAHPAUŒ‚—ÍAƒXƒs[ƒh
	Status m_Status;

	Vector3 firstposition;                                //Å‰‚ÌÀ•W
	Vector3 OldPosition = Vector3::Zero;                  //‘O‚ÌƒtƒŒ[ƒ€‚ÌÀ•W
	Vector3 m_position = Vector3::Zero;                   //À•W
	float m_position_YUp = 36.0f;                         //ƒ‚ƒfƒ‹‚Ì²‚ª˜‚É‚ ‚é‚Ì‚ÅYÀ•W‚ğ50.0fã‚°‚é
	Vector3 m_forward = Vector3::AxisZ;                   //³–ÊƒxƒNƒgƒ‹
	Vector3 collisionRot= Vector3::Zero;                  //•KE‹Z
	CollisionObject* collisionObject;                     //ƒRƒŠƒWƒ‡ƒ“
	Vector3 UltCollisionPos= Vector3::Zero;               //•KE‹Z‚Ì“–‚½‚è”»’è‚ÌÀ•W
	Vector3 m_Skill_Right = Vector3::Zero;                 //ƒJƒƒ‰‚Ì‰E•ûŒü
	Vector3 m_Skill_Forward = Vector3::Zero;               //ƒJƒƒ‰‚Ì‘O•ûŒü
	CharacterController m_charCon;                        //ƒLƒƒƒ‰ƒNƒ^[ƒRƒ“ƒgƒ[ƒ‹
	Quaternion m_rot = Quaternion::Identity;              //ƒNƒH[ƒ^ƒjƒIƒ“
	ModelRender m_modelRender;                            //ƒ‚ƒfƒ‹ƒŒƒ“ƒ_[
	AnimationClip m_animationClips[enAnimationClip_Num]; //ƒAƒjƒ[ƒVƒ‡ƒ“ƒNƒŠƒbƒv
	PlayerState m_playerState/* = enKnightState_Num*/;
	
	//ƒŒƒxƒ‹ƒAƒbƒv‚É‘‰Á‚·‚éƒXƒe[ƒ^ƒX
	LvUpStatus LvUpStatus = { 30,10,30.0f };
	PlayerState m_animState = enKnightState_Idle;
	//Œ»İ‚ÌƒRƒ“ƒ{
	int ComboState = 0;
	//ƒRƒ“ƒ{‚ªŒp‘±‚·‚éŠÔ‚ğ‹L˜^‚·‚é
	float ComboTimer = 0;
	//ƒ{ƒ^ƒ“‚ª‰Ÿ‚³‚ê‚½‚©‚Ì”»’è
	bool pushFlag = false;
	//ˆê’i–Ú‚ÌƒAƒ^ƒbƒN‚ğ‚µ‚½‚©‚Ì”»’è
	bool AtkState = false;
	//ƒXƒLƒ‹‚ÌƒAƒjƒ[ƒVƒ‡ƒ“Ä¶‚ªI‚í‚Á‚½‚©‚Ì”»’è
	bool SkillEndFlag = false;
	//‰ñ”ğƒAƒjƒ[ƒVƒ‡ƒ“‚ğÄ¶‚µ‚½‚©‚Ì”»’è
	bool AvoidanceFlag = false;
	//
	bool AvoidanceEndFlag = false;
	//uvƒ{[ƒ“‚ÌID
	int m_swordBoneId = -1;
	//UŒ‚ƒAƒjƒ[ƒVƒ‡ƒ“ƒCƒxƒ“ƒgÄ¶‚ÌŒ•m‚ÌÀ•W‚ğæ“¾‚·‚é
	int AtkEndPosId= -1;
	//ƒXƒLƒ‹‚ÌƒN[ƒ‹ƒ^ƒCƒ€‚ğŒvZ‚·‚éƒ^ƒCƒ}[
	float SkillTimer = 0;
	//‰ñ”ğ‚ÌƒN[ƒ‹ƒ^ƒCƒ€‚ğŒvZ‚·‚éƒ^ƒCƒ}[
	float AvoidanceTimer = 0;

	//Šl“¾‚µ‚½ŒoŒ±’l‰¼
	int exp=5;
	//Newtral_Enemy‚ÌUŒ‚—Í
	int Enemy_atk = 10;
	//•KE‹Zg—p‚Ìƒtƒ‰ƒO
	bool UltCollisionSetFlag = false;
	//UŒ‚‚ÌŒ•‚ÌƒRƒŠƒWƒ‡ƒ“‚ğ•\¦‚·‚é‚©‚Ìƒtƒ‰ƒO
	bool AtkCollistionFlag = false;

};

