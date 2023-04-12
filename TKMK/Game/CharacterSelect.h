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
	void Render(RenderContext& rc);

	
	
	enum EnCharacterSelect {
		enCharacterSelect_Knight,		//剣士
		enCharacterSelect_Wizard,		//魔法使い
		enCharacterSelect_Mitei,		//未定
		enCharacterSelect_Mitei2,		//未定
		enCharacterSelect_Num,			//キャラクターの種類の数
	};
 

private:
	SpriteRender m_CharacterSelectPic;		//セレクト画面
	SpriteRender m_SelectCursor;			//カーソル
	SpriteRender m_KnightPic;				//剣士の画像
	SpriteRender m_WizardPic;				//魔法使いの画像

	EnCharacterSelect m_characterSelect;	//キャラクターセレクト

	Vector3 curPosition = { -510.0f,75.0f,0.0f };

	int CursorNum = 0;
};

