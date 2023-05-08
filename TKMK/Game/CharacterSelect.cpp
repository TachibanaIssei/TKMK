#include "stdafx.h"
#include "CharacterSelect.h"
#include "Tittle.h"
#include "Game.h"
#include "Fade.h"

namespace {
	const Vector3	STATUS_POS = Vector3(400.0f, 250.0f, 0.0f);				//ステータスの座標
	const Vector3	ATTACK_ICON_POS = Vector3(400.0f, -150.0f, 0.0f);		//攻撃、スキル、必殺技アイコンの座標
	const Vector3	NAME_POS = Vector3(-600.0f, -400.0f, 0.0f);				//名前の座標
	const Vector3	UNDERBAR_POS = Vector3(0.0f, -500.0f, 0.0f);			//画面下のバーの座標

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
	Vector3 dir = { 0.0f,-1.0f,0.0f };
	Vector3 color = { 0.5f,0.5f,0.5f };
	g_renderingEngine->SetDirectionLight(0, dir, color);

	//剣士のモデル、アニメーション
	SetKnightModel();

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
	m_platform.Init("Assets/modelData/platform/platform.tkm");
	m_platform.SetPosition(PLATFORM_POS);
	m_platform.SetScale(2.2f, 2.0f, 2.2f);
	m_platform.Update();

	//カーソル
	m_selectCursor.Init("Assets/sprite/SelectCurSor.DDS", 1200.0f, 675.0f);
	m_selectCursor.SetPosition(m_cursorPosition);
	m_selectCursor.SetScale(1.0f, 1.0f, 1.0f);
	m_selectCursor.Update();

	//////////////////////////////////////////////////////////////////////////////////////
	//ステータス
	m_status.Init("Assets/sprite/Select/Status.DDS", 950.0f, 350.0f);
	m_status.SetPosition(STATUS_POS);
	m_status.SetScale(1.0f, 1.0f, 1.0f);
	m_status.Update();

	//攻撃、スキル、必殺技アイコン
	m_attackIcon.Init("Assets/sprite/Select/Attack_Icon.DDS", 1000.0f, 450.0f);
	m_attackIcon.SetPosition(ATTACK_ICON_POS);
	m_attackIcon.SetScale(1.0f, 1.0f, 1.0f);
	m_attackIcon.Update();

	//名前
	m_name.Init("Assets/sprite/Select/name.DDS", 550.0f, 200.0f);
	m_name.SetPosition(NAME_POS);
	m_name.SetScale(1.0f, 1.0f, 1.0f);
	m_name.Update();

	//画面下のバー
	m_underBar.Init("Assets/sprite/Select/underBar.DDS", 1920.0f, 300.0f);
	m_underBar.SetPosition(UNDERBAR_POS);
	m_underBar.SetScale(1.0f, 1.0f, 1.0f);
	m_underBar.Update();

	//説明文
	{
		//攻撃の説明文
		m_attackExplanation.Init("Assets/sprite/Select/Attack_explanation.DDS", 1120.0f, 300.0f);
		m_attackExplanation.SetPosition(ATTACK_EXPLANATION_POS);
		m_attackExplanation.SetScale(1.0f, 1.0f, 1.0f);
		m_attackExplanation.Update();

		//スキルの説明文
		m_skillExplanation.Init("Assets/sprite/Select/Skill_explanation.DDS", 1120.0f, 450.0f);
		m_skillExplanation.SetPosition(SKILL_EXPLANATION_POS);
		m_skillExplanation.SetScale(1.0f, 1.0f, 1.0f);
		m_skillExplanation.Update();

		//必殺技の説明文
		m_ultExplanation.Init("Assets/sprite/Select/Ult_explanation.DDS", 1700.0f, 501.0f);
		m_ultExplanation.SetPosition(ULT_EXPLANATION_POS);
		m_ultExplanation.SetScale(1.0f, 1.0f, 1.0f);
		m_ultExplanation.Update();
	}

	g_soundEngine->ResistWaveFileBank(45, "Assets/sound/characterSelectBGM/characterSelect1.wav");

	m_bgm = NewGO<SoundSource>(0);
	m_bgm->Init(45);
	m_bgm->Play(true);
	m_bgm->SetVolume(m_bgmVolume);

	return true;
}

void CharacterSelect::Update()
{
	//ポインターの点滅
	time += g_gameTime->GetFrameDeltaTime();

	if (m_readyFlag == true)
	{
		Ready();
	}
	else
	{
		PointerMove();

		//Aボタンを押した時
		if (g_pad[0]->IsTrigger(enButtonA))
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
	}

	m_selectCursor.Update();

	//剣士を回転させる
	m_knightRot.AddRotationDegY(2.0f);
	m_knight.SetRotation(m_knightRot);
	m_knight.Update();

	m_platform.SetRotation(m_knightRot);
	m_platform.Update();

	//当たり判定の可視化
	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
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

void CharacterSelect::Render(RenderContext& rc)
{
	//剣士のモデル
	m_knight.Draw(rc);
	m_platform.Draw(rc);

	m_status.Draw(rc);
	m_attackIcon.Draw(rc);
	m_name.Draw(rc);
	m_underBar.Draw(rc);

	if (m_attackExplanationFlag == true)
	{
		m_attackExplanation.Draw(rc);
	}
	if (m_skillExplanationFlag == true)
	{
		m_skillExplanation.Draw(rc);
	}
	if (m_ultExplanationFlag == true)
	{
		m_ultExplanation.Draw(rc);
	}

	//点滅早くtodo
	if ((int)time % 2 == 0)
	{
		m_pointerWhite.Draw(rc);
	}
	
	m_pointerBlack.Draw(rc);
}

