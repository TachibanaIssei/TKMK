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
	void Ilust();

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
		enTitleScene_Change,						//線形補完のタイミング
		enTitleScene_Select,						//操作説明などの画面
		enTitleScene_Num,
	};

	//どこを選択しているか
	enum EnSelectPosition {
		enSelectPosition_Start,						//スタートポジション
		enSelectPosition_Operation,					//操作説明のポジション
		enSelectPosition_Option,					//オプションのポジション
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
	SpriteRender m_start;						//"START"非選択
	SpriteRender m_startST;						//"START"選択
	SpriteRender m_operation;					//"HOWTOPLAY"非選択
	SpriteRender m_operationST;					//"HOWTOPLAY"選択
	SpriteRender m_option;						//"OPTION"非選択
	SpriteRender m_optionST;					//"OPTION"選択
	SpriteRender m_fire;						//セレクト
	SpriteRender m_operationPic;				//操作説明の画像
	SpriteRender m_KnightOp;					//剣士の説明画像
	SpriteRender m_WizardOp;					//魔法使いの説明画像
	SpriteRender m_ZombieOp;					//ゾンビの説明画像
	SpriteRender m_Opchoice;					//キャラ説明のセレクト画像
	SpriteRender m_iluststart;					//"START"の時のイラスト
	SpriteRender m_iluststartOp;				//"START"の時の説明文
	SpriteRender m_ilusthowtoplay;				//"HOWTOPLAY"の時のイラスト
	SpriteRender m_ilusthowtoplayOp;			//"HOWTOPLAY"の時の説明文
	SpriteRender m_ilustoption;					//"OPTION"の時のイラスト
	SpriteRender m_ilustoptionOp;				//"OPTION"の時の説明文

	EnTitleScene m_titleScene;					//タイトルのポジション
	EnSelectPosition m_tSelectPosition;			//セレクトのポジション
	EnOperationLook m_operationLook;			//操作説明画像の表示
	EnCharacterOpLook m_characterOpLook;		//キャラクター説明の表示
	EnCharacterOpPosition m_characterOpPosition;//キャラクター説明のポジション

	Game* game=nullptr;
	SoundSource* m_bgm = nullptr;

	Vector3 m_sPosition = { g_vec3Zero };					//セレクトのポジション
	Vector3 m_firstPosition = { 0.0f,-2000.0f,0.0f };		//下に読み込むときのポジション
	Vector3 m_Topfirstposition = { 0.0f,2000.0f,0.0f };		//上に読み込むときのポジション
	Vector3 m_RightfirstPosition = { 2000.0f,0.0f,0.0f };	//右に読み込むときのポジション
	Vector3 m_opPosition = { g_vec3Zero };					//画像を表示するポジション

	Vector3 m_operationPosition = { 0.0f,-150.0f,0.0f };			//"HowToPlay"
	Vector3 m_startPosition = { 0.0f,-250.0f,0.0f };				//"Start"
	Vector3 m_optionPosition = { 0.0f,-350.0f,0.0f };				//"Option"

	const Vector3 m_titleLogoPosition = { 0.0f,200.0f,0.0f };				//起動時の画面のタイトルロゴのポジション
	const Vector3 m_selectLogoPosition = { -500.0f,400.0f,0.0f };			//セレクト画面のタイトルロゴのポジション
	
	Vector3 m_titleLogoScale = { 3.0f, 1.5f, 1.0f };				//起動時のタイトルロゴの大きさ
	Vector3 m_selectLogoScale = { 1.7f, 0.7f, 1.0f };				//セレクト画面のタイトルロゴの大きさ
	Vector3 m_LogoPosition;											//ロゴを線形補完するときに使う
	Vector3 m_LogoScale;
	Vector3 m_titlefireScale = { 1.0f, 1.0f, 1.0f };				//起動時の火花の大きさ
	Vector3 m_selectfireScale = { 0.6f, 0.6f, 1.0f };
	Vector3 m_fireScale;											//火花の線形補間
	Vector3 m_selectilust = { 500.0f,0.0f,0.0f };					//セレクト画面のイラストの場所
	Vector3 m_ilust;												//イラストの線形補間
	Vector3 m_selectilustOp = { 500.0f,-400.0f,0.0f };				//セレクト画面の説明文の場所
	Vector3 m_ilustOp;												//説明文の線形補間

	//セレクトの項目ごとの位置
	Vector3 m_Top = { -500.0f,-130.0f,0.0f };						//上
	Vector3 m_Central = { -500.0f,-250.0f,0.0f };					//中央
	Vector3 m_Under = {-500.0f,-370.0f,0.0f };						//下

	//キャラ選択の時のカーソルの位置
	Vector3 m_KnightCursor = { -400.0f, 450.0f, 0.0f };
	Vector3 m_WizardCursor = { -130.0f, 450.0f, 0.0f };
	Vector3 m_ZombieCursor = { 145.0f, 450.0f, 0.0f };
	Vector3 m_MiteiCursor = { 425.0f, 450.0f, 0.0f };

	//キャラクター説明画面でカーソルを左側に表示させるときに足すためのポジション
	Vector3 m_charaLeftCursor = { -130.0f,0.0f,0.0f };

	Vector4 m_color = { 1.0f,1.0f,1.0f,0.3f };		//選択されてないときに透明度を低くして強調しないようにする時に使う
	Vector4 m_colorST = { g_vec4White };			//通常の透明度

	int selectPosition = 0;							//タイトルのセレクト画面でカーソルを移動するときに使う変数
	int characterOpPosition = 0;					//キャラ説明画面でカーソルを移動するときに使う変数
	int titleScene = 0;								//タイトルのシーン変換するときの変数
	float LogoComplement = 0.04f;					//補完率
	int m_timer = 0;
};

