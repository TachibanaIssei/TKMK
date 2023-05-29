#include "stdafx.h"
#include "KnightPlayer.h"
#include "Game.h"
#include "Neutral_Enemy.h"
#include "KnightUlt.h"
#include "GameUI.h"
#include "Fade.h"
#include "GameCamera.h"
#include "WizardUlt.h"
#include "ChaseEFK.h"
#include "Sounds.h"

//todo
//HP0になってもしなない問題死ぬときにほかのステートに移れないようにする
//たまに死んでも動ける
//ジャンプ
//三段目攻撃も途中にほかのキャラが必殺技を使うとフラグがかわらなくなる
//必殺技が一人しか当たらないときにカメラに処理できていないかも
//ベクタークラスでかんりする

//必殺技中断された時ライトりせっと



namespace {
	const Vector2 AVOIDANCE_BAR_POVOT = Vector2(1.0f,1.0f);
	const Vector3 AVOIDANCE_BAR_POS = Vector3(98.0f, -397.0f, 0.0f);

	const Vector3 AVOIDANCE_FLAME_POS = Vector3(0.0f, -410.0f, 0.0f);

	const Vector3 ATKUPICON_POS = Vector3(-870.0f, -390.0f, 0.0f);
	const Vector3 ATKUPICON_SCALE = Vector3(0.2f, 0.2f, 1.0f);
}

KnightPlayer::KnightPlayer()
{
	
}

KnightPlayer::~KnightPlayer()
{

}

bool KnightPlayer::Start() {
	m_gameUI = FindGO<GameUI>("m_gameUI");
	m_game = FindGO<Game>("game");
	m_fade = FindGO<Fade>("fade");
	

	SetModel();

	//アニメーションイベント用の関数を設定する。
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});
	//リスポーンする座標0番の取得
	GetRespawnPos();
	respawnNumber = 2;        //リスポーンする座標の番号0

	//リスポーンする座標のセット
	//キャラコン
	m_charCon.SetPosition(m_respawnPos[respawnNumber]);
	//剣士
	m_modelRender.SetPosition(m_respawnPos[respawnNumber]);
	m_modelRender.SetRotation(m_respawnRotation[respawnNumber]);

	//リスポーン時に向いている方向の前方向を取得
	ForwardSet();

	m_modelRender.Update();

	//回避のフレームの設定
	m_Avoidance_flameRender.Init("Assets/sprite/avoidance_flame.DDS", 300, 50);
	m_Avoidance_flameRender.SetPosition(AVOIDANCE_FLAME_POS);
	//回避のバーの設定
	m_Avoidance_barRender.Init("Assets/sprite/avoidance_bar.DDS", 194, 26);
	m_Avoidance_barRender.SetPivot(AVOIDANCE_BAR_POVOT);
	m_Avoidance_barRender.SetPosition(AVOIDANCE_BAR_POS);

	m_AtkUpIcon_Render.Init("Assets/sprite/gameUI/attackUP.DDS", 512, 512);
	m_AtkUpIcon_Render.SetPosition(ATKUPICON_POS);
	m_AtkUpIcon_Render.SetScale(ATKUPICON_SCALE);
	m_AtkUpIcon_Render.Update();

	return true;
}

void KnightPlayer::Update()
{
	//アニメーションの再生
	PlayAnimation();
	//無敵時間
	if (Invincible() == false) {
		//当たり判定
		Collition();
	}
	

	// 追尾エフェクトのリセット
	EffectNullptr();
	
	//必殺技を打った時
	if (UltimaitSkillTime() == true) {
		return;
	}
	
	//一度死んだらデスステートのままにする

	//誰かが必殺技を使っているまたは必殺技を打ったアクターが自分でないなら
	if (m_game->GetStopFlag() == true && m_game->GetUltActor() != this)
	{
		//死ぬステートまたはダメージステートなら
		if (m_charState == enCharState_Death || m_charState == enCharState_Damege)
		{
			m_modelRender.Update();
		}
		//抜け出す
		return;
	}

	//gameクラスのポーズのフラグが立っている間処理を行わない
	if (m_GameState == enPause) {
		return;
	}
	
	//ゲームUIがnullなら
	if (m_gameUI == nullptr)
	{
		m_gameUI = FindGO<GameUI>("m_gameUI");
	}

	//やられたらリスポーンするまで実行する
	if (DeathToRespawnTimer(m_DeathToRespwanFlag,m_fade,true) == true)
	{
		//m_charState = enCharState_Idle;
		//アニメーションの再生
		PlayAnimation();
		m_modelRender.Update();
		
		return;
	}

	if (m_charState == enCharState_Death) {
		//アニメーションの再生
		PlayAnimation();
		m_modelRender.Update();
		//ステート
		ManageState();
		return;
	}
	
	//ゲームのステートがスタート,エンド、リザルトでないなら
	if (m_game->NowGameState() < 3 && m_game->NowGameState() != 0)
	{
		//今のフレームと前のフレームのレベルが違っていたら
		if (oldLv != Lv) {
			//レベルに合わせてGameUIのレベルの画像を変更する
			m_gameUI->ChangePlayerLevel(Lv);
			//エフェクトを出す
			IsLevelEffect(oldLv, Lv);
		}
			//前フレームのレベルを取得
			oldLv = Lv;
			//前フレームの座標を取得
			OldPosition = m_position;

			//リスポーンしたときしか使えない
			//飛び降りる処理
			//地上にいないならジャンプしかしないようにする
			if (GetIsGroundFlag() == false) {
				//地上にいるなら
				if (IsActorGroundChack() == true) {
					IsGroundFlag = true;
					//地面に降りた時の音再生
					SetAndPlaySoundSource(enSound_Metal_Falling);
				}
				else {
					//ジャンプ
					if (pushFlag == false && m_charCon.IsOnGround() && g_pad[0]->IsTrigger(enButtonA))
					{
						pushFlag = true;
						m_charState = enCharState_Jump;
					}

				}
			}
			else
			{
				//攻撃処理
				Attack();
				//回避処理
				Avoidance();
				
			}

			//攻撃上昇中
			//AttackUP();

			//移動処理
			Vector3 stickL = Vector3::Zero;
			if (CantMove == false)
			{
				stickL.x = g_pad[0]->GetLStickXF();
				stickL.y = g_pad[0]->GetLStickYF();
			}
			Move(m_position, m_charCon, m_Status, stickL);


			//回避中なら
			if (AvoidanceFlag == true) {
				m_charState = enCharState_Avoidance;
				//移動処理を行う(直線移動のみ)。
				MoveStraight();
			}

			//スキル使用中なら
			if (SkillState == true) {
				//スキルステート
				//m_charState = enCharState_Skill;
				//移動処理を行う(直線移動のみ)。
				MoveStraight();
			}
			//ステート
			ManageState();
			
			//回転処理
			Rotation();

			CoolTimeProcess();
			GrayScaleUI();

		}
		//速度を0にする(動かないようにする)
		else
		{
			m_moveSpeed = Vector3::Zero;
		}



		if (AvoidanceTimer != AvoidanceCoolTime)
		{
			//回避のスプライトの表示の処理
			AvoidanceSprite();
		}

		//キャラクターコントローラーを使って座標を移動させる。
		//ワープする時はキャラコンを移動させない
		if (IsEnableMove() == true) {

			m_position = m_charCon.Execute(m_moveSpeed, 1.0f / 60.0f);
		}

		//ジャンプ中ではないかつ落下中なら
		if (m_charState != enCharState_Jump && m_charCon.IsOnGround() == false)
		{
			m_charState = enCharState_Fall;
		}
		//前方向
		if (m_moveSpeed.LengthSq() != 0.0f) {
			m_forwardNow = m_moveSpeed;
			m_forwardNow.Normalize();
			m_forwardNow.y = 0.0f;
		}

		// レベルをゲームに教える（下部スプライト更新用）
		m_game->UnderSprite_Level(Lv);

		m_modelRender.SetPosition(m_position);
		m_modelRender.Update();
	
}


//攻撃処理
void KnightPlayer::Attack()
{
	//ステートがデスのとき
	if (m_charState == enCharState_Death)
	{
		return;
	}

	//一段目のアタックをしていないなら
	if (pushFlag==false&&AtkState == false)
	{
		//Aボタン押されたら攻撃する
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			m_charState = enCharState_Attack;
			m_game->UnderSprite_Attack();
			pushFlag = true;
			AtkState = true;
		}
	}

	//一段目のアタックのアニメーションがスタートしたなら
	if (m_AtkTmingState == FirstAtk_State)
	{
		if (g_pad[0]->IsTrigger(enButtonA)&& m_AtkTmingState!= SecondAtk_State)
		{
			//ステートを二段目のアタックのアニメーションスタートステートにする
			m_AtkTmingState = SecondAtk_State;
			EffectEmitter* EffectKnight_AttackChack;
			EffectKnight_AttackChack = NewGO <EffectEmitter>(0);
			EffectKnight_AttackChack->Init(EnEFK::enEffect_Knight_AttackChack);
			Vector3 effectPosition = m_position;
			//座標を少し上にする。
			effectPosition.y += 50.0f;
			EffectKnight_AttackChack->SetScale(Vector3::One * 30.0f);
			EffectKnight_AttackChack->SetPosition(effectPosition);
			EffectKnight_AttackChack->Play();
		}
	}
	//二段目のアタックのアニメーションがスタートしたなら
	if (m_AtkTmingState == SecondAtkStart_State)
	{
		if (g_pad[0]->IsTrigger(enButtonA)&&m_AtkTmingState != LastAtk_State)
		{
			//ステートを三段目のアタックのアニメーションスタートステートにする
			m_AtkTmingState = LastAtk_State;
			EffectEmitter* EffectKnight_AttackChack;
			EffectKnight_AttackChack = NewGO <EffectEmitter>(0);
			EffectKnight_AttackChack->Init(EnEFK::enEffect_Knight_AttackChack);
			Vector3 effectPosition = m_position;
			//座標を少し上にする。
			effectPosition.y += 50.0f;
			EffectKnight_AttackChack->SetScale(Vector3::One * 30.0f);
			EffectKnight_AttackChack->SetPosition(effectPosition);
			EffectKnight_AttackChack->Play();
		}
	}


	//スキルを発動する処理
	//Bボタンが押されたら
	if (pushFlag == false && SkillEndFlag==false && SkillState == false && g_pad[0]->IsTrigger(enButtonB)&&m_Status.Hp>0)
	{
		//スキルを使うときのスピードを使う
		AnimationMove(SkillSpeed);
		{
			//エフェクトの座標を更新させる
			m_game->UnderSprite_Skill();

			//剣にまとわせるエフェクト
			if (EffectKnightSkill != nullptr) {
				EffectKnightSkill->DeleteEffect();
			}
			EffectKnightSkill = NewGO <ChaseEFK>(4);
			EffectKnightSkill->SetEffect(EnEFK::enEffect_Knight_Skill, this, Vector3::One * 30.0f);
			EffectKnightSkill->AutoRot(true);
			EffectKnightSkill->SetAutoRotAddY(360.0f);
			// 座標の加算量を計算
			Vector3 effectAddPos = Vector3::Zero;
			effectAddPos.y = 50.0f;
			EffectKnightSkill->SetAddPos(effectAddPos);

			//床のエフェクト
			EffectEmitter* EffectKnightSkillGround_;
			EffectKnightSkillGround_ = NewGO <EffectEmitter>(0);
			EffectKnightSkillGround_->Init(EnEFK::enEffect_Knight_SkillGround);
			EffectKnightSkillGround_->SetScale(Vector3::One * 40.0f);
			EffectKnightSkillGround_->Play();
			Vector3 effectPosition = m_position;
			Quaternion EffRot = m_rot;
			EffRot.AddRotationDegY(360.0f);
			EffectKnightSkillGround_->SetPosition(effectPosition);
			EffectKnightSkillGround_->SetRotation(EffRot);
			EffectKnightSkillGround_->Update();

			//土煙のエフェクト
			if (FootSmoke != nullptr) {
				FootSmoke->DeleteEffect();
			}
			FootSmoke = NewGO<ChaseEFK>(4);
			FootSmoke->SetEffect(EnEFK::enEffect_Knight_FootSmoke, this, Vector3::One * 20.0f);
			FootSmoke->AutoRot(true);

		}
		
		SkillEndFlag = true;
		m_charState = enCharState_Skill;
		pushFlag = true;
		SkillState = true;
	}

	//必殺技を発動する処理
	//Xボタンが押されたら
	if (pushFlag == false && Lv >= 4 && g_pad[0]->IsTrigger(enButtonX))
	{
		//画面を暗くする
		UltimateDarknessFlag = true;
		//ゲーム側に教える
		m_game->UnderSprite_Ult();
		//画面を暗くする
		m_game->SetUltTimeSkyFlag(true);
		//UltimateDarknessFlag = true;
		//魔法陣生成
		CreatMagicCircle();
		pushFlag = true;
		
		//必殺技の溜めステートに移行する
		m_charState = enCharState_Ult_liberation;

		//地面に降りた時の音再生
		SetAndPlaySoundSource(enSound_Knight_Charge_Power);

		Vector3 m_SwordPos = Vector3::Zero;
		Quaternion m_SwordRot;
		//自身をまとうエフェクト
		Ult_Swordeffect = NewGO<EffectEmitter>(0);
		Ult_Swordeffect->Init(enEffect_Knight_Ult_Aura);
		Ult_Swordeffect->SetScale({ 20.0f,40.0f,20.0f });
		Ult_Swordeffect->SetPosition(m_position);
			//エフェクトを再生
		Ult_Swordeffect->Play();
	}
	//攻撃かスキルを使用しているなら
	//コリジョン作成
	if (AtkCollistionFlag == true) AtkCollisiton();
	
}

//必殺技を打っている間の処理
bool KnightPlayer::UltimaitSkillTime()
{
	if (m_UseUltimaitSkillFlag == true)
	{
		if (m_UltshootTimer > 1)
		{
			MakeUltSkill();
		}
		else
		{
			m_UltshootTimer += g_gameTime->GetFrameDeltaTime();
		}
		
		//全ての雷が落ちてから
		//雷を落とすアクターのリストの中身がなくなったら
		if (DamegeUltActor.empty()==true)
		{
			//for (auto actor : m_game->GetActors())
			//{
			//	//必殺技打たれた状態を無くす
			//	actor->ChangeDamegeUltFlag(false);
			//}

			//対象のアクターがいなかったフラグをfalseにする
			m_NoTargetActor = false;
			//レベルを下げる
			UltimateSkill();
			//時間を動かす
			UltEnd();
			m_game->SetStopFlag(false);
			//画面を明るくする
			UltimateDarknessFlag = false;
			//画面を暗くするフラグをfalseにする
			m_game->SetUltTimeSkyFlag(false);
			//画面が暗いのをリセットする
			m_game->LightReset();
			//中身を全て消去する
			DamegeUltActor.clear();
		}
		

		return true;
	}

	return false;
}

/// <summary>
/// 回避処理
/// </summary>
void KnightPlayer::Avoidance()
{
	//ステートがデスのとき
	if (m_charState == enCharState_Death)
	{
		return;
	}
	//RBボタンが押されたら。
	//回避
	if (pushFlag == false && AvoidanceEndFlag == false && AvoidanceFlag == false) {
		if (g_pad[0]->IsTrigger(enButtonRB1) || g_pad[0]->IsTrigger(enButtonLB1)) {
			//回避ステート
			AnimationMove(AvoidanceSpeed);
			pushFlag = true;
			AvoidanceFlag = true;
		}
	}
}

/// <summary>
/// 必殺技の当たり判定生成する
/// </summary>
void KnightPlayer::MakeUltSkill()
{
	//同じやつ二回当ててる
	//必殺技の雷の生成
	for (auto actor : DamegeUltActor)
	{
			//必殺技の雷の生成
			WizardUlt* wizardUlt = NewGO<WizardUlt>(0, "wizardUlt");
			//生成したのがプレイヤーなのでtrue
			wizardUlt->SetThisCreatCharcter(true);
			//自分のオブジェクトの名前をセット
			wizardUlt->SetCreatorName(GetName());
			//攻撃するアクターのオブジェクト名をセット
			wizardUlt->SetActor(actor->GetName());
			//攻撃力を決める
			wizardUlt->SetAboutUlt(Lv);
			//攻撃するアクターの座標取得
			Vector3 UltPos = actor->GetPosition();
			UltPos.y += 100.0f;
			wizardUlt->SetPosition(UltPos);
			wizardUlt->SetGame(m_game);

			//効果音再生
			SetAndPlaySoundSource(enSound_Sword_Ult);

			//雷を落とすキャラがリストの最後なら
			if (actor == DamegeUltActor.back())
			{
				//最後であることを知らせる
				wizardUlt->ChangeUltEndFlag(true);

				//DamegeUltActor.clear();

				m_UltshootTimer = 0.0f;
				//一人ずつ必殺技を打つのでぬける
				return;
			}

			m_UltshootTimer = 0.0f;
			//一人ずつ必殺技を打つのでぬける
			return;
		
	}
	
	//KnightUlt* knightUlt = NewGO<KnightUlt>(0,"knightUlt");
	////製作者の名前を入れる
	//knightUlt->SetCreatorName(GetName());
	//// 制作者を教える
	//knightUlt->SetActor(this);
	//knightUlt->SetUltColorNumb(respawnNumber);
	////キャラのレベルを入れる
	//knightUlt->GetCharLevel(Lv);
	////座標の設定
	//Vector3 UltPos = m_position;
	//UltPos.y += 60.0f;
	//knightUlt->SetPosition(UltPos);
	//knightUlt->SetRotation(m_rot);
	//knightUlt->SetEnUlt(KnightUlt::enUltSkill_Player);
	//knightUlt->SetGame(m_game);

}

void KnightPlayer::CoolTimeProcess()
{
	//スキルクールタイムの処理
	COOlTIME(Cooltime, SkillEndFlag, SkillTimer);

	//回避クールタイムの処理
	COOlTIME(AvoidanceCoolTime, AvoidanceEndFlag, AvoidanceTimer);

}

void KnightPlayer::GrayScaleUI()
{
	if (SkillEndFlag)	m_gameUI->SetSkillIconGrayScale(true);
	else				m_gameUI->SetSkillIconGrayScale(false);

	if (Lv < 4)			m_gameUI->SetULTIconGrayScale(true);
	else				m_gameUI->SetULTIconGrayScale(false);
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
		m_AtkTmingState = FirstAtk_State;
		//剣のコリジョンを生成
		AtkCollistionFlag = true;
		//剣１段目音
		SetAndPlaySoundSource(enSound_ComboONE);

		/*SoundSource* se = NewGO<SoundSource>(0);
		se->Init(enSound_ComboONE);
		se->Play(false);
		se->SetVolume(m_game->GetSoundEffectVolume());*/
	}

	//二段目のアタックのアニメーションが始まったら
	if (wcscmp(eventName, L"SecondAttack_Start") == 0)
	{
		m_AtkTmingState = SecondAtkStart_State;
		//剣のコリジョンを生成
		AtkCollistionFlag = true;
		//剣２段目音
		SetAndPlaySoundSource(enSound_ComboTwo);
	}

	//三段目のアタックのアニメーションが始まったら
	if (wcscmp(eventName, L"LastAttack_Start") == 0)
	{
		m_AtkTmingState = LastAtk_State;
		//剣のコリジョンを生成
		AtkCollistionFlag = true;
		//剣３段目音
		SetAndPlaySoundSource(enSound_ComboThree);
	}
	//三段目のアタックのアニメーションが始まったら
	if (wcscmp(eventName, L"Move_True") == 0)
	{
		CantMove = true;

	}

	//スキルのアニメーションが始まったら
	if (wcscmp(eventName, L"SkillAttack_Start") == 0)
	{
		//剣のコリジョンを生成
		AtkCollistionFlag = true;

		//スキル音を発生
		SetAndPlaySoundSource(enSound_Sword_Skill);
	}
	
	//必殺技のアニメーションが始まったら
	if (wcscmp(eventName, L"timeStop") == 0)
	{
		m_game->SetStopFlag(true);
		m_game->SetUltActor(this);
	}

	//必殺技 剣を空に掲げたら
	if (wcscmp(eventName, L"UltimateAttack_Charge") == 0)
	{
		//雷チャージエフェクト生成
		EffectEmitter* ThunderCharge = NewGO<EffectEmitter>(0);
		ThunderCharge->Init(EnEFK::enEffect_Knight_Thunder_Charge);

		Vector3 ChargePos = Vector3::Zero;
		//「Sword」ボーンのワールド行列を取得する。
		Matrix matrix = m_modelRender.GetBone(m_swordBoneId)->GetWorldMatrix();
		matrix.Apply(ChargePos);

		ChargePos.y += 20.0f;
		ThunderCharge->SetPosition(ChargePos);
		ThunderCharge->SetScale(Vector3::One * 5.0f);
		ThunderCharge->Play();
	}

	//必殺技のアニメーションで剣を空に掲げたら
	if (wcscmp(eventName, L"UltimateAttack_Start") == 0)
	{
		//雷を生成する
		//必殺技使用時の処理ができるようにする
		m_UseUltimaitSkillFlag = true;
		//地上にいるキャラをカウントする
		for (auto actor : m_game->GetActors())
		{
			//名前が自身と同じもしくは一度調べたキャラならやり直す
			if (GetName() == actor->GetName()) {
				continue;
			}

			if (actor->GetIsGroundFlag() == true) {
				//雷を打たれるキャラの情報を入れる
				DamegeUltActor.push_back(actor);
			}
		}

		//プレイヤーのみの処理
		//攻撃対象のアクターがいなかったら
		if (DamegeUltActor.empty() == true) {

			m_NoTargetActor = true;
		}


		//エネルギースラッシュ用
		//必殺技の当たり判定のクラスを作成
		//MakeUltSkill();
		//レベルを下げる
		//UltimateSkill();
		//エフェクトを移動
		//m_swordEffectFlag = false;
		
	}

	//ジャンプのアニメーションが始まったら
	if (wcscmp(eventName, L"Jump_Start") == 0)
	{
		m_RespawnJumpFlag = true;
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
			m_charState = enCharState_Idle;
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
			m_charState = enCharState_Idle;
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
		//カメラを揺らすフラグを立てる
		GameCamera* gameCamera = FindGO<GameCamera>("gamecamera");
		gameCamera->ChangeCameraShakeFlag(true);
	}
	//三段目のアタックのアニメーションで剣を振り終わったら移動できないように
	if (wcscmp(eventName, L"Move_False") == 0)
	{
		CantMove = false;
	}
	///三段目のアタックのアニメーションが終わったら
	if (wcscmp(eventName, L"LastToIdle") == 0)
	{
			//ボタンプッシュフラグをfalseにする
			pushFlag = false;
			AtkState = false;
			m_charState = enCharState_Idle;
			m_AtkTmingState = Num_State;
	}

	//スキルのアニメーションで剣を振り終わったら
	if (wcscmp(eventName, L"SkillAttack_End") == 0)
	{
		//m_Status.Atk -= 20;
		m_AtkTmingState = Num_State;
		AtkState = false;
		//スキルの移動処理をしないようにする
		SkillState = false;
		//剣のコリジョンを生成しない
		AtkCollistionFlag = false;
	}
	//回避アニメーションが終わったら
	if (wcscmp(eventName, L"Avoidance_End") == 0)
	{
		//移動処理をしないようにする
		pushFlag = false;
		AvoidanceFlag = false;
		//m_AtkTmingState = Num_State;
	
	}
}

void  KnightPlayer::IsLevelEffect(int oldlevel, int nowlevel)
{
	if (nowlevel > oldlevel)
	{
		if (LevelUp_efk != nullptr) {
			LevelUp_efk->DeleteEffect();
		}
		LevelUp_efk = NewGO<ChaseEFK>(4);
		LevelUp_efk->SetEffect(EnEFK::enEffect_Knight_LevelUp, this, Vector3::One * 15.0f);

		SetAndPlaySoundSource(enSound_Level_UP);
	}
	else if (nowlevel < oldlevel)
	{
		if (LevelDown_efk != nullptr) {
			LevelDown_efk->DeleteEffect();
		}
		LevelDown_efk = NewGO<ChaseEFK>(4);
		LevelDown_efk->SetEffect(EnEFK::enEffect_Knight_LevelDown, this, Vector3::One * 15.0f);

		SetAndPlaySoundSource(enSound_Level_Down);
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

//サウンドソースの読み込みと再生
void KnightPlayer::SetAndPlaySoundSource(EnSound soundNumber)
{
	SoundSource* se = NewGO<SoundSource>(0);
	se->Init(soundNumber);
	se->SetVolume(m_game->GetSoundEffectVolume());
	se->Play(false);
}

void KnightPlayer::Render(RenderContext& rc)
{

	if (DarwFlag == true) {
		m_modelRender.Draw(rc);
	}
	

	//スキルのクールタイムとタイマーが違う時だけ表示
	/*if(SkillTimer!=Cooltime)
	Skillfont.Draw(rc);*/

	//回避のクールタイムとタイマーが違う時だけ表示
	if (AvoidanceTimer != AvoidanceCoolTime)
	{
		m_Avoidance_flameRender.Draw(rc);
		m_Avoidance_barRender.Draw(rc);
	}
	
	if (m_atkUpSpriteFlag == true) {
		m_AtkUpIcon_Render.Draw(rc);
	}
}
