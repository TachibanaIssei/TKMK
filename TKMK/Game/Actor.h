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
	/// ‰ŠúƒXƒe[ƒ^ƒX
	/// </summary>
	//struct Status {
	//	int MaxHp;                 //Å‘åƒqƒbƒgƒ|ƒCƒ“ƒg
	//	int Hp;                    //ƒqƒbƒgƒ|ƒCƒ“ƒg(‘Ì—Í)
	//	int Atk;                   //UŒ‚—Í
	//	float Speed;               //ˆÚ“®‘¬“x
	//};

	/// <summary>
	/// ƒŒƒxƒ‹ƒAƒbƒv‚É‘‰Á‚·‚éƒXƒe[ƒ^ƒX
	/// </summary>
	struct LvUpStatus {
		int LvHp;                  //ƒŒƒxƒ‹ƒAƒbƒv‚Ìƒqƒbƒgƒ|ƒCƒ“ƒg
		int LvAtk;                 //ƒŒƒxƒ‹ƒAƒbƒv‚ÌUŒ‚—Í
		float LvSpeed;             //ƒŒƒxƒ‹ƒAƒbƒv‚ÌˆÚ“®‘¬“x
	};

public:

	/// <summary>
	/// ˆÚ“®ˆ—
	/// </summary>
	/// <param name="position">ƒLƒƒƒ‰ƒNƒ^[‚ÌŒ»İ‚ÌÀ•W</param>
	/// <param name="charcon">ƒLƒƒƒ‰ƒRƒ“</param>
	/// <param name="status">ƒLƒƒƒ‰‚ÌƒXƒe[ƒ^ƒX</param>
	/// <param name="stickL">ˆÚ“®‚Ì“ü—Í—Ê</param>
	void Move(Vector3& position,CharacterController& charcon, Status& status,Vector3 stickL);

	/// <summary>
	/// ˆÚ“®ˆ—‚µ‚½‚­‚È‚¢ƒAƒjƒ[ƒVƒ‡ƒ“‚ÌƒXƒe[ƒg‚ğ‘‚­
	/// </summary>
	/// <returns>ˆÚ“®ˆ—‚µ‚È‚¢ƒAƒjƒ[ƒVƒ‡ƒ“‚ÌƒXƒe[ƒg</returns>
	virtual bool IsEnableMove()const = 0;

	/// <summary>
	/// ƒŠƒXƒ|[ƒ“‚·‚éÀ•W‚ğæ“¾‚·‚é
	/// </summary>
	void GetRespawnPos();

	virtual void SetRespawn() = 0;

	/// <summary>
	/// UŒ‚‚Ìˆ—,ƒˆ‰¼‘zŠÖ”‚È‚Ì‚Å”h¶ƒNƒ‰ƒX‚ÅÀ‘•
	/// </summary>
	virtual void Attack() = 0;

	/// <summary>
	/// UŒ‚‚ğó‚¯‚½‚Æ‚«‚Ìˆ—
	/// </summary>
	/// <param name="damege">“G‚ÌUŒ‚—Í</param>
	virtual void Dameged(int damege) = 0;

	/// <summary>
	/// •KE‹Z‚ğ‘Å‚Âˆ—
	/// </summary>
	virtual void UltimateSkill() = 0;

	/// <summary>
	/// ƒXƒLƒ‹‚ªg—p‚³‚ê‚½Œã‚ÌƒN[ƒ‹ƒ^ƒCƒ€‚Ìˆ—
	/// </summary>
	/// <param name="SkillCooltimer">ƒXƒLƒ‹‚ÌƒN[ƒ‹ƒ^ƒCƒ€</param>
	/// <param name="skillstate">ƒXƒLƒ‹‚ğg—p‚µ‚½‚©‚Ì”»’è</param>
	/// <param name="timer">ƒN[ƒ‹ƒ^ƒCƒ€‚ğŒvZ‚·‚é•Ï”</param>
	void COOlTIME(float SkillCooltimer,bool& skillstate,float& timer);
	
	/// <summary>
	/// Œ»İ‚ÌƒŒƒxƒ‹‚ğ•Ô‚·@KnightBase‚Ì‚ğ‚Á‚Ä‚­‚éto do
	/// </summary>
	/// <returns></returns>
	inline int GetLevel() { return Lv; }

	/// <summary>
	/// ƒŒƒxƒ‹ƒAƒbƒv‚Ìˆ—
	/// </summary>
	/// <param name="lus">ƒŒƒxƒ‹ƒAƒbƒv‚É‘‰Á‚·‚éƒXƒe[ƒ^ƒX</param>
	/// <param name="nowStatus">Œ»İ‚ÌƒXƒe[ƒ^ƒX</param>
	/// <param name="Level">Œ»İ‚ÌƒŒƒxƒ‹</param>
	void LevelUp(LvUpStatus& lus, Status& nowStatus,int& Level);

	/// <summary>
	/// ƒŒƒxƒ‹ƒ_ƒEƒ“‚Ìˆ—
	/// </summary>
	/// <param name="lus">ƒŒƒxƒ‹ƒAƒbƒv‚É‘‰Á‚·‚éƒXƒe[ƒ^ƒX</param>
	/// <param name="nowStatus">Œ»İ‚ÌƒXƒe[ƒ^ƒX</param>
	/// <param name="Level">Œ»İ‚ÌƒŒƒxƒ‹</param>
	void levelDown(LvUpStatus& lus, Status& nowStatus, int& Level,int downLevel);

	/// <summary>
    /// ƒŠƒXƒ|[ƒ“‚µ‚½‚Æ‚«‚ÌƒŒƒxƒ‹‚É‚æ‚Á‚ÄŒoŒ±’l‚ğ•ÏX‚·‚é
    /// </summary>
    /// <param name="Lv">Œ»İ‚ÌƒŒƒxƒ‹</param>
	/// <param name="getExp">ŒoŒ±’l</param>
	void ExpReset(int& Lv, int& getExp);

	/// <summary>
    /// ŒoŒ±’lƒe[ƒuƒ‹‚ğ•ÏX‚·‚é
    /// </summary>
    /// <param name="Lv">Œ»İ‚ÌƒŒƒxƒ‹</param>
    /// <param name="expTable">ŒoŒ±’lƒe[ƒuƒ‹</param>
	void ExpTableChamge(int& Lv,int& expTable);

	/// <summary>
	/// €‚ñ‚¾‚Æ‚«‚Ìˆ—
	/// </summary>
	virtual void Death() = 0;

	/// <summary>
	/// ƒŠƒXƒ|[ƒ“‚·‚éÀ•W‚ğİ’è‚·‚é
	/// </summary>
	/// <param name="pos">ƒŠƒXƒ|[ƒ“‚·‚éÀ•W</param>
	/// <param name="number">À•W‚ğƒZƒbƒg‚·‚é”Ô†</param>
	void SetRespawnPos(Vector3 pos, int number) {
		m_respawnPos[number] = pos;
	};

	/// <summary>
	/// À•W‚ğİ’è
	/// </summary>
	/// <param name="position">À•W</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}

	/// <summary>
	/// À•W‚ğæ“¾
	/// </summary>
	/// <returns>À•W</returns>
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
	/// ˆø”‚ÉUŒ‚—Í‚ğ‘ã“ü‚³‚¹‚é
	/// </summary>
	int GetAtk() {
		return m_Status.Atk;
	}

	/////////////////è¿½åŠ ///////////////
	/// <summary>
	/// æ”»æ’ƒåŠ›ã‚’å¢—åŠ ã•ã›ã‚‹
	/// </summary>
	/// <param name="AtkUp">æ”»æ’ƒåŠ›ã‚’å¢—åŠ ã•ã›ã‚‹æ•°å€¤</param>
	void AtkUp(int AtkUp)
	{
		m_Status.Atk += AtkUp;
	}

	/// <summary>
	/// HPã‚’å›å¾©
	/// </summary>
	/// <param name="HpUp">å‰£å£«ã®HPã‚’å›å¾©ã•ã›ã‚‹æ•°å€¤</param>
	void HpUp(int HpUp)
	{
		m_Status.Hp += HpUp;
	}

	/// <summary>
	/// HPãŒä¸Šé™ã‚’è¶…ãˆãŸæ™‚ã«ä¸Šé™ã¾ã§ã«ã™ã‚‹é–¢æ•°
	/// </summary>
	/// /// <param name="HpReset">MaxHpã‚’ä»£å…¥ã™ã‚‹</param>	
	void HpReset(int HpReset)
	{
		m_Status.Hp = HpReset;
	}

	//Hpä¸Šé™ã‚’æ¸¡ã™
	int GetMaxHp() { return m_Status.MaxHp; };
	//Hpã‚’æ¸¡ã™
	int GetHp() { return m_Status.Hp; };
	/////////////////////è¿½åŠ çµ‚ã‚ã‚Š///////////////////////////

	/// <summary>
	/// ’†—§‚Ì“G‚ğ“|‚µ‚½‚Æ‚«‚ÌŒoŒ±’l‚Ìˆ—
	/// </summary>
	/// <param name="GetExp">’†—§‚Ì“G‚ÌŒoŒ±’l</param>
	void ExpProcess(int Exp);

private:
    Level3DRender m_respawnLevel;

	

protected:
	int Lv;                    //ƒŒƒxƒ‹
	int AtkSpeed;              //UŒ‚‘¬“x
	float Cooltime;            //ƒXƒLƒ‹‚ÌƒN[ƒ‹ƒ^ƒCƒ€
	float AvoidanceCoolTime;    //‰ñ”ğ‚ÌƒN[ƒ‹ƒ^ƒCƒ€
	bool SkillState = false;     //ƒXƒLƒ‹‚ªg—p‰Â”\‚©‚Ç‚¤‚©‚Ì”»’è
	int Point;                 //“G‚ğ“|‚µ‚Äè‚É“ü‚ê‚½ƒ|ƒCƒ“ƒg
	int GetExp;                //’†—§‚Ì“G‚ğ“|‚µ‚½‚Æ‚«‚ÌŒoŒ±’l
	int ExpTable;              //ŒoŒ±’lƒe[ƒuƒ‹
	int respawnNumber;         //ƒŠƒXƒ|[ƒ“‚·‚éÀ•W‚Ì”Ô†
	bool isDeath = false;      //€‚ñ‚¾‚©‚Ç‚¤‚©‚Ì”»’è
	bool invincibleFlag = false;     //–³“GŠÔƒtƒ‰ƒO
	//float timer = 0.0f;
	//Status m_Status;           //ƒXƒe[ƒ^ƒX
	Vector3 m_respawnPos[4];    //ƒŠƒXƒ|[ƒ“‚·‚éÀ•W‚Ì”z—ñ
	Vector3 m_moveSpeed = Vector3::Zero;      //ˆÚ“®—Ê

	Vector3 m_Forward;          //ƒvƒŒƒCƒ„[‚Ì‘O•ûŒü

	Vector3 m_position = Vector3::Zero;                   //À•W
	ModelRender m_modelRender;                            //ƒ‚ƒfƒ‹ƒŒƒ“ƒ_[
	Quaternion m_rot = Quaternion::Identity;              //‰ñ“]
	bool m_spriteFlag = true;
	Status m_Status;                                      //ƒXƒe[ƒ^ƒX
	//ƒŒƒxƒ‹ƒAƒbƒv‚É‘‰Á‚·‚éƒXƒe[ƒ^ƒX
	LvUpStatus LvUPStatus = { 30,5,10.0f };
};

