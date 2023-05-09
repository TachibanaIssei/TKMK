#include "stdafx.h"
#include "CharacterSelect.h"
#include "Tittle.h"
#include "Game.h"
#include "Fade.h"

namespace {
	const Vector3	STATUS_POS = Vector3(400.0f, 250.0f, 0.0f);				//ステータスの座標

	const Vector3	NORMAL_ATTACK_ICON_POS = Vector3(115.0f, -130.0f, 0.0f);//通常攻撃アイコンの座標
	const Vector3	SKILL_ICON_POS = Vector3(445.0f, -130.0f, 0.0f);		//スキルアイコンの座標
	const Vector3	ULT_ICON_POS = Vector3(770.0f, -130.0f, 0.0f);			//必殺技アイコンの座標
	const float		ICON_WIDTH	= 310.0f;									//アイコン画像の横の解像度
	const float		ICON_HEIGHT = 400.0f;									//アイコン画像の縦の解像度
	const float		NORMAL_ATTACK_ICON_HEIGHT = 417.0f;

	const Vector3	NAME_POS = Vector3(-600.0f, -400.0f, 0.0f);				//名前の座標
	const Vector3	UNDERBAR_POS = Vector3(0.0f, -490.0f, 0.0f);			//画面下のバーの座標
	const float		UNDER_BER_WIDTH		= 1920.0f;							//バーの幅
	const float		UNDER_BER_HEIGHT	= 300.0f;							//バーの高さ

	const Vector3	START_POS = Vector3(0.0f, -460.0f, 0.0f);				//スタート画像の座標
	const float		START_WIDTH = 471.0f;									//スタート画像の幅
	const float		START_HEIGHT = 145.0f;									//スタート画像の高さ
	const float		START_WIDTH_DUMMY = 600.0f;								//カーソルと重なっているかの判定のための幅
	const float		START_HEIGHT_DUMMY = 200.0f;							//カーソルと重なっているかの判定のための高さ

	const Vector3	ATTACK_EXPLANATION_POS = Vector3(20.0f, 33.0f, 0.0f);	//通常攻撃説明画像の座標
	const Vector3	SKILL_EXPLANATION_POS = Vector3(105.0f, 33.0f, 0.0f);	//スキル説明画像の座標
	const Vector3	ULT_EXPLANATION_POS = Vector3(40.0f, 30.0f, 0.0f);		//必殺技説明画像の座標

	const Vector3	KNIGHT_POS = Vector3(-150.0f, 0.0f, 0.0f);				//剣士の座標
	const Vector3	PLATFORM_POS = Vector3(-150.0f, -40.0f, 0.0f);			//土台の座標

	const Vector3	CAMERA_TARGET_POS = Vector3(0.0f, 90.0f, -200.0f);		//カメラのターゲット
	const Vector3	CAMERA_POSITION = Vector3( 0.0f, 90.0f, -248.0f );		//カメラの座標

	const float		POINTER_SPEED		= 10.0f;							//ポインターのスピード

	const float		MAX_SCREEN_WIDTH	= 960.0f;							//画面の横の最大値
	const float		MIN_SCREEN_WIDTH	= -960.0f;							//画面の横の最小値

	const float		MAX_SCREEN_HEIGHT = 540.0f;								//画面の縦の最大値
	const float		MIN_SCREEN_HEIGHT = -540.0f;							//画面の縦の最小値

}

CharacterSelect::CharacterSelect()
{
}

CharacterSelect::~CharacterSelect()
{
}

bool CharacterSelect::Start()
{
	fade = FindGO<Fade>("fade");

	//カメラの座標を設定
	Quaternion rot;
	g_camera3D->SetTarget(CAMERA_TARGET_POS);
	g_camera3D->SetPosition(CAMERA_POSITION);
	g_camera3D->Update();

	g_renderingEngine->SetAmbient(Vector3(0.5f, 0.5f, 0.5f));
	Vector3 dir = Vector3(0.0f,-1.0f,0.5f);
	dir.Normalize();
	Vector3 color = { 0.5f,0.5f,0.5f };
	g_renderingEngine->SetDirectionLight(0, dir, color);

	//剣士のモデル、アニメーション
	SetKnightModel();

	//画像の初期化
	InitSprite();

	g_soundEngine->ResistWaveFileBank(45, "Assets/sound/characterSelectBGM/characterSelect1.wav");

	m_bgm = NewGO<SoundSource>(0);
	m_bgm->Init(45);
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

	if (m_readyFlag == true)
	{
		Ready();
		return;
	}

	PointerMove();
	CheckIconOverlap();

	if 
	(
		//スタートボタンを押したときか
		g_pad[0]->IsTrigger(enButtonStart) ||
		(
			//STARTの範囲内でAボタンを押した時
			m_underBarDrawFlag &&
			g_pad[0]->IsTrigger(enButtonA)
		)
	)
	{
		//フェードアウトを始める
		fade->StartFadeIn(1.0f);
		m_readyFlag = true;
	}

	//Bボタンが押されたらタイトルに戻る
	if (g_pad[0]->IsTrigger(enButtonB))
	{
		Tittle* tittle = NewGO<Tittle>(0, "tittle");
		DeleteGO(this);
		DeleteGO(m_bgm);
	}

	ModelRotation();
}

void CharacterSelect::PointerMove()
{
	PointerMoveX();
	PointerMoveY();

	m_pointerBlack.SetPosition(m_pointerPosition);
	m_pointerWhite.SetPosition(m_pointerPosition);

	m_pointerBlack.Update();
	m_pointerWhite.Update();
}

void CharacterSelect::PointerMoveX()
{
	//左に動く
	if (g_pad[0]->GetLStickXF() <= -0.001f)
	{
		m_pointerPosition.x -= POINTER_SPEED;

		//画面の左端の場合はそれ以上外にいかないようにする
		if (m_pointerPosition.x <= MIN_SCREEN_WIDTH)
		{
			m_pointerPosition.x = MIN_SCREEN_WIDTH;
		}
	}
	//右に動く
	else if (g_pad[0]->GetLStickXF() >= 0.001f)
	{
		m_pointerPosition.x += POINTER_SPEED;

		//画面の右端の場合はそれ以上外にいかないようにする
		if (m_pointerPosition.x >= MAX_SCREEN_WIDTH)
		{
			m_pointerPosition.x = MAX_SCREEN_WIDTH;
		}
	}
}

void CharacterSelect::PointerMoveY()
{
	//下に動く
	if (g_pad[0]->GetLStickYF() <= -0.001f)
	{
		m_pointerPosition.y -= POINTER_SPEED;

		//画面の左端の場合はそれ以上外にいかないようにする
		if (m_pointerPosition.y <= MIN_SCREEN_HEIGHT)
		{
			m_pointerPosition.y = MIN_SCREEN_HEIGHT;
		}
	}
	//上に動く
	else if (g_pad[0]->GetLStickYF() >= 0.001f)
	{
		m_pointerPosition.y += POINTER_SPEED;

		//画面の右端の場合はそれ以上外にいかないようにする
		if (m_pointerPosition.y >= MAX_SCREEN_HEIGHT)
		{
			m_pointerPosition.y = MAX_SCREEN_HEIGHT;
		}
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

	//台の設定
	m_platform.InitBackGround("Assets/modelData/platform/platform.tkm");
	m_platform.SetPosition(PLATFORM_POS);
	m_platform.SetScale(2.2f, 2.0f, 2.2f);
	m_platform.Update();

	//////////////////////////////////////////////////////////////////////////////////////
	//ステータス
	m_status.Init("Assets/sprite/Select/Status.DDS", 950.0f, 350.0f);
	m_status.SetPosition(STATUS_POS);
	m_status.SetScale(g_vec3One);
	m_status.Update();

	//通常攻撃アイコン
	m_attackIcon.Init("Assets/sprite/Select/NormalAttack_Icon.DDS", ICON_WIDTH, 417.0f);
	m_attackIcon.SetPosition(NORMAL_ATTACK_ICON_POS);
	m_attackIcon.SetScale(g_vec3One);
	m_attackIcon.Update();

	//スキルアイコン
	m_skillIcon.Init("Assets/sprite/Select/Skill_Icon.DDS", ICON_WIDTH, ICON_HEIGHT);
	m_skillIcon.SetPosition(SKILL_ICON_POS);
	m_skillIcon.SetScale(g_vec3One);
	m_skillIcon.Update();

	//必殺アイコン
	m_ultIcon.Init("Assets/sprite/Select/ULT_Icon.DDS", ICON_WIDTH, ICON_HEIGHT);
	m_ultIcon.SetPosition(ULT_ICON_POS);
	m_ultIcon.SetScale(g_vec3One);
	m_ultIcon.Update();

	//名前
	m_name.Init("Assets/sprite/Select/name.DDS", 550.0f, 200.0f);
	m_name.SetPosition(NAME_POS);
	m_name.SetScale(g_vec3One);
	m_name.Update();

	//画面下のバー
	m_underBar.Init("Assets/sprite/Select/underBarIn.DDS", UNDER_BER_WIDTH, UNDER_BER_HEIGHT);
	m_underBar.SetPosition(UNDERBAR_POS);
	m_underBar.SetScale(g_vec3One);
	m_underBar.Update();

	m_underBarYellow.Init("Assets/sprite/Select/underBarInYellow.DDS", UNDER_BER_WIDTH, UNDER_BER_HEIGHT);
	m_underBarYellow.SetPosition(UNDERBAR_POS);
	m_underBarYellow.SetScale(g_vec3One);
	m_underBarYellow.Update();

	m_underBarFrame.Init("Assets/sprite/Select/underBarFrame.DDS", UNDER_BER_WIDTH, UNDER_BER_HEIGHT);
	m_underBarFrame.SetPosition(UNDERBAR_POS);
	m_underBarFrame.SetScale(g_vec3One);
	m_underBarFrame.Update();

	m_startSprite.Init("Assets/sprite/Select/Start.DDS", START_WIDTH, START_HEIGHT);
	m_startSprite.SetPosition(START_POS);
	m_startSprite.SetScale(g_vec3One);
	m_startSprite.Update();

	//説明文
	{
		//攻撃の説明文
		m_attackExplanation.Init("Assets/sprite/Select/Attack_explanation.DDS", 1120.0f, 300.0f);
		m_attackExplanation.SetPosition(ATTACK_EXPLANATION_POS);
		m_attackExplanation.SetScale(g_vec3One);
		m_attackExplanation.Update();

		//スキルの説明文
		m_skillExplanation.Init("Assets/sprite/Select/Skill_explanation.DDS", 1120.0f, 450.0f);
		m_skillExplanation.SetPosition(SKILL_EXPLANATION_POS);
		m_skillExplanation.SetScale(g_vec3One);
		m_skillExplanation.Update();

		//必殺技の説明文
		m_ultExplanation.Init("Assets/sprite/Select/Ult_explanation.DDS", 1700.0f, 501.0f);
		m_ultExplanation.SetPosition(ULT_EXPLANATION_POS);
		m_ultExplanation.SetScale(g_vec3One);
		m_ultExplanation.Update();
	}
}

//ゲームに遷移する前にフェードアウトする
void CharacterSelect::Ready()
{
	if (fade->GetCurrentAlpha() >= 1.0f)
	{
		Game* game = NewGO<Game>(0, "game");
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
/// 剣士のモデル、アニメーション読み込み
/// </summary>
void CharacterSelect::SetKnightModel()
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
	m_animationClips[enAnimationClip_UltimateSkill].Load("Assets/animData/Knight/Knight_UltimateAttack.tka");
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

	//剣士モデルを読み込み
	m_knight.Init("Assets/modelData/character/Knight/Knight_Blue2.tkm", m_animationClips, enAnimationClip_Num, enModelUpAxisZ);
	m_knight.SetPosition(KNIGHT_POS);
	m_knight.SetScale(2.7f, 2.7f, 2.7f);
	m_knight.Update();
}

void CharacterSelect::ModelRotation()
{
	//剣士を回転させる
	m_knightRot.AddRotationDegY(2.0f);
	m_knight.SetRotation(m_knightRot);

	//土台を回転させる
	m_platform.SetRotation(m_knightRot);
	
	m_knight.Update();
	m_platform.Update();
}

void CharacterSelect::CheckIconOverlap()
{
	m_attackExplanationFlag = CheckNormalAttackIconOverlap();
	m_skillExplanationFlag	= CheckSkillIconOverlap();
	m_ultExplanationFlag	= CheckUltIconOverlap();
	m_underBarDrawFlag = CheckUnderBarOverlap();
}

bool CharacterSelect::CheckNormalAttackIconOverlap()
{
	Vector4 IconPos = CalcIconPos(NORMAL_ATTACK_ICON_POS.x, NORMAL_ATTACK_ICON_POS.y, ICON_WIDTH, NORMAL_ATTACK_ICON_HEIGHT);

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
	Vector4 IconPos = CalcIconPos(SKILL_ICON_POS.x, SKILL_ICON_POS.y, ICON_WIDTH, ICON_HEIGHT);

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
	Vector4 IconPos = CalcIconPos(ULT_ICON_POS.x, ULT_ICON_POS.y, ICON_WIDTH, ICON_HEIGHT);

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
	Vector4 IconPos = CalcIconPos(START_POS.x, START_POS.y, START_WIDTH_DUMMY, START_HEIGHT_DUMMY);

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

void CharacterSelect::Render(RenderContext& rc)
{
	//剣士のモデル
	m_knight.Draw(rc);
	m_platform.Draw(rc);

	m_status.Draw(rc);

	m_attackIcon.Draw(rc);
	m_skillIcon.Draw(rc);
	m_ultIcon.Draw(rc);

	m_name.Draw(rc);

	if (m_underBarDrawFlag)	m_underBarYellow.Draw(rc);
	else m_underBar.Draw(rc);

	m_underBarFrame.Draw(rc);
	m_startSprite.Draw(rc);

	if (m_attackExplanationFlag)	m_attackExplanation.Draw(rc);
	if (m_skillExplanationFlag)		m_skillExplanation.Draw(rc);
	if (m_ultExplanationFlag)		m_ultExplanation.Draw(rc);

	//点滅早くtodo
	if ((int)time % 2 == 0)
	{
		m_pointerWhite.Draw(rc);
	}
	
	m_pointerBlack.Draw(rc);
}

