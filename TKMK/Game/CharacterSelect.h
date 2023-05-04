#pragma once
#include "physics/PhysicsGhostObject.h"

class Tittle;
class Game;

class CharacterSelect:public IGameObject
{
public:
	CharacterSelect();
	~CharacterSelect();
	bool Start();
	void Update();
	void Cursor();

	/// <summary>
	/// ポインターの移動処理
	/// </summary>
	void PointerMove();

	/// <summary>
	/// コリジョン作成
	/// </summary>
	void SetCollision();

	/// <summary>
	/// ゴーストオブジェクトの当たり判定の処理
	/// </summary>
	void GhostCollision();

	void SetKnightModel();

	void Render(RenderContext& rc);
	
	//アニメーション
	enum EnAnimationClip {
		enAnimationClip_Idle,
		enAnimationClip_Walk,
		enAnimationClip_Run,
		enAnimationClip_ChainAtk,
		enAnimationClip_SecondAtk,
		enAnimationClip_lastAtk,
		enAnimationClip_Damege,
		enAnimationClip_Death,
		enAnimationClip_Skill,
		enAnimationClip_UltimateSkill,
		enAnimationClip_Avoidance,
		enAnimationClip_Jump,
		enAnimationClip_Fall,
		enAnimationClip_Num,
	};
	AnimationClip m_animationClips[enAnimationClip_Num]; //アニメーションクリップ

	enum EnCharacterSelect {
		enCharacterSelect_Knight,		//剣士
		enCharacterSelect_Wizard,		//魔法使い
		enCharacterSelect_Zombie,		//ゾンビ
		enCharacterSelect_Mitei2,		//未定
		enCharacterSelect_Num,			//キャラクターの種類の数
	};
 

private:
	SpriteRender m_SelectCursor;			//カーソル

	SpriteRender m_pointer_black;					//ポインター
	SpriteRender m_pointer_white;					//ポインター


	SpriteRender m_Status;					//ステータス
	SpriteRender m_Attack_Icon;				//攻撃、スキル、必殺技アイコン
	SpriteRender m_UnderBar;				//画面下のバー
	SpriteRender m_name;					//名前
	
	SpriteRender m_Attack_explanation;		//攻撃の説明文
	SpriteRender Skill_explanation;			//スキルの説明文
	SpriteRender Ult_explanation;			//必殺技の説明文

	ModelRender m_Knight;					//剣士のモデル
	Quaternion m_KnightRot;

	ModelRender m_platform;					//モデルを乗せる台
	

	EnCharacterSelect m_characterSelect;	//キャラクターセレクト

	Vector3 curPosition = { -510.0f,75.0f,0.0f };


	//ポインター
	CharacterController Poimter;
	Vector3 m_Pointerposition = Vector3(0.0f,200.0f,-0.0f);
	Vector3 m_moveSpeed = Vector3::Zero;

	PhysicsGhostObject Attack;
	bool Attack_explanationFlag = false;
	PhysicsGhostObject Skill;
	bool Skill_explanationFlag = false;
	PhysicsGhostObject UltimateSkill;
	bool Ult_explanationFlag = false;

	float time;

	int CursorNum = 0;
	int TitleScene = 1;
};

