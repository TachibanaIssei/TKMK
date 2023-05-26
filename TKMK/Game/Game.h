#pragma once
#include "Effect.h"
#include "Level3DRender.h"
#include "Sounds.h"

class BackGround;
class Result;
class Tittle;
class GameCamera;
class KnightPlayer;
class Neutral_Enemy;
class GameUI;
class Map;
class KnightAI;
class WizardPlayer;
class Player;
class CharUltFlag;
class Actor;
class Lamp;
class Fade;
class ExpforKnight;

class Game : public IGameObject
{
public:
	Game();
	~Game();

	enum EnGameState {
		enGameState_Start=0,
		enGameState_Battle,
		enGameState_Pause,
		enGamestate_End,
		enGameState_Rezult,
		enGameState_BetweenGameAndPause,
		enGameState_Num,
	};
	

	EnEFK m_EnEffect = enEffect_Num;

	EnSound m_Sound = enSound_Num;

	enum EnPauseMenu
	{
		enPauseMenuState_Back,
		enPauseMenuState_HowToPlay,
		enPauseMenuState_BGM,
		enPauseMenuState_SE,
		enPauseMenuState_QuitGame,
	};
	EnPauseMenu m_EnPauseMenu = enPauseMenuState_Back;

	bool Start();
	void Update();
	void BattleStart();
	void Battle();
	void Pause();
	void End();
	void Between();
	void GoResult();
	void GameState();
	void Push_OK();							//決定音

	/// <summary>
	/// ポーズ時の移動処理
	/// </summary>
	void PauseMove();

	/// <summary>
	/// 選んだ番号に対応したステートにする処理
	/// </summary>
	void SelectMenu();

	/// <summary>
	/// メニュー時のステートの管理
	/// </summary>
	void MenuState();
	
	void Menu_Back();
	void Menu_HowToPlay();
	void Menu_BGM();
	void Menu_SE();
	void Menu_QuitGame();

	/// <summary>
	/// ゲーム中に再生される音の読み込み
	/// </summary>
	void SetMusic();

	void SetEffects();

	/// <summary>
	/// 効果音の音量を返す
	/// </summary>
	/// <returns>効果音の音量</returns>
	const float SetSoundEffectVolume()const
	{
		return SoundEffectVolume;
	}
	/// <summary>
	/// タワー上のエフェクト
	/// </summary>
	void TowerEFK();
	/// <summary>
	/// 中立の敵のリスポーン
	/// </summary>
	void Respawn();
	/// <summary>
	/// ウサギのリスポーン
	/// </summary>
	void RabbitRespawn();
	/// <summary>
	/// リスポーンする座標の設定
	/// </summary>
	void SetEnemyRespawnPos();

	//リスポーンする座標を格納する
	void SetRespawnPosition(Vector3 pos, Quaternion rot, int number) {
		EnemyRespawnPosition[number] = pos;
		EnemyReapawnPot[number] = rot;
	};

	//キャラが必殺技を使った時に全体的に暗くする
	void UltTimeSkyDarkness();

	//ライト系のリセット
	void LightReset();

	void Render(RenderContext& rc);

	/// <summary>
	/// 中立の敵の情報をリストから削除する
	/// </summary>
	/// <param name="enemy">消したい中立の敵</param>
	void RemoveEnemyFromList(Neutral_Enemy* enemy)
	{
		std::vector<Neutral_Enemy*>::iterator it = std::find(
			m_neutral_Enemys.begin(), // 中立の敵のリストの最初
			m_neutral_Enemys.end(),   // 中立の敵のリストの最後
			enemy                     // 消したい中立の敵
		);

		if (it != m_neutral_Enemys.end()) {
			m_neutral_Enemys.erase(it);
		}
	}
	//中立の敵のリストを返す

	std::vector<Neutral_Enemy*>& GetNeutral_Enemys() {
		return m_neutral_Enemys;
	}

	//Actorのリストを返す
	std::vector<Actor*>& GetActors() {
		return m_Actors;
	}

	/// <summary>
	/// CharacterSelectで選んだキャラをセットする
	/// </summary>
	/// <param name="select">使うキャラナンバー</param>
	void SetCharacterSelect(int select)
	{
		SelectCharNumber = select;
	}

	void GetActorPoints(int charPoints[]);

	/// <summary>
	/// 制限時間の処理
	/// </summary>
	void CountDown();

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	int CountDownMinutes() const
	{
		return m_StartToGameTimer;
	}

	float GetSecondsTimer()
	{
		return SecondsTimer;
	}

	float GetMinutesTimer()
	{
		return MinutesTimer;
	}

	EnGameState NowGameState()
	{
		return m_GameState;
	}

	void SetUltActor(Actor* actor)
	{
		Ultactor = actor;
	}
	Actor* GetUltActor()
	{
		return Ultactor;
	}

	/// <summary>
	/// 必殺技の溜めに入った時と必殺技発動後に呼び出す
	/// </summary>
	/// <param name="flag"></param>
	void SetUltTimeSkyFlag(bool flag)
	{
		UltTimeSkyFlag = flag;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	bool GetUltTimeSkyFlag()
	{
		return UltTimeSkyFlag;
	}

	/// <summary>
	///	必殺技を使ったときに呼び出す
	/// </summary>
	/// <param name="ultstop"></param>
	void SetStopFlag(bool ultstop)
	{
		UltStopFlag = ultstop;
	}

	/// <summary>
	/// 必殺技でゲームが止まっているかのフラグを返す
	/// </summary>
	/// <returns></returns>
	bool GetStopFlag()
	{
		return UltStopFlag;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="rabbit"></param>
	void SetRabbitFlag(bool rabbit)
	{
		RabbitFlag = rabbit;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="flag"></param>
	void SetUltCanUseFlag(bool flag)
	{
		UltCanUseFlag = flag;
		// タイマーをセット
		if (UltCanUseFlag == true)
		{
			UltCanUseTimer = 3.0f;
		}
	}

	//AIの判断用
	//必殺が使えるがどうか
	bool GetUltCanUseFlag()
	{
		return UltCanUseFlag;
	}

	void UnderSprite_Attack() {
		if (m_underSprite_Attack) {
			return;
		}

		if (m_underSprite_TowerDown) {
			m_underSprite_Attack = true;
		}
		UnderSpriteUpdate();
	}

	void UnderSprite_Skill() {
		if (m_underSprite_Skill) {
			return;
		}

		if (m_underSprite_TowerDown) {
			m_underSprite_Skill = true;
		}
		UnderSpriteUpdate();
	}

	void UnderSprite_Level(int lv) {
		if (m_underSprite_Level) {
			return;
		}

		if (lv >= 4) {
			m_underSprite_Level = true;
			UnderSpriteUpdate();
		}
	}

	void UnderSprite_Ult() {
		if (m_underSprite_Ult) {
			return;
		}

		if (m_underSprite_Attack && m_underSprite_Skill) {
			m_underSprite_Ult = true;
		}
		UnderSpriteUpdate();
	}

	/*bool IsActorGroundChack(Actor* actor);*/

	//ゲームが制限時間時間に達したかのフラグを返す
	bool IsGameEnd()
	{
		return GameEndFlag;
	}

	/// <summary>
	/// ゲームオブジェクトのアクティブと非アクティブを切り替える
	/// </summary>
	/// <param name="IsUltFlag">必殺技発動するか終わりかのフラグ</param>
	/// <param name="targetActor">カメラのターゲットのアクター</param>
	void ToggleObjectActive(bool IsUltFlag, Actor* targetActor);


private:
	/// <summary>
	/// 中立の敵の名前を設定する
	/// </summary>
	char* CreateEnemyName() {
		enemyNum++;
		swprintf_s(name_t, 255, L"Neutral_Enemy%d", enemyNum);
		enemyName = (char*)name_t;
		wcstombs(enemyName, name_t, sizeof(name_t));

		return enemyName;
	}
	/// <summary>
	/// 中立の敵の生成
	/// </summary>
	void CreateEnemy(Vector3 pos, Quaternion rot, bool isRabiit = false);

	/// <summary>
	/// スカイキューブの初期化処理
	/// </summary>
	void InitSkyCube();

	void UnderSpriteUpdate() {

		// Attack/Skill
		if (m_underSprite_Attack && m_underSprite_Skill && m_underSprite_Level) {
			m_underSprite.Init("Assets/sprite/Ult.DDS", 797.0f, 229.0f);
			m_underSprite.SetScale(Vector3(0.9f, 0.9f, 0.9f));
			return;
		}
		// Down
		if (m_underSprite_TowerDown) {
			m_underSprite.Init("Assets/sprite/FirstAttack.DDS", 886.0f, 255.0f);
			return;
		}
	}

	/// <summary>
	/// ターゲットのアクターをセット
	/// </summary>
	/// <param name="target"></param>
	void SetTargetActor(Actor* target)
	{
		TargetActor = target;
	}

	

	//ゲームのステート
	EnGameState m_GameState = enGameState_Start;

	Level3DRender m_level3DRender;
	Level3DRender m_Enemylevel;
	Level3DRender m_AIPos;
	FontRender m_fontRender;

	Quaternion m_rotation = Quaternion::Identity;
	Quaternion m_sRotation = Quaternion::Identity;
	Vector3 EnemyRespawnPosition[45];
	Quaternion EnemyReapawnPot[45];
	//ポーズの画像
	SpriteRender m_Pause_Front;    //ポーズのメイン
	SpriteRender m_Pause_Back;     //背景
	SpriteRender m_Menu_Back;        //Back
	SpriteRender m_Menu_HowToPlay;   //HowToPlay
	SpriteRender m_Menu_BGM;         //BGM
	SpriteRender m_Menu_SE;          //SE
	SpriteRender m_Menu_QuitGame;    //QuitGame
	SpriteRender m_Menu_SelectBar_BGM;   //SelectBar_BGM
	SpriteRender m_Menu_SelectBar_SE;   //SelectBar_SE
	SpriteRender m_operationPic;        //操作説明

	//ゲームの説明
	SpriteRender m_underSprite;			//下部に表示する説明の画像
	SpriteRender m_RabbitSprite;		//ウサギ出現の画像

	bool m_underSprite_TowerDown = false;
	bool m_underSprite_Attack = false;
	bool m_underSprite_Skill = false;
	bool m_underSprite_Level = false;
	bool m_underSprite_Ult = false;

	Vector3 SelectBar_BGMPos = Vector3::AxisX;
	Vector3 SelectBar_SEPos = Vector3::AxisX;

	float m_nuwBGMPos=30.0f;
	float m_nuwSEPos=30.0f;

	SkyCube* m_skyCube = nullptr;
	BackGround* m_backGround = nullptr;
	Result* m_rezult=nullptr;
	Tittle* m_tittle = nullptr;
	GameCamera* m_gamecamera = nullptr;
	GameUI* m_gameUI = nullptr;
	KnightPlayer* m_knightplayer = nullptr;
	KnightAI* m_KnightAI = nullptr;
	KnightAI* m_KnightAI1 = nullptr;
	KnightAI* m_KnightAI2 = nullptr;
	Neutral_Enemy* neutral_Enemy = nullptr;
	Map* m_Map = nullptr;
	SoundSource* m_bgm = nullptr;	
	WizardPlayer* wizardPlayer = nullptr;
	Player* player = nullptr;
	CharUltFlag* charUltFlag = nullptr;
	Lamp* lamp = nullptr;
	Fade* fade = nullptr;

	std::vector<Neutral_Enemy*> m_neutral_Enemys;
	std::vector<Actor*> m_Actors;
	std::vector<SoundSource*> m_soundSources;

	Vector3 m_position = Vector3::Zero;
	Vector3 m_moveSpeed = Vector3::Zero;
	Vector3 m_EFK_Pos = Vector3::Zero;

	//RigidBody				m_rigidBody;						//剛体。
	//BoxCollider			m_boxCollider;							//コライダー。

	float m_FluctuateSkyColor;
	float DarknessSkyColor;

	float m_FluctuateDirectionColor = 0.0f;
	Vector3 m_FluctuateAmbientColor = Vector3::Zero;

	Vector3 directionLightColor2;

	bool HowToPlaySpriteFlag = false;

	float m_spriteAlpha = 0.0f;

	int enemyNumber = 0;

	int SearchRespawnPosNumber = 0;
	int RandamRespawnPosNumber;
	bool EnemyRespawnFlag[10];

	//秒を計るタイマー
	float SecondsTimer = 0.0f;
	//分を計るタイマー5
	float MinutesTimer = 3.0f;

	//制限時間に達したかの判定
	bool GameEndFlag = false;
	//必殺技中みんな止まる
	bool UltStopFlag = false;
	//必殺技の溜めから必殺技終了までtrue
	bool UltTimeSkyFlag = false;

	Actor* Ultactor = nullptr;
	//必殺技発動時のターゲット
	Actor* TargetActor = nullptr;

	float m_StartToGameTimer = 6.0f;

	float m_EndtoResultTimer = 0.0f;
	//必殺使えるかどうかのタイマー
	float m_UltCanUseTimer = 0.0f;
	//リスポーンタイマー
	float m_RespawnTimer = 0.0f;
	//リスポーンタイマー（ウサギ用）
	float m_RabbitRespawnTimer = 0.0f;
	//
	float m_BetweenTimer = 0.0f;
	//ウサギが生きてるかどうかのタイマー
	bool RabbitFlag = false;

	//BGMの初期音量
	float BGMVolume = 2.0f;
	//効果音の初期音量1
	float SoundEffectVolume = 2.0f;

	//プレイヤーの使うキャラの番号
	//０…剣士
	//１…魔法使い
	//２…ゾンビ
	//３…魔物
	int SelectCharNumber = 1;


	//メニュー
	int MenuNumber = 0;
	int MenuNumber_old = 0;

	int enemyNum = 0;
	char* enemyName;
	wchar_t name_t[255];
	
	//falseの時しか必殺技を使えん
	bool UltCanUseFlag = false;
	float UltCanUseTimer = 0.0f;

	EffectEmitter* TowerDown = nullptr;


};

