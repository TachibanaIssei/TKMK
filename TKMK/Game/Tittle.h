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
	void Scene();
	void Select();
	void Operation();
	void CharacterOp();

	/// <summary>
	/// キャラ選択画面からタイトルに戻った時にタイトルのセレクト画面に戻す関数
	/// </summary>
	/// <param name="scene">タイトルのシーン　0.PlessA画面 1.タイトルセレクト画面</param>
	void SetTitleScene(int scene)
	{
		titleScene = scene;
	}

	//どの画面が表示されているか
	enum EnTitleScene {
		enTitleScene_PressAScene,					//最初のpressAボタンの画面
		enTitleScene_Select,						//操作説明などの画面
		enTitleScene_Num,
	};

	//カーソルがどこに表示されているか
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

	SpriteRender m_spriteRender;				//タイトル背景
	SpriteRender m_titleLogo;					//タイトルロゴ
	SpriteRender m_start;						//"はじめる"
	SpriteRender m_operation;					//"操作説明"
	SpriteRender m_charaExplanation;			//"キャラクター説明"
	SpriteRender m_choice;						//セレクト
	SpriteRender m_operationPic;				//操作説明の画像
	SpriteRender m_KnightOp;					//剣士の説明画像
	SpriteRender m_WizardOp;					//魔法使いの説明画像
	SpriteRender m_ZombieOp;					//ゾンビの説明画像
	SpriteRender m_Opchoice;					//キャラ説明のセレクト画像


	EnTitleScene m_titleScene;			//タイトルのポジション
	EnSelectPosition m_tSelectPosition;			//セレクトのポジション
	EnOperationLook m_operationLook;			//操作説明画像の表示
	EnCharacterOpLook m_characterOpLook;		//キャラクター説明の表示
	EnCharacterOpPosition m_characterOpPosition;//キャラクター説明のポジション

	Game* game=nullptr;
	SoundSource* m_bgm = nullptr;

	Vector3 m_sPosition = { g_vec3Zero };				//セレクトのポジション
	Vector3 m_firstPosition = { 0.0f,-2000.0f,0.0f };	//読み込むときのポジション
	Vector3 m_opPosition = { g_vec3Zero };				//画像を表示するポジション

	Vector3 m_operationPosition = { 0.0f,-150.0f,0.0f };			//"操作説明"
	Vector3 m_startPosition = { 0.0f,-250.0f,0.0f };				//"はじめる"
	Vector3 m_charaExplanationPosition = { 0.0f,-350.0f,0.0f };		//"キャラクター説明"

	const Vector3 m_titleLogoPosition = { 0.0f,200.0f,0.0f };				//起動時の画面のタイトルロゴのポジション
	const Vector3 m_selectLogoPosition = { -500.0f,400.0f,0.0f };			//セレクト画面のタイトルロゴのポジション
	
	Vector3 m_titleLogoScale = { 3.0f, 1.5f, 1.0f };				//起動時のタイトルロゴの大きさ
	Vector3 m_selectLogoScale = { 1.7f, 0.7f, 1.0f };				//セレクト画面のタイトルロゴの大きさ
	Vector3 m_LogoPosition;											//ロゴを線形補完するときに使う
	Vector3 m_LogoScale;

	//セレクトの項目ごとの位置
	Vector3 m_Top = { -500.0f,-150.0f,0.0f };						//上
	Vector3 m_Central = { -500.0f,-250.0f,0.0f };					//中央
	Vector3 m_Under = {-500.0f,-350.0f,0.0f };						//下

	Vector4 m_color = { 1.0f,1.0f,1.0f,0.3f };		//選択されてないときに透明度を高くして強調しないようにする時に使う

	int selectPosition = 0;							//タイトルのセレクト画面でカーソルを移動するときに使う変数
	int characterOpPosition = 0;					//キャラ説明画面でカーソルを移動するときに使う変数
	int titleScene = 0;								//タイトルのシーン変換するときの変数
	float LogoComplement = 0.01f;					//補完率
	int m_timer = 0;
};

