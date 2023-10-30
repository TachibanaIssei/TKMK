#include "stdafx.h"
#include "WizardPlayer.h"
#include "Game.h"
#include "Neutral_Enemy.h"
#include "MagicBall.h"
#include "WizardUlt.h"
#include "GameUI.h"

namespace {
	const Vector2 AVOIDANCE_BAR_POVOT = Vector2(1.0f, 1.0f);
	const Vector3 AVOIDANCE_BAR_POS = Vector3(98.0f, -397.0f, 0.0f);

	const Vector3 AVOIDANCE_FLAME_POS = Vector3(0.0f, -410.0f, 0.0f);
}

WizardPlayer::WizardPlayer()
{
	//魔法使いのモデルを読み込む
	SetModel();

	//アニメーションイベント用の関数を設定する。
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});
	//リスポーンする座標2番の取得
	GetRespawnPos();
	m_respawnNumber = 2;        //リスポーンする座標の番号
	
	//リスポーンする座標のセット
	//キャラコン
	m_charCon.SetPosition(m_respawnPos[m_respawnNumber]);
	m_modelRender.SetPosition(m_respawnPos[m_respawnNumber]);
	m_modelRender.SetRotation(m_respawnRotation[m_respawnNumber]);
	
	//リスポーン時に向いている方向の前方向を取得
	ForwardSet();

	m_modelRender.Update();

	//スキルのクールタイムを表示するフォントの設定
	Skillfont.SetPosition(805.0f, -400.0f, 0.0f);
	Skillfont.SetScale(2.0f);
	Skillfont.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	Skillfont.SetRotation(0.0f);
	Skillfont.SetShadowParam(true, 2.0f, g_vec4Black);

	//回避のフレームの設定
	m_Avoidance_flameRender.Init("Assets/sprite/avoidance_flame.DDS", 300, 50);
	m_Avoidance_flameRender.SetPosition(AVOIDANCE_FLAME_POS);
	//回避のバーの設定
	m_Avoidance_barRender.Init("Assets/sprite/avoidance_bar.DDS", 194, 26);
	m_Avoidance_barRender.SetPivot(AVOIDANCE_BAR_POVOT);
	m_Avoidance_barRender.SetPosition(AVOIDANCE_BAR_POS);

	game = FindGO<Game>("game");
}

WizardPlayer::~WizardPlayer()
{

}

void WizardPlayer::Update()
{
	//gameクラスのポーズのフラグが立っている間処理を行わない
	if (m_GameState == enPause) {
		return;
	}

	if (gameUI == nullptr)
	{
		gameUI = FindGO<GameUI>("m_gameUI");
	}

	//ゲームのステートがスタート,エンド、リザルトでないなら
	if (game->NowGameState() < 3 && game->NowGameState() != 0)
	{
		//今のフレームと前のフレームのレベルが違っていたら
		if (oldLv != m_lv) {
			//レベルに合わせてGameUIのレベルの画像を変更する
			gameUI->LevelSpriteChange(m_lv);
		}

		oldLv = m_lv;

		//関数にする
		int SkillCoolTime = (int)m_skillTimer;
		wchar_t Skill[255];
		swprintf_s(Skill, 255, L"%d", SkillCoolTime);
		Skillfont.SetText(Skill);

		//前フレームの座標
		oldPosition = m_position;

		//移動処理
		Vector3 stickL;
		stickL.x = g_pad[m_playerNumber]->GetLStickXF();
		stickL.y = g_pad[m_playerNumber]->GetLStickYF();
		Move(m_position, m_charCon, m_status, stickL,m_playerNumber);

		//回避中なら
		if (AvoidanceFlag == true) {
			m_wizardState = enWizardState_Avoidance;
			//移動処理を行う(直線移動のみ)。
			MoveStraight(m_Skill_Right, m_Skill_Forward);
		}

		//スキルクールタイムの処理
		CoolTime(m_skillCoolTime, SkillEndFlag, m_skillTimer);

		//回避クールタイムの処理
		CoolTime(m_avoidanceCoolTime, AvoidanceEndFlag, m_avoidanceTimer);

		//レベルアップする
		//if (g_pad[0]->IsTrigger(/*enButtonLB1*/enButtonA))
		//{
		//	if (m_lv != 10)
		//		ExpProcess(exp);
		//	//m_status.m_getExp += 5;
		//	//m_gameUI->LevelSpriteChange(m_lv);
		//}

		//リスポーンしたときしか使えない
		//飛び降りる処理
		//地上にいないならジャンプしかしないようにする
		if (m_position.y > 1.0f) {
			if (pushFlag == false && m_charCon.IsOnGround() && g_pad[0]->IsTrigger(enButtonA))
			{
				pushFlag = true;
				jampAccumulateflag = true;
				m_wizardState = enWizardState_Jump;
			}
		}
		else
		{
			if (IsEnableMove() != false)
			{
				Attack();
				//回避処理
				Avoidance();
			}

		}

		//回転処理
		Rotation();
		//当たり判定
		Collision();
	}
	else
	{
		m_moveSpeed = Vector3::Zero;
	}
	
	//ステート
	ManageState();
	//アニメーションの再生
	PlayAnimation();

	//クールタイムが終わっていないなら
	if (m_avoidanceTimer != m_avoidanceCoolTime)
	{
		//回避のスプライトの表示の処理
		AvoidanceSprite();
	}

	//キャラクターコントローラーを使って座標を移動させる。
	//ワープする時はキャラコンを移動させない
	if (IsEnableMove() == true)
	{
		//ジャンプの溜め中でないなら
		if (jampAccumulateflag == false)
		{
			m_position = m_charCon.Execute(m_moveSpeed, 1.0f / 60.0f);
		}
		
	}
	
	//ジャンプ中ではないかつ落下中なら
	if (m_wizardState != enWizardState_Jump && m_charCon.IsOnGround()==false)
	{
		m_wizardState = enWizardState_Fall;
	}
	
	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();
}

/// <summary>
/// 攻撃処理
/// </summary>
void WizardPlayer::Attack()
{
	//アタック
	//Aボタンを押したら
	if (pushFlag==false&&g_pad[0]->IsTrigger(enButtonA))
	{
		m_wizardState = enWizardState_Attack;
		//AttackFlag = true;
		//FirstAtkFlag = true;
		pushFlag = true;
	}

	//スキルを発動
	//Bボタンを押したら
	if (pushFlag == false && SkillEndFlag == false && g_pad[0]->IsTrigger(enButtonB))
	{
		//HPを減らしてHPが0になるならワープさせない
		if (m_status.GetHp() <= 10)
		{
			return;
		}
		else
		{
			//HPを10減らす
			int hp = m_status.GetHp() - 10;
			m_status.SetHp(hp);
		}
		m_wizardState = enWizardState_Skill;
		m_isSkillReady = true;
		pushFlag = true;
	}

	//必殺技の発動
	//レベル４以上でＸボタンを押したら
	if (pushFlag == false && m_lv >= 4 && g_pad[0]->IsTrigger(enButtonX))
	{
		pushFlag = true;
		
		//必殺技発動時の処理
		UltimateSkill();
	}
}

/// <summary>
/// 
/// </summary>
void WizardPlayer::Avoidance()
{
	//RBボタンが押されたら。
	//回避
	if (pushFlag == false && AvoidanceEndFlag == false && g_pad[0]->IsTrigger(enButtonRB1)) {
		Vector3 stickL;
		stickL.x = g_pad[0]->GetLStickXF();
		stickL.y = g_pad[0]->GetLStickYF();
		AnimationMove(AvoidanceSpeed, stickL);
		pushFlag = true;
		AvoidanceFlag = true;
	}
}

void WizardPlayer::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//一段目のアタックのアニメーションが始まったら
	if (wcscmp(eventName, L"Attack_Start") == 0)
	{
		//マジックボールを生成する
		MakeMagicBall();
	}

	//ワープが始まったら
	if (wcscmp(eventName, L"Warp_Start") == 0)
	{
		Skill(m_position, m_rot, m_charCon);
		m_modelRender.SetPosition(m_position);
	}

	//ジャンプが始まったら
	if (wcscmp(eventName, L"Jump_Start") == 0)
	{
		m_RespawnJumpFlag = true;
		jampAccumulateflag = false;
	}

	//必殺技が打たれたら
	if (wcscmp(eventName, L"UltAttack_Start") == 0)
	{
		//雷の生成
		MakeUltimateSkill();
	}

}

/// <summary>
/// マジックボールを生成する
/// </summary>
void WizardPlayer::MakeMagicBall()
{
	MagicBall* magicBall = NewGO<MagicBall>(0, "magicBall");
	//製作者の名前を入れる
	magicBall->SetCreatorName(GetName());
	Vector3 MagicBallPos = m_position;
	MagicBallPos.y += 25.0f;

	magicBall->SetPosition(MagicBallPos);
	magicBall->SetRotation(m_rot);
	magicBall->SetEnMagician(MagicBall::enMagician_Player);
}

/// <summary>
/// 必殺技の雷の生成
/// </summary>
void WizardPlayer::MakeUltimateSkill()
{
	for (auto actors : game->GetActors())
	{
		//生成するキャラと自分のオブジェクトの名前が同じなら処理を飛ばす
		if (GetName() == actors->GetName())
		{
			continue;
		}
		//必殺技の雷の生成
		WizardUlt* wizardUlt = NewGO<WizardUlt>(0, "wizardUlt");
		//自分のオブジェクトの名前をセット
		wizardUlt->SetCreatorName(GetName());
		//攻撃するアクターのオブジェクト名をセット
		wizardUlt->SetActor(actors->GetName());
		//攻撃するアクターの座標取得
		Vector3 UltPos = actors->GetPosition();
		UltPos.y += 100.0f;
		wizardUlt->SetPosition(UltPos);
	}
}

void WizardPlayer::AvoidanceSprite()
{
	Vector3 AvoidanceScale = Vector3::One;
	//HPバーの減っていく割合。
	AvoidanceScale.x = (float)m_avoidanceTimer / (float)m_avoidanceCoolTime;
	m_Avoidance_barRender.SetScale(AvoidanceScale);

	m_Avoidance_flameRender.Update();
	m_Avoidance_barRender.Update();
}

void WizardPlayer::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
	//スキルのクールタイムとタイマーが違う時だけ表示
	if (m_skillTimer != m_skillCoolTime)
		Skillfont.Draw(rc);
	//回避のクールタイムとタイマーが違う時だけ表示
	if (m_avoidanceTimer != m_avoidanceCoolTime)
	{
		m_Avoidance_flameRender.Draw(rc);
		m_Avoidance_barRender.Draw(rc);
	}
}

