#pragma once

class Game;

class Tittle:public IGameObject
{
public:
	Tittle();
	~Tittle();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void Select();

	enum EnSelectPosition {
		enSelectPosition_Start,						//スタートポジション
		enSelectPosition_Operation,					//操作説明のポジション
		enSelectPosition_CharaExplanation,			//キャラ説明のポジション
		enSelectPosition_Num,						//セレクトの数
	};


private:
	Quaternion m_sRotation;

	SpriteRender m_spriteRender;
	SpriteRender m_start;						//はじめる
	SpriteRender m_operation;					//操作説明
	SpriteRender m_charaExplanation;			//キャラクター説明
	SpriteRender m_choice;						//セレクト

	EnSelectPosition m_tSelectPosition;			//セレクトのポジション

	Game* game=nullptr;
	SoundSource* m_bgm = nullptr;

	Vector3 m_sPosition = { 0.0f,-150.0f,0.0f };	//セレクトのポジション
	Vector3 m_conPosition = { 0.0f,2000.0f,0.0f };	//コントローラーの画像のポジション

	int selectPosition = 0;
	int conPosition = 0;
};

