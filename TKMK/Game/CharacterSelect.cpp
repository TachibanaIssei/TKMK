#include "stdafx.h"
#include "CharacterSelect.h"
#include "Tittle.h"
#include "Game.h"
#include "Fade.h"

namespace SelectConst {
	const float		MOVE_SPEED = 30.0f;

	const Vector3	STATUS_POS = Vector3(400.0f, 250.0f, 0.0f);				//ステータスの座標

	const Vector3	NORMAL_ATTACK_ICON_POS = Vector3(115.0f, -140.0f, 0.0f);//通常攻撃アイコンの座標
	const Vector3	SKILL_ICON_POS = Vector3(445.0f, -130.0f, 0.0f);		//スキルアイコンの座標
	const Vector3	ULT_ICON_POS = Vector3(770.0f, -130.0f, 0.0f);			//必殺技アイコンの座標
	const float		ICON_WIDTH = 310.0f;									//アイコン画像の横の解像度
	const float		ICON_HEIGHT = 400.0f;									//アイコン画像の縦の解像度
	const float		NORMAL_ATTACK_ICON_HEIGHT = 417.0f;

	const Vector3	NAME_POS = Vector3(-600.0f, -400.0f, 0.0f);				//名前の座標
	const Vector3	UNDERBAR_POS = Vector3(0.0f, -490.0f, 0.0f);			//画面下のバーの座標
	const float		UNDER_BER_WIDTH = 1920.0f;							//バーの幅
	const float		UNDER_BER_HEIGHT = 300.0f;							//バーの高さ

	const Vector3	START_POS = Vector3(0.0f, -460.0f, 0.0f);				//スタート画像の座標
	const float		START_WIDTH = 471.0f;									//スタート画像の幅
	const float		START_HEIGHT = 145.0f;									//スタート画像の高さ
	const float		START_WIDTH_DUMMY = 600.0f;								//カーソルと重なっているかの判定のための幅
	const float		START_HEIGHT_DUMMY = 200.0f;							//カーソルと重なっているかの判定のための高さ

	const Vector3 HP_BER_FRAME_POS = Vector3(527.0, 286.0f, 0.0f);			//HPバーのフレーム
	const Vector3 ATK_BER_FRAME_POS = Vector3(527.0, 163.0f, 0.0f);			//攻撃バーのフレーム
	const float BER_FRAME_WIDTH = 675.0f;									//フレームの幅
	const float BER_FRAME_HEIGHT = 109.0f;									//フレームの高さ
	const Vector3 HP_BER_POS = Vector3(203.0f, 286.0f, 0.0f);				//HPバーの座標
	const Vector3 ATK_BER_POS = Vector3(203.0f, 163.0f, 0.0f);				//攻撃バーの座標
	const float BER_WIDTH = 649.0f;											//バーの幅
	const float BER_HEIGHT = 96.0f;											//バーの高さ
	const Vector2 BER_PIVOT = Vector2(0.0f, 0.5f);							//バーのピボット

	const Vector3	ATTACK_EXPLANATION_POS = Vector3(0.0f, 33.0f, 0.0f);	//通常攻撃説明画像の座標
	const Vector3	SKILL_EXPLANATION_POS = Vector3(0.0f, 0.0f, 0.0f);		//スキル説明画像の座標
	const Vector3	ULT_EXPLANATION_POS = Vector3(0.0f, 30.0f, 0.0f);		//必殺技説明画像の座標

	const Vector3	GUIDE_BUTTON_POS = Vector3(648.0f, -445.0f, 0.0f);
	const float		GUIDE_BUTTON_WIDTH = 594.0f;
	const float		GUIDE_BUTTON_HEIGHT = 81.0f;

	const std::array<Vector3, 4>	KNIGHT_POS = {
		Vector3(-150.0f, 0.0f, 0.0f),				//剣士の座標
		Vector3(-350.0f, 0.0f, 150.0f),
		Vector3(-170.0f, 0.0f, 150.0f),
		Vector3(-20.0f, 0.0f, 150.0f)
	};

	const std::array<Vector3, 4>	PLATFORM_POS = {
		Vector3(KNIGHT_POS[0].x, -40.0f, KNIGHT_POS[0].z),			//土台の座標
		Vector3(KNIGHT_POS[1].x, -40.0f, KNIGHT_POS[1].z),
		Vector3(KNIGHT_POS[2].x, -40.0f, KNIGHT_POS[2].z),
		Vector3(KNIGHT_POS[3].x, -40.0f, KNIGHT_POS[3].z)
	};

	const Vector3	STAGE_POS = Vector3(0.0f, -50.0f, -90.0f);

	const Vector3	CAMERA_TARGET_POS = Vector3(0.0f, 90.0f, -200.0f);		//カメラのターゲット
	const Vector3	CAMERA_POSITION = Vector3(0.0f, 90.0f, -248.0f);		//カメラの座標

	const float		POINTER_SPEED = 10.0f;							//ポインターのスピード

	const float		MAX_SCREEN_WIDTH = 960.0f;							//画面の横の最大値
	const float		MIN_SCREEN_WIDTH = -960.0f;							//画面の横の最小値

	const float		MAX_SCREEN_HEIGHT = 540.0f;								//画面の縦の最大値
	const float		MIN_SCREEN_HEIGHT = -540.0f;							//画面の縦の最小値

	const float SE_OK_VOLUME = 0.3f;		//決定音の音量
	const float SE_SHOUTING_VOLUME = 1.0f;	//掛け声の音量

	const Vector3 PLAYER_COUNT_FONT = SelectConst::NAME_POS + Vector3(35.0f, 75.0f, 0.0f);
}

CharacterSelect::CharacterSelect()
{
}

CharacterSelect::~CharacterSelect()
{
	DeleteGO(m_skyCube);
}

bool CharacterSelect::Start()
{
	m_fade = FindGO<Fade>("fade");
	m_fade->StartFadeOut(1.0f);

	m_skyCube = NewGO<SkyCube>(0, "skyCube");
	m_skyCube->SetScale(300.0f);
	Vector3 pos = Vector3(0.0f, -900.0f, -900.0f);
	m_skyCube->SetPosition(pos);


	//カメラの座標を設定
	Quaternion rot;
	g_camera3D[0]->SetTarget(SelectConst::CAMERA_TARGET_POS);
	g_camera3D[0]->SetPosition(SelectConst::CAMERA_POSITION);
	g_camera3D[0]->Update();

	g_renderingEngine->SetAmbient(Vector3(0.5f, 0.5f, 0.5f));
	Vector3 dir = Vector3(0.0f, -1.0f, 0.5f);
	dir.Normalize();
	Vector3 color = { 0.5f,0.5f,0.5f };
	g_renderingEngine->SetDirectionLight(0, dir, color);

	//剣士のモデル、アニメーション
	SetModel();

	//画像の初期化
	InitSprite();
	InitFont();

	g_soundEngine->ResistWaveFileBank(enSound_CharSelectBGM, "Assets/sound/characterSelectBGM/characterSelect1.wav");
	g_soundEngine->ResistWaveFileBank(enSound_KnightShouting, "Assets/sound/characterSelectBGM/knight_shouting.wav");

	m_bgm = NewGO<SoundSource>(0);
	m_bgm->Init(enSound_CharSelectBGM);
	m_bgm->Play(true);
	m_bgm->SetVolume(m_bgmVolume);

	//当たり判定の可視化
	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();

	return true;
}

void CharacterSelect::Update()
{
	//ポインターの点滅
	time += g_gameTime->GetFrameDeltaTime();

	ModelRotation();
	PlayAnimation();

	if (m_readyFlag == true)
	{
		Ready();
		return;
	}

	PointerMove();
	CheckIconOverlap();
	SetPlayerCountText();

	//スタートボタンを押したときか				//STARTの範囲内でAボタンを押した時
	if (g_pad[0]->IsTrigger(enButtonStart) || (m_underBarDrawFlag && g_pad[0]->IsTrigger(enButtonA)))
	{
		//決定音
		SoundSource* okSE = NewGO<SoundSource>(0);
		okSE->Init(enSound_OK);
		//プレイヤーとの距離によって音量調整
		okSE->SetVolume(SelectConst::SE_OK_VOLUME);
		okSE->Play(false);

		//掛け声
		SoundSource* knightShouting = NewGO<SoundSource>(0);
		knightShouting->Init(enSound_KnightShouting);
		//プレイヤーとの距離によって音量調整
		knightShouting->SetVolume(SelectConst::SE_SHOUTING_VOLUME);
		knightShouting->Play(false);

		m_charState = enCharacterState_Start;
	}

	//Bボタンが押されたらタイトルに戻る
	if (g_pad[0]->IsTrigger(enButtonB))
	{
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(enSound_Cancel);
		//プレイヤーとの距離によって音量調整
		se->SetVolume(1.0f);
		se->Play(false);
		Tittle* tittle = NewGO<Tittle>(0, "tittle");
		DeleteGO(m_bgm);
		DeleteGO(this);
	}

}

void CharacterSelect::PointerMove()
{
	m_moveSpeed = Vector3::Zero;

	Vector3 stickL = Vector3::Zero;
	stickL.x = g_pad[0]->GetLStickXF();
	stickL.y = g_pad[0]->GetLStickYF();
	stickL.z = 0.0f;

	m_moveSpeed.x += stickL.x * SelectConst::MOVE_SPEED;
	m_moveSpeed.y += stickL.y * SelectConst::MOVE_SPEED;

	m_pointerPosition += m_moveSpeed;

	PointerMoveX();
	PointerMoveY();

	m_pointerBlack.SetPosition(m_pointerPosition);
	m_pointerWhite.SetPosition(m_pointerPosition);

	m_pointerBlack.Update();
	m_pointerWhite.Update();
}

void CharacterSelect::PointerMoveX()
{
	//画面の左端の場合はそれ以上外にいかないようにする
	if (m_pointerPosition.x <= SelectConst::MIN_SCREEN_WIDTH)
	{
		m_pointerPosition.x = SelectConst::MIN_SCREEN_WIDTH;
	}
	//画面の右端の場合はそれ以上外にいかないようにする
	else if (m_pointerPosition.x >= SelectConst::MAX_SCREEN_WIDTH)
	{
		m_pointerPosition.x = SelectConst::MAX_SCREEN_WIDTH;
	}
}

void CharacterSelect::PointerMoveY()
{
	//画面の左端の場合はそれ以上外にいかないようにする
	if (m_pointerPosition.y <= SelectConst::MIN_SCREEN_HEIGHT)
	{
		m_pointerPosition.y = SelectConst::MIN_SCREEN_HEIGHT;
	}
	//画面の右端の場合はそれ以上外にいかないようにする
	else if (m_pointerPosition.y >= SelectConst::MAX_SCREEN_HEIGHT)
	{
		m_pointerPosition.y = SelectConst::MAX_SCREEN_HEIGHT;
	}
}

void CharacterSelect::InitSprite()
{
	//ポインターの黒
	m_pointerBlack.Init("Assets/sprite/Select/pointer_black.DDS", 220.0f, 220.0f);
	m_pointerBlack.SetPosition(m_pointerPosition);
	m_pointerBlack.SetScale(0.6f, 0.6f, 0.6f);
	m_pointerBlack.Update();

	//ポインターの白
	m_pointerWhite.Init("Assets/sprite/Select/pointer_white.DDS", 220.0f, 220.0f);
	m_pointerWhite.SetPosition(m_pointerPosition);
	m_pointerWhite.SetScale(0.6f, 0.6f, 0.6f);
	m_pointerWhite.Update();

	//////////////////////////////////////////////////////////////////////////////////////
	//ステータス
	m_status.Init("Assets/sprite/Select/Status_notBer.DDS", 950.0f, 350.0f);
	m_status.SetPosition(SelectConst::STATUS_POS);
	m_status.SetScale(g_vec3One);
	m_status.Update();

	//通常攻撃アイコン
	m_attackIcon.Init("Assets/sprite/Select/NormalAttack_Icon.DDS", SelectConst::ICON_WIDTH, 417.0f);
	m_attackIcon.SetPosition(SelectConst::NORMAL_ATTACK_ICON_POS);
	m_attackIcon.SetScale(g_vec3One);
	m_attackIcon.Update();

	//スキルアイコン
	m_skillIcon.Init("Assets/sprite/Select/Skill_Icon.DDS", SelectConst::ICON_WIDTH, SelectConst::ICON_HEIGHT);
	m_skillIcon.SetPosition(SelectConst::SKILL_ICON_POS);
	m_skillIcon.SetScale(g_vec3One);
	m_skillIcon.Update();

	//必殺アイコン
	m_ultIcon.Init("Assets/sprite/Select/ULT_Icon.DDS", SelectConst::ICON_WIDTH, SelectConst::ICON_HEIGHT);
	m_ultIcon.SetPosition(SelectConst::ULT_ICON_POS);
	m_ultIcon.SetScale(g_vec3One);
	m_ultIcon.Update();

	//名前
	m_name.Init("Assets/sprite/Select/name2.DDS", 550.0f, 200.0f);
	m_name.SetPosition(SelectConst::NAME_POS);
	m_name.SetScale(g_vec3One);
	m_name.Update();

	//画面下のバー
	m_underBar.Init("Assets/sprite/Select/underBarIn.DDS", SelectConst::UNDER_BER_WIDTH, SelectConst::UNDER_BER_HEIGHT);
	m_underBar.SetPosition(SelectConst::UNDERBAR_POS);
	m_underBar.SetScale(g_vec3One);
	m_underBar.Update();

	m_underBarYellow.Init("Assets/sprite/Select/underBarInYellow.DDS", SelectConst::UNDER_BER_WIDTH, SelectConst::UNDER_BER_HEIGHT);
	m_underBarYellow.SetPosition(SelectConst::UNDERBAR_POS);
	m_underBarYellow.SetScale(g_vec3One);
	m_underBarYellow.Update();

	m_underBarFrame.Init("Assets/sprite/Select/underBarFrame.DDS", SelectConst::UNDER_BER_WIDTH, SelectConst::UNDER_BER_HEIGHT);
	m_underBarFrame.SetPosition(SelectConst::UNDERBAR_POS);
	m_underBarFrame.SetScale(g_vec3One);
	m_underBarFrame.Update();

	m_startSprite.Init("Assets/sprite/Select/Start.DDS", SelectConst::START_WIDTH, SelectConst::START_HEIGHT);
	m_startSprite.SetPosition(SelectConst::START_POS);
	m_startSprite.SetScale(g_vec3One);
	m_startSprite.Update();

	m_hpBerFrame.Init("Assets/sprite/Select/bar_Frame.DDS", SelectConst::BER_FRAME_WIDTH, SelectConst::BER_FRAME_HEIGHT);
	m_hpBerFrame.SetPosition(SelectConst::HP_BER_FRAME_POS);
	m_hpBerFrame.SetScale(g_vec3One);
	m_hpBerFrame.Update();

	m_atkBerFrame.Init("Assets/sprite/Select/bar_Frame.DDS", SelectConst::BER_FRAME_WIDTH, SelectConst::BER_FRAME_HEIGHT);
	m_atkBerFrame.SetPosition(SelectConst::ATK_BER_FRAME_POS);
	m_atkBerFrame.SetScale(g_vec3One);
	m_atkBerFrame.Update();

	m_hpBerLv1.Init("Assets/sprite/Select/bar_Blue.DDS", SelectConst::BER_WIDTH, SelectConst::BER_HEIGHT);
	m_hpBerLv1.SetPosition(SelectConst::HP_BER_POS);
	m_hpBerLv1.SetPivot(SelectConst::BER_PIVOT);
	m_hpBerLv1.SetScale(Vector3(0.3f, 1.0f, 1.0f));
	m_hpBerLv1.Update();

	m_hpBerLvmax.Init("Assets/sprite/Select/bar_white.DDS", SelectConst::BER_WIDTH, SelectConst::BER_HEIGHT);
	m_hpBerLvmax.SetPosition(SelectConst::HP_BER_POS);
	m_hpBerLvmax.SetPivot(SelectConst::BER_PIVOT);
	m_hpBerLvmax.SetScale(Vector3(0.9f, 1.0f, 1.0f));
	m_hpBerLvmax.Update();

	m_atkBerLv1.Init("Assets/sprite/Select/bar_Blue.DDS", SelectConst::BER_WIDTH, SelectConst::BER_HEIGHT);
	m_atkBerLv1.SetPosition(SelectConst::ATK_BER_POS);
	m_atkBerLv1.SetPivot(SelectConst::BER_PIVOT);
	m_atkBerLv1.SetScale(Vector3(0.35f, 1.0f, 1.0f));
	m_atkBerLv1.Update();

	m_atkBerLvmax.Init("Assets/sprite/Select/bar_white.DDS", SelectConst::BER_WIDTH, SelectConst::BER_HEIGHT);
	m_atkBerLvmax.SetPosition(SelectConst::ATK_BER_POS);
	m_atkBerLvmax.SetPivot(SelectConst::BER_PIVOT);
	m_atkBerLvmax.SetScale(Vector3(0.85f, 1.0f, 1.0f));
	m_atkBerLvmax.Update();

	//説明文
	{
		//攻撃の説明文
		m_attackExplanation.Init("Assets/sprite/Select/Attack_explanation.DDS", 1120.0f, 300.0f);
		m_attackExplanation.SetPosition(SelectConst::ATTACK_EXPLANATION_POS);
		m_attackExplanation.SetScale(g_vec3One);
		m_attackExplanation.Update();

		//スキルの説明文
		m_skillExplanation.Init("Assets/sprite/Select/Skill_explanation.DDS", 1120.0f, 450.0f);
		m_skillExplanation.SetPosition(SelectConst::SKILL_EXPLANATION_POS);
		m_skillExplanation.SetScale(g_vec3One);
		m_skillExplanation.Update();

		//必殺技の説明文
		m_ultExplanation.Init("Assets/sprite/Select/Ult_explanation.DDS", 1700.0f, 501.0f);
		m_ultExplanation.SetPosition(SelectConst::ULT_EXPLANATION_POS);
		m_ultExplanation.SetScale(g_vec3One);
		m_ultExplanation.Update();
	}

	//操作の表示
	m_guideButton.Init("Assets/sprite/Select/guide_button.DDS", SelectConst::GUIDE_BUTTON_WIDTH, SelectConst::GUIDE_BUTTON_HEIGHT);
	m_guideButton.SetPosition(SelectConst::GUIDE_BUTTON_POS);
	m_guideButton.Update();
}

void CharacterSelect::InitFont()
{
	m_countFont.SetPosition(SelectConst::PLAYER_COUNT_FONT);
	m_countFont.SetScale(2.0f);
	m_countFont.SetShadowParam(true, 1.0f, g_vec4Black);
}

//ゲームに遷移する前にフェードアウトする
void CharacterSelect::Ready()
{
	if (m_fade->GetCurrentAlpha(Fade::enFadeSpriteType_Full, Fade::enFadeSpriteCategory_Tip) >= 1.0f)
	{
		g_renderingEngine->SetGameModeToRenderingEngine(m_gameMode);
		Game* game = NewGO<Game>(5, "game");
		//キャラクターセレクトが
		switch (m_characterSelect)
		{
			//剣士だったら
		case enCharacterSelect_Knight:
			game->SetCharacterSelect(m_characterSelect);
			break;
			//魔法使いだったら
		case enCharacterSelect_Wizard:
			game->SetCharacterSelect(m_characterSelect);
			break;
			//ゾンビ(予定)だったら
		case enCharacterSelect_Zombie:
			game->SetCharacterSelect(enCharacterSelect_Knight);
			break;
			//未定
		case enCharacterSelect_Mitei2:
			game->SetCharacterSelect(enCharacterSelect_Wizard);
			break;
		}
		DeleteGO(this);
		DeleteGO(m_bgm);

	}
}

/// <summary>
/// モデル、アニメーション読み込み
/// </summary>
void CharacterSelect::SetModel()
{
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/Knight/Knight_idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Walk].Load("Assets/animData/Knight/Knight_Walk.tka");
	m_animationClips[enAnimationClip_Walk].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Run].Load("Assets/animData/Knight/run.tka");
	m_animationClips[enAnimationClip_Run].SetLoopFlag(true);
	m_animationClips[enAnimationClip_ChainAtk].Load("Assets/animData/Knight/ChainAttack/first_Attack.tka");
	m_animationClips[enAnimationClip_ChainAtk].SetLoopFlag(false);
	m_animationClips[enAnimationClip_SecondAtk].Load("Assets/animData/Knight/ChainAttack/Second_Attack.tka");
	m_animationClips[enAnimationClip_SecondAtk].SetLoopFlag(false);
	m_animationClips[enAnimationClip_lastAtk].Load("Assets/animData/Knight/ChainAttack/last_Attack.tka");
	m_animationClips[enAnimationClip_lastAtk].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Skill].Load("Assets/animData/Knight/Knight_Skill.tka");
	m_animationClips[enAnimationClip_Skill].SetLoopFlag(false);
	m_animationClips[enAnimationClip_UltimateSkill].Load("Assets/animData/Knight/Knight_Ult_Tunder.tka");
	m_animationClips[enAnimationClip_UltimateSkill].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Damege].Load("Assets/animData/Knight/Knight_Damege.tka");
	m_animationClips[enAnimationClip_Damege].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Death].Load("Assets/animData/Knight/Knight_Death.tka");
	m_animationClips[enAnimationClip_Death].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Avoidance].Load("Assets/animData/Knight/Knight_Avoidance.tka");
	m_animationClips[enAnimationClip_Avoidance].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Jump].Load("Assets/animData/Knight/Knight_Jump.tka");
	m_animationClips[enAnimationClip_Jump].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Fall].Load("Assets/animData/Knight/Knight_fall2.tka");
	m_animationClips[enAnimationClip_Fall].SetLoopFlag(true);

	m_knight[0].Init("Assets/modelData/character/Knight/Knight_Blue2.tkm", m_animationClips, enAnimationClip_Num, enModelUpAxisZ);
	m_knight[1].Init("Assets/modelData/character/Knight/Knight_Red2.tkm", m_animationClips, enAnimationClip_Num, enModelUpAxisZ);
	m_knight[2].Init("Assets/modelData/character/Knight/Knight_Yellow2.tkm", m_animationClips, enAnimationClip_Num, enModelUpAxisZ);
	m_knight[3].Init("Assets/modelData/character/Knight/Knight_Green2.tkm", m_animationClips, enAnimationClip_Num, enModelUpAxisZ);
	for (int i = 0; i < m_knight.size(); i++)
	{
		m_platform[i].Init("Assets/modelData/platform/platform.tkm");

		//アニメーションイベント用の関数を設定する。
		m_knight[i].AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
			OnAnimationEvent(clipName, eventName);
			});
		m_knight[i].SetScale(2.7f, 2.7f, 2.7f);
		m_platform[i].SetScale(2.2f, 2.0f, 2.2f);

		m_knight[i].SetPosition(SelectConst::KNIGHT_POS[i]);
		m_platform[i].SetPosition(SelectConst::PLATFORM_POS[i]);
	}

	for (int i = 0; i < m_knight.size(); i++)
	{
		m_knight[i].Update();
		m_platform[i].Update();
	}

	m_stage.Init("Assets/modelData/background/stadium05_ground.tkm");
	m_stage.SetPosition(SelectConst::STAGE_POS);
	m_stage.SetScale(1.0f, 1.2f, 1.0f);
	m_stage.Update();

	m_wall.Init("Assets/modelData/background/stadium05_Wall.tkm");
	m_wall.SetPosition(SelectConst::STAGE_POS);
	m_wall.SetScale(1.0f, 0.7f, 1.0f);
	m_wall.Update();
}

void CharacterSelect::ModelRotation()
{
	m_knightRot.AddRotationDegY(2.0f);
	for (int i = 0; i < m_knight.size(); i++)
	{
		//剣士を回転させる
		m_knight[i].SetRotation(m_knightRot);

		//土台を回転させる
		m_platform[i].SetRotation(m_knightRot);

		m_knight[i].Update();
		m_platform[i].Update();
	}
}

void CharacterSelect::CheckIconOverlap()
{
	m_attackExplanationFlag = CheckNormalAttackIconOverlap();
	m_skillExplanationFlag = CheckSkillIconOverlap();
	m_ultExplanationFlag = CheckUltIconOverlap();
	m_underBarDrawFlag = CheckUnderBarOverlap();
}

void CharacterSelect::PlayAnimation()
{
	for (int i = 0; i < m_playerCount; i++)
	{
		switch (m_charState)
		{
		case(enCharacterState_Idel):
			m_knight[i].PlayAnimation(enAnimationClip_Idle, 0.1f);
			break;
		case(enCharacterState_Start):
			m_knight[i].PlayAnimation(enAnimationClip_UltimateSkill, 0.1f);
			break;
		}
	}
}

bool CharacterSelect::CheckNormalAttackIconOverlap()
{
	Vector4 IconPos = CalcIconPos(SelectConst::NORMAL_ATTACK_ICON_POS.x, SelectConst::NORMAL_ATTACK_ICON_POS.y, SelectConst::ICON_WIDTH, SelectConst::NORMAL_ATTACK_ICON_HEIGHT);

	//X軸が重なっているか
	if (m_pointerPosition.x >= IconPos.y && m_pointerPosition.x <= IconPos.x)
	{
		//Y軸が重なっているか
		if (m_pointerPosition.y >= IconPos.w && m_pointerPosition.y <= IconPos.z)
		{
			return true;
		}
	}
	return false;
}

bool CharacterSelect::CheckSkillIconOverlap()
{
	Vector4 IconPos = CalcIconPos(SelectConst::SKILL_ICON_POS.x, SelectConst::SKILL_ICON_POS.y, SelectConst::ICON_WIDTH, SelectConst::ICON_HEIGHT);

	//X軸が重なっているか
	if (m_pointerPosition.x >= IconPos.y && m_pointerPosition.x <= IconPos.x)
	{
		//Y軸が重なっているか
		if (m_pointerPosition.y >= IconPos.w && m_pointerPosition.y <= IconPos.z)
		{
			return true;
		}
	}
	return false;
}

bool CharacterSelect::CheckUltIconOverlap()
{
	Vector4 IconPos = CalcIconPos(SelectConst::ULT_ICON_POS.x, SelectConst::ULT_ICON_POS.y, SelectConst::ICON_WIDTH, SelectConst::ICON_HEIGHT);

	//X軸が重なっているか
	if (m_pointerPosition.x >= IconPos.y && m_pointerPosition.x <= IconPos.x)
	{
		//Y軸が重なっているか
		if (m_pointerPosition.y >= IconPos.w && m_pointerPosition.y <= IconPos.z)
		{
			return true;
		}
	}
	return false;
}

bool CharacterSelect::CheckUnderBarOverlap()
{
	Vector4 IconPos = CalcIconPos(SelectConst::START_POS.x, SelectConst::START_POS.y, SelectConst::START_WIDTH_DUMMY, SelectConst::START_HEIGHT_DUMMY);

	//X軸が重なっているか
	if (m_pointerPosition.x >= IconPos.y && m_pointerPosition.x <= IconPos.x)
	{
		//Y軸が重なっているか
		if (m_pointerPosition.y >= IconPos.w && m_pointerPosition.y <= IconPos.z)
		{
			return true;
		}
	}
	return false;
}

void CharacterSelect::SetPlayerCountText()
{
	if (g_pad[0]->IsTrigger(enButtonLeft))
	{
		m_playerCount--;
		m_playerCount = max(1, m_playerCount);
	}
	if (g_pad[0]->IsTrigger(enButtonRight))
	{
		m_playerCount++;
		m_playerCount = min(m_playerCount, 4);
	}

	wchar_t buf[255];
	swprintf_s(buf, L"%d", m_playerCount);
	m_countFont.SetText(buf);

	m_gameMode = static_cast<RenderingEngine::EnGameMode>(m_playerCount);
}

Vector4 CharacterSelect::CalcIconPos(float posX, float posY, float W, float H)
{
	Vector4 pos = g_vec4Black;
	//アイコンの右端
	pos.x = posX + (W / 2);
	//アイコンの左端
	pos.y = posX - (W / 2);
	//アイコンの上
	pos.z = posY + (H / 2);
	//アイコンの下
	pos.w = posY - (H / 2);

	return pos;
}

void CharacterSelect::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	if (wcscmp(eventName, L"start_game") == 0)
	{
		//フェードアウトを始める
		m_fade->StartFadeIn(1.0f, Fade::enFadeSpriteType_Full, Fade::enFadeSpriteCategory_Tip);
		m_readyFlag = true;
	}
}

void CharacterSelect::Render(RenderContext& rc)
{
	//剣士のモデル
	for (int i = 0; i < m_playerCount; i++)
	{
		m_knight[i].Draw(rc);
		m_platform[i].Draw(rc);
	}

	m_wall.Draw(rc);
	m_stage.Draw(rc);

	m_status.Draw(rc);
	//m_hpBerLvmax.Draw(rc);
	m_hpBerLv1.Draw(rc);
	//m_atkBerLvmax.Draw(rc);
	m_atkBerLv1.Draw(rc);

	m_hpBerFrame.Draw(rc);
	m_atkBerFrame.Draw(rc);

	m_attackIcon.Draw(rc);
	m_skillIcon.Draw(rc);
	m_ultIcon.Draw(rc);

	m_name.Draw(rc);
	m_countFont.Draw(rc);

	if (m_underBarDrawFlag)	m_underBarYellow.Draw(rc);
	else m_underBar.Draw(rc);

	m_underBarFrame.Draw(rc);
	m_startSprite.Draw(rc);

	if (m_attackExplanationFlag)	m_attackExplanation.Draw(rc);
	if (m_skillExplanationFlag)		m_skillExplanation.Draw(rc);
	if (m_ultExplanationFlag)		m_ultExplanation.Draw(rc);

	m_guideButton.Draw(rc);

	if ((int)time % 2 == 0)
	{
		m_pointerWhite.Draw(rc);
	}

	m_pointerBlack.Draw(rc);
}

