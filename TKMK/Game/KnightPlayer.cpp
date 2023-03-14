#include "stdafx.h"
#include "KnightPlayer.h"
#include "Game.h"
//#include "GameUI.h"

namespace {
	const Vector2 AVOIDANCE_BAR_POVOT = Vector2(1.0f,1.0f);
	const Vector3 AVOIDANCE_BAR_POS = Vector3(98.0f, -397.0f, 0.0f);

	const Vector3 AVOIDANCE_FLAME_POS = Vector3(0.0f, -410.0f, 0.0f);
}

KnightPlayer::KnightPlayer()
{
	//m_gameUI = FindGO<GameUI>("m_gameUI");

	SetModel();
	//アニメーションイベント用の関数を設定する。
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});
	//リスポーンする座標0番の取得
	GetRespawnPos();
	m_respawnPos[respawnNumber].y += m_position_YUp;
	//リスポーンする座標のセット
	//キャラコン
	m_charCon.SetPosition(m_respawnPos[respawnNumber]);
	//剣士
	m_modelRender.SetPosition(m_respawnPos[respawnNumber]);

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


}

KnightPlayer::~KnightPlayer()
{

}

void KnightPlayer::Update()
{
	//todo
	//gameクラスのポーズのフラグが立っている間処理を行わない
	if (m_playerState == enKnightState_Pause) {
		return;
	}

	int SkillCoolTime = SkillTimer;
	wchar_t Skill[255];
	swprintf_s(Skill, 255, L"%d", SkillCoolTime);
	Skillfont.SetText(Skill);


	//前フレームの座標を取得
	OldPosition = m_position;

	//移動処理ステータスの値が入っていない
	Move(m_position, m_charCon, m_Status);
	
	////RBボタンが押されたら。
	////回避
	//if (AvoidanceEndFlag==false && AvoidanceFlag == false && g_pad[0]->IsTrigger(enButtonRB1)) {
	//	//回避ステート
	//	//m_playerState = enKnightState_Avoidance;
	//	AnimationMove();
	//	AvoidanceFlag = true;
	//}
	
	//回避中なら
	if (AvoidanceFlag == true) {
		m_playerState = enKnightState_Avoidance;
		//移動処理を行う(直線移動のみ)。
		MoveStraight(m_Skill_Right, m_Skill_Forward);
	}

	//攻撃処理
	Attack();

	//回避処理
	Avoidance();

	//スキル使用中なら
	if (SkillState == true) {
		//スキルステート
		m_playerState = enKnightState_Skill;
		//移動処理を行う(直線移動のみ)。
		MoveStraight(m_Skill_Right, m_Skill_Forward);
	}

	//回転処理
	Rotation();

	//スキルクールタイムの処理
	COOlTIME(Cooltime, SkillEndFlag,SkillTimer);

	//回避クールタイムの処理
	COOlTIME(AvoidanceCoolTime, AvoidanceEndFlag, AvoidanceTimer);

	//レベルアップする
	if (g_pad[0]->IsTrigger(/*enButtonLB1*/enButtonA))
	{
		if(Lv!=5)
		ExpProcess(exp);
		//m_gameUI->LevelFontChange(Lv);
	}

	//ダメージを受ける
	/*if (g_pad[0]->IsTrigger(enButtonX))
	{
		Dameged(dddd);
	}*/

	//当たり判定
	Collition();

	//ステート
	ManageState();
	//アニメーションの再生
	PlayAnimation();

	if (AvoidanceTimer != AvoidanceCoolTime)
	{
		//回避のスプライトの表示の処理
		AvoidanceSprite();
	}
	

	//剣士のY座標が腰なのでY座標を上げる
	m_position.y = m_position_YUp;

	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();
}

//攻撃処理
void KnightPlayer::Attack()
{
	//連打で攻撃できなくなる

	//一段目のアタックをしていないなら
	//if (pushFlag==false&&AtkState == false)
	//{
	//	//Bボタン押されたら攻撃する
	//	if (g_pad[0]->IsTrigger(enButtonA))
	//	{
	//		m_playerState = enKnightState_ChainAtk;
	//		
	//		//FirstAtkFlag = true;
	//		//コンボを1増やす
	//		//ComboState++;
	//		pushFlag = true;
	//		AtkState = true;
	//	}
	//}
	//一段目のアタックのアニメーションがスタートしたなら
	if (m_AtkTmingState == FirstAtk_State)
	{
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			//ステートを二段目のアタックのアニメーションスタートステートにする
			m_AtkTmingState = SecondAtk_State;
		}
	}

	if (m_AtkTmingState == SecondAtkStart_State)
	{
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			//ステートを三段目のアタックのアニメーションスタートステートにする
			m_AtkTmingState = LastAtk_State;
		}
	}


	//スキルを発動する処理
	//Bボタンが押されたら
	if (pushFlag == false && SkillEndFlag==false && SkillState == false && g_pad[0]->IsTrigger(enButtonB))
	{
		//移動速度を上げる
		m_Status.Speed += 120.0f;
		
		AnimationMove(SkillSpeed);
		pushFlag = true;
		SkillState = true;
		//AtkCollistionFlag = true;
	}

	//必殺技を発動する処理
	//Xボタンが押されたら
	if (pushFlag == false && Lv >= 4 && g_pad[0]->IsTrigger(enButtonX))
	{
		pushFlag = true;
		//アニメーション再生、レベルを３下げる
		UltimateSkill();
		//必殺技発動フラグをセット
		UltimateSkillFlag = true;
	}

	//必殺技発動フラグがセットされているなら
	if (UltimateSkillFlag == true)
	{
		UltimateSkillTimer += g_gameTime->GetFrameDeltaTime();
		//必殺技タイマーが3.0fまでの間
		if (UltimateSkillTimer <= 3.0f)
		{
			//コリジョンの作成、移動処理
			UltimateSkillCollistion(OldPosition, m_position);
		}
		else
		{
			//攻撃が有効な時間をリセット
			UltimateSkillTimer = 0;
			//必殺技発動フラグをリセット
			UltimateSkillFlag = false;
			//コリジョン削除
			DeleteGO(collisionObject);
			//コリジョン作成フラグをリセット
			UltCollisionSetFlag = false;
		}
	}

	//攻撃かスキルを使用しているなら
	//コリジョン作成
	if(AtkCollistionFlag ==true)AtkCollisiton();
}

/// <summary>
/// 回避処理
/// </summary>
void KnightPlayer::Avoidance()
{
	//RBボタンが押されたら。
	//回避
	if (pushFlag == false && AvoidanceEndFlag == false && AvoidanceFlag == false && g_pad[0]->IsTrigger(enButtonRB1)) {
		//回避ステート
		//m_playerState = enKnightState_Avoidance;
		AnimationMove(AvoidanceSpeed);
		pushFlag = true;
		AvoidanceFlag = true;
	}
}

/// <summary>
/// アニメーションイベントの再生
/// </summary>
/// <param name="clipName"></param>
/// <param name="eventName"></param>
void KnightPlayer::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//一段目のアタックのアニメーションが始まったら
	if (wcscmp(eventName, L"FirstAttack_Start") == 0)
	{
		m_AtkTmingState =FirstAtk_State;
		//剣のコリジョンを生成
		AtkCollistionFlag = true;
	}
	//二段目のアタックのアニメーションが始まったら
	if (wcscmp(eventName, L"SecondAttack_Start") == 0)
	{
		m_AtkTmingState = SecondAtkStart_State;
		//剣のコリジョンを生成
		AtkCollistionFlag = true;
	}
	//三段目のアタックのアニメーションが始まったら
	if (wcscmp(eventName, L"LastAttack_Start") == 0)
	{
		m_AtkTmingState = LastAtk_State;
		//剣のコリジョンを生成
		AtkCollistionFlag = true;
	}
	//スキルのアニメーションが始まったら
	if (wcscmp(eventName, L"SkillAttack_Start") == 0)
	{
		m_Status.Atk += 20;
		//m_AtkTmingState = LastAtk_State;
		//剣のコリジョンを生成
		AtkCollistionFlag = true;
	}
	//////////////////////////////////////////////////////////////////////////
	//一段目のアタックのアニメーションで剣を振り終わったら
	if (wcscmp(eventName, L"FirstAttack_End") == 0)
	{
		
		//剣のコリジョンを生成しない
		AtkCollistionFlag = false;
	}
	///一段目のアタックのアニメーションが終わったら
	if (wcscmp(eventName, L"FirstToIdle") == 0)
	{
		//ボタンが押されていなかったら
		if (m_AtkTmingState != SecondAtk_State)
		{
			//ボタンプッシュフラグをfalseにする
			pushFlag = false;
			AtkState = false;
			m_playerState = enKnightState_Idle;
			m_AtkTmingState = Num_State;
		}
	}

	//二段目のアタックのアニメーションで剣を振り終わったら
	if (wcscmp(eventName, L"SecondAttack_End") == 0)
	{
		
		//剣のコリジョンを生成しない
		AtkCollistionFlag = false;
		//ボタンが押されていなかったら
		if (m_AtkTmingState != LastAtk_State)
		{
			//ボタンプッシュフラグをfalseにする
			pushFlag = false;
			AtkState = false;
			m_playerState = enKnightState_Idle;
			m_AtkTmingState = Num_State;
		}
	}
	//三段目のアタックのアニメーションで剣を振り終わったら
	if (wcscmp(eventName, L"LastAttack_End") == 0)
	{
		m_AtkTmingState = Num_State;
		AtkState = false;
		//剣のコリジョンを生成しない
		AtkCollistionFlag = false;
	}
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false) {
		m_playerState = enKnightState_Idle;
		AtkState = false;
		//ボタンプッシュフラグをfalseにする
		pushFlag = false;
	}

	//スキルのアニメーションで剣を振り終わったら
	if (wcscmp(eventName, L"SkillAttack_End") == 0)
	{
		m_Status.Atk -= 20;
		m_AtkTmingState = Num_State;
		AtkState = false;
		//スキルの移動処理をしないようにする
		SkillState = false;
		m_Status.Speed -= 120.0f;
		//剣のコリジョンを生成しない
		AtkCollistionFlag = false;
	}
	//回避アニメーションが終わったら
	if (wcscmp(eventName, L"Avoidance_End") == 0)
	{
		//移動処理をしないようにする

		AvoidanceFlag = false;
		//m_AtkTmingState = Num_State;
	
	}
}

void KnightPlayer::AvoidanceSprite()
{
	Vector3 AvoidanceScale = Vector3::One;
	//HPバーの減っていく割合。
	AvoidanceScale.x = (float)AvoidanceTimer / (float)AvoidanceCoolTime;
	m_Avoidance_barRender.SetScale(AvoidanceScale);

	m_Avoidance_flameRender.Update();
	m_Avoidance_barRender.Update();
}

void KnightPlayer::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
	//スキルのクールタイムとタイマーが違う時だけ表示
	if(SkillTimer!=Cooltime)
	Skillfont.Draw(rc);
	//回避のクールタイムとタイマーが違う時だけ表示
	if (AvoidanceTimer != AvoidanceCoolTime)
	{
		m_Avoidance_flameRender.Draw(rc);
		m_Avoidance_barRender.Draw(rc);
	}
	
}
