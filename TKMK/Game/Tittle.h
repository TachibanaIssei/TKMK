#pragma once

class Game;
class CharacterSelect;

class Tittle:public IGameObject
{
public:
	Tittle();
	~Tittle();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void Select();
	void Operation();
	void CharacterOp();

	enum EnTitlePosition {
		enTitlePosition_Top,						//上
		enTitlePosition_Central,					//中央
		enTitlePosition_Under,						//下
		enTitlePosition_Num,
	};


	enum EnSelectPosition {
		enSelectPosition_Start,						//スタートポジション
		enSelectPosition_Operation,					//操作説明のポジション
		enSelectPosition_CharaExplanation,			//キャラ説明のポジション
		enSelectPosition_Num,						//セレクトの数
	};
	
	//操作説明画面が見えるか見えないか
	enum EnOperationLook {
		enOperationLook_UnSeem,						//見えない
		enOperationLook_Seem,						//見える
		enOperationLook_Num,
	};

	//キャラクター説明画面が見えるか見えないか
	enum EnCharacterOpLook {
		enCharacterOpLook_UnSeem,					//見えない
		enCharacterOpLook_Seem,						//見える
		enCharacterOpLook_Num,
	};

	//どのキャラクター説明を表示するか
	enum EnCharacterOpPosition {
		enCharacterOpPosition_Knight,				//剣士の説明
		enCharacterOpPosition_Wizard,				//魔法使いの説明
		enCharacterOpPosition_Zombie,				//ゾンビの説明
		enCharacterOpPosition_Mitei,				//キャラ未定
		enCharacterOpPosition_Num,
	};

private:
	Quaternion m_sRotation;

	SpriteRender m_spriteRender;
	SpriteRender m_start;						//"はじめる"
	SpriteRender m_operation;					//"操作説明"
	SpriteRender m_charaExplanation;			//"キャラクター説明"
	SpriteRender m_choice;						//セレクト
	SpriteRender m_operationPic;				//操作説明の画像
	SpriteRender m_KnightOp;					//剣士の説明画像
	SpriteRender m_WizardOp;					//魔法使いの説明画像
	SpriteRender m_ZombieOp;					//ゾンビの説明画像
	SpriteRender m_Opchoice;					//キャラ説明のセレクト画像

	EnSelectPosition m_tSelectPosition;			//セレクトのポジション
	EnOperationLook m_operationLook;			//操作説明画像の表示
	EnCharacterOpLook m_characterOpLook;		//キャラクター説明の表示
	EnCharacterOpPosition m_characterOpPosition;//キャラクター説明のポジション

	Game* game=nullptr;
	SoundSource* m_bgm = nullptr;

	Vector3 m_sPosition = { g_vec3Zero };		//セレクトのポジション
	Vector3 m_firstPosition = { 0.0f,2000.0f,0.0f };	//読み込むときのポジション
	Vector3 m_opPosition = { g_vec3Zero };			//画像を表示するポジション

	Vector3 m_TopPosition = { 0.0f,-150.0f,0.0f };			//上
	Vector3 m_CentralPosition = { 0.0f,-250.0f,0.0f };		//中央
	Vector3 m_UnderPosition = { 0.0f,-350.0f,0.0f };		//下

	int selectPosition = 0;
	int characterOpPosition = 0;
};

