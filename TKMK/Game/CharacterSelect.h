#pragma once

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
	SpriteRender m_CharacterSelectPic;		//セレクト画面
	SpriteRender m_SelectCursor;			//カーソル
	SpriteRender m_KnightPic;				//剣士の画像
	SpriteRender m_WizardPic;				//魔法使いの画像

	SpriteRender m_Status;					//ステータス
	SpriteRender m_Attack_Icon;				//攻撃、スキル、必殺技アイコン
	SpriteRender m_UnderBar;				//画面下のバー
	SpriteRender m_name;					//名前

	ModelRender m_Knight;					//剣士のモデル
	Quaternion m_KnightRot;
	

	EnCharacterSelect m_characterSelect;	//キャラクターセレクト

	Vector3 curPosition = { -510.0f,75.0f,0.0f };

	Vector3 m_CameraPosition = { -30.0f, 50.0f, -150.0f };

	int CursorNum = 0;
	int TitleScene = 1;
};

