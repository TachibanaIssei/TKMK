#pragma once

class Game;
class CharacterSelect;
class Fade;

class Tittle:public IGameObject
{
public:
	Tittle();
	~Tittle();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	void InitSprite();
	void ResistSound();
	void SpriteUpdate();
	void Scene();
	void Select();
	void Operation();
	void CharacterOp();
	void Ilust();
	void FadeSprite();

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

	//線形補間のタイミング
	enum EnLinear {
		enLinear_sword1,								//剣１
		enLinear_sword2,								//剣２
		enLinear_delay,									//タイムラグを作る
		enLinear_Logo,									//タイトルロゴ
		enLinear_End,									//終わり
		enLinear_Num,
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

	/// <summary>
	/// 操作説明画面のページの種類
	/// </summary>
	enum EnOperationPage {
		enOperationPage_nonGame = 0,	//プレイ以外の操作説明
		enOperationPage_InGame	= 1,		//プレイ中の操作説明
		enOperationPage_Ghost	= 2,		//ゴーストの説明
		enOperationpage_Num
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
	Fade* m_fade = nullptr;
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
	SpriteRender m_operationInGame;				//ゲーム中の操作
	SpriteRender m_ghostExplanation;			//ゴーストの説明画像
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
	SpriteRender m_titleswordwhite;				//白い剣
	SpriteRender m_titleswordbrack;				//黒い剣
	SpriteRender m_pressAButton;				//プレスAボタン
	SpriteRender m_choiceCursor;				//選択中のものを指す剣画像

	EnTitleScene m_titleScene;					//タイトルのポジション
	EnSelectPosition m_tSelectPosition;			//セレクトのポジション
	EnOperationLook m_operationLook;			//操作説明画像の表示
	EnOperationPage m_enOperationPage = EnOperationPage::enOperationpage_Num;	//操作説明画面の種類
	EnCharacterOpLook m_characterOpLook;		//キャラクター説明の表示
	EnCharacterOpPosition m_characterOpPosition;//キャラクター説明のポジション
	EnLinear m_linear;

	Game* game=nullptr;
	SoundSource* m_bgm = nullptr;

	Vector3 m_sPosition = { g_vec3Zero };					//セレクトのポジション
	Vector3 m_firstPosition = { 0.0f,-2000.0f,0.0f };		//下に読み込むときのポジション
	Vector3 m_Toprightfirstposition = { 1000.0f,2000.0f,0.0f };//右上に読み込むときのポジション
	Vector3 m_Topleftfirstposition = { -1000.0f,2000.0f,0.0f };	//左上に読み込むときのポジション
	Vector3 m_RightfirstPosition = { 2000.0f,0.0f,0.0f };	//右に読み込むときのポジション
	Vector3 m_opPosition = { g_vec3Zero };					//画像を表示するポジション

	Vector3 m_operationPosition = { 0.0f,-150.0f,0.0f };			//"HowToPlay"
	Vector3 m_startPosition = { 0.0f,-250.0f,0.0f };				//"Start"
	Vector3 m_optionPosition = { 0.0f,-350.0f,0.0f };				//"Option"

	const Vector3 m_titleLogoPosition = { -20.0f,160.0f,0.0f };				//起動時の画面のタイトルロゴのポジション
	const Vector3 m_selectLogoPosition = { -500.0f,400.0f,0.0f };			//セレクト画面のタイトルロゴのポジション
	
	Vector3 m_firsttitleScale = {5.0f,5.0f,1.0f };					//タイトルロゴの最初の大きさ
	Vector3 m_changeLogoScale = Vector3::Zero;
	Vector3 m_titleLogoScale = { 1.3f,1.1f,0.0f };						//起動時のタイトルロゴの大きさ
	Vector3 m_selectLogoScale = { 0.7f, 0.45f, 1.0f };				//セレクト画面のタイトルロゴの大きさ
	Vector3 m_LogoPosition = Vector3::Zero;											//ロゴを線形補完するときに使う
	Vector3 m_LogoScale = Vector3::Zero;
	Vector3 m_titlefireScale = { 1.0f, 1.0f, 1.0f };				//起動時の火花の大きさ
	Vector3 m_selectfireScale = { 0.6f, 0.6f, 1.0f };
	Vector3 m_fireScale = Vector3::Zero;											//火花の線形補間
	Vector3 m_selectilust = { 500.0f,0.0f,0.0f };					//セレクト画面のイラストの場所
	Vector3 m_ilust = Vector3::Zero;												//イラストの線形補間
	Vector3 m_selectilustOp = { 500.0f,-400.0f,0.0f };				//セレクト画面の説明文の場所
	Vector3 m_ilustOp = Vector3::Zero;												//説明文の線形補間
	Vector3 m_swordright = Vector3::Zero;											//白い剣の線形補間
	Vector3 m_swordleft = Vector3::Zero;											//黒い剣の線形補間
	Vector3 m_titleswordscale = { g_vec3One };						//起動時の大きさ
	Vector3 m_selectswordscale = { 0.7f, 0.7f, 1.0f };				//セレクト画面の剣の大きさ
	Vector3 m_swordscale = Vector3::Zero;											//剣のスケール
	Vector3 m_swordPosition = { 0.0f,220.0f,0.0f };

	//セレクトの項目ごとの位置
	Vector3 m_Top = { -500.0f,-130.0f,0.0f };						//上
	Vector3 m_Central = { -500.0f,-250.0f,0.0f };					//中央
	Vector3 m_Under = {-500.0f,-370.0f,0.0f };						//下

	//キャラ選択の時のカーソルの位置
	Vector3 m_KnightCursor = { -400.0f, 450.0f, 0.0f };
	Vector3 m_WizardCursor = { -130.0f, 450.0f, 0.0f };
	Vector3 m_ZombieCursor = { 145.0f, 450.0f, 0.0f };
	Vector3 m_MiteiCursor = { 425.0f, 450.0f, 0.0f };

	//線形補間後のポジションを仮保存
	Vector3 m_RightPos;                 //右の剣の座標
	Vector3 m_LeftPos;					//左の剣の座標

	//Logoのポジションを左にずらす
	Vector3 m_LeftLogo = { -10.0f,0.0f,0.0f };

	Vector4 m_color = { 1.0f,1.0f,1.0f,0.0f };		//選択されてないときに透明度を低くして強調しないようにする時に使う
	Vector4 m_colorST = { g_vec4White };			//通常の透明度

	int m_operationPageNumber = 3;
	int selectPosition = 0;							//タイトルのセレクト画面でカーソルを移動するときに使う変数
	int characterOpPosition = 0;					//キャラ説明画面でカーソルを移動するときに使う変数
	int titleScene = 0;								//タイトルのシーン変換するときの変数
	float firstLogo = 0.0f;
	float LogoComplement = 0.04f;					//補完率
	float swordright = 0.0f;						//剣の補完率(右)

	float swordleft = 0.0f;							//剣の補完率(左)
	int m_timer = 0;
	int fadetime = 0;
	int delaytime = 0;								//遅らせる時間
	float m_alpha = 0.0f;
	bool m_isWaitFadeout = false;					//PressAButtonの表示する間隔
	bool m_fadeSeem = true;							//PressAButtonを表示するか
	bool m_titlefadeSeem = false;					//タイトルの
	bool m_titleanim = false;						//タイトルのアニメーション中か
	float SEVolume = 0.4f;
	bool m_isComplement = true;						//線形補間中？trueなら補完中
	bool GOBack = false;
};

