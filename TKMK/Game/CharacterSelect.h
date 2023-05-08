#pragma once
#include "physics/PhysicsGhostObject.h"

class Tittle;
class Game;
class Fade;

class CharacterSelect:public IGameObject
{
public:
	CharacterSelect();
	~CharacterSelect();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
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

	enum EnCharacterSelect {
		enCharacterSelect_Knight,		//剣士
		enCharacterSelect_Wizard,		//魔法使い
		enCharacterSelect_Zombie,		//ゾンビ
		enCharacterSelect_Mitei2,		//未定
		enCharacterSelect_Num,			//キャラクターの種類の数
	};

	/// <summary>
	/// インゲームへの処理
	/// </summary>
	void Ready();

	/// <summary>
	/// ポインターの移動処理
	/// </summary>
	void PointerMove();

	/// <summary>
	/// ポインターの横移動処理
	/// </summary>
	void PointerMoveX();

	/// <summary>
	/// ポインターの縦移動処理
	/// </summary>
	void PointerMoveY();

	/// <summary>
	/// 剣士モデルの設定
	/// </summary>
	void SetKnightModel();


private:
	Fade* fade = nullptr;
	SoundSource* m_bgm = nullptr;

	AnimationClip m_animationClips[enAnimationClip_Num]; //アニメーションクリップ

	SpriteRender m_selectCursor;			//カーソル
	SpriteRender m_pointerBlack;			//黒ポインター
	SpriteRender m_pointerWhite;			//白ポインター
	SpriteRender m_status;					//ステータス
	SpriteRender m_attackIcon;				//攻撃、スキル、必殺技アイコン
	SpriteRender m_underBar;				//画面下のバー
	SpriteRender m_name;					//名前
	SpriteRender m_attackExplanation;		//攻撃の説明文
	SpriteRender m_skillExplanation;		//スキルの説明文
	SpriteRender m_ultExplanation;			//必殺技の説明文

	ModelRender m_knight;					//剣士のモデル
	ModelRender m_platform;					//モデルを乗せる台

	Quaternion m_knightRot;					//剣士の回転

	EnCharacterSelect m_characterSelect;	//キャラクターセレクト

	Vector3 m_cursorPosition = { -510.0f,75.0f,0.0f };

	Vector3 m_pointerPosition	= Vector3(0.0f,90.0f,0.0f);

	bool m_attackExplanationFlag = false;	//通常攻撃説明画像表示フラグ
	bool m_skillExplanationFlag = false;	//スキル説明画像表示フラグ
	bool m_ultExplanationFlag = false;		//必殺技説明画像表示フラグ
	bool m_readyFlag = false;				//インゲーム開始フラグ

	float time;
	float m_bgmVolume = 0.4f;				//BGMのボリューム

};

