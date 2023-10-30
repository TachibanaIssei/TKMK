#include "stdafx.h"
#include "KnightBase.h"
#include "Status.h"
#include "GameUI.h"
#include "Player.h"
#include "Effect.h"
#include "Game.h"
#include "Sounds.h"

KnightBase::KnightBase()
{
	//ステータスを読み込む
	m_status.Init("Knight");
	m_InitialStatus = m_status;  //初期ステータスのセット
	m_lv=1;                    //レベル
	m_attackSpeed=20;              //攻撃速度

	m_skillCoolTime = 5.0f;            //スキルのクールタイム
	m_skillTimer = m_skillCoolTime;

	m_avoidanceCoolTime = 2;     ///回避のクールタイム
	m_avoidanceTimer = m_avoidanceCoolTime;

	m_point=0;                 //敵を倒して手に入れたポイント
	m_getExp=0;                //中立の敵を倒したときの経験値
	m_expTable=5;              //経験値テーブル
	//m_respawnNumber = 0;        //リスポーンする座標の番号

	IsGroundFlag = false;    //地上にいないのでfalse
}

KnightBase::~KnightBase()
{

}

void KnightBase::SetModel()
{
	//プレイヤー
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
	m_animationClips[enAnimationClip_lastAtk].Load("Assets/animData/Knight/ChainAttack/last_Attack2.tka");
	m_animationClips[enAnimationClip_lastAtk].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Skill].Load("Assets/animData/Knight/Knight_Skill.tka");
	m_animationClips[enAnimationClip_Skill].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Ult_liberation].Load("Assets/animData/Knight/Knight_Ult_liberation.tka");
	m_animationClips[enAnimationClip_Ult_liberation].SetLoopFlag(false);
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

	//剣士モデルを読み込み
	//m_modelRender.Init("Assets/modelData/character/Knight/model_Knight.tkm", m_animationClips, enAnimationClip_Num, enModelUpAxisZ);
	
	//剣士モデルを読み込み
	switch (m_knightKind)
	{
	case enKnightKinds_Red:
		m_modelRender.Init("Assets/modelData/character/Knight/Knight_Red2.tkm", m_animationClips, enAnimationClip_Num, enModelUpAxisZ);
		break;
	case enKnightKinds_Blue:
		m_modelRender.Init("Assets/modelData/character/Knight/Knight_Blue2.tkm", m_animationClips, enAnimationClip_Num, enModelUpAxisZ);
		break;
	case enKnightKinds_Green:
		m_modelRender.Init("Assets/modelData/character/Knight/Knight_Green2.tkm", m_animationClips, enAnimationClip_Num, enModelUpAxisZ);
		break;
	case enKnightKinds_Yellow:
		m_modelRender.Init("Assets/modelData/character/Knight/Knight_Yellow2.tkm", m_animationClips, enAnimationClip_Num, enModelUpAxisZ);
		break;

	}
	
	//「mixamorig:RightHand」(右手)ボーンのID(番号)を取得する。
	m_swordBoneId = m_modelRender.FindBoneID(L"mixamorig:RightHand");
	//攻撃時のアニメーションイベント剣士の座標のID(番号)を取得する。
	AtkEndPosId = m_modelRender.FindBoneID(L"movePos");


	m_position = { 0.0f,0.0f,0.0f };
	m_modelRender.SetPosition(m_position);
	//m_modelRender.SetScale(Vector3(0.1f, 0.1f, 0.1f));

	m_rot.SetRotationY(0.0f);
	m_modelRender.SetRotation(m_rot);

	m_charCon.Init(
		15.0f,
		35.0f,
		m_position
	);

	player = FindGO<Player>("player");
}

/// <summary>
/// 中立の敵を倒したときの経験値の処理
/// </summary>
/// <param name="m_getExp">中立の敵の経験値</param>
void KnightBase::ExpProcess(int Exp)
{
	//もしレベルが10(Max)なら
	if (m_lv == 10)return;
	//自身の経験値に敵を倒したときに手に入れる経験値を足す
	m_getExp += Exp;
	//手に入れた経験値より経験値テーブルのほうが大きかったら
	if (m_getExp < m_expTable) return;      //抜け出す
	else {
		//経験値テーブルより手に入れた経験値のほうが大きかったら
		//レベルアップ
		LevelUp(m_lv);
		//レベルに合わせてレベルの画像を変更する
		m_gameUI->LevelSpriteChange(m_lv);
		switch (m_lv)
		{
		case 2:
			m_expTable = 10;
			break;
		case 3:
			m_expTable = 20;
			break;
		case 4:
			m_expTable = 30;
			break;
		case 5:
			m_expTable = 40;
			break;
		case 6:
			m_expTable = 50;
			break;
		case 7:
			m_expTable = 60;
			break;
		case 8:
			m_expTable = 70;
			break;
		case 9:
			m_expTable = 80;
			break;
		default:
			break;
		}
	}
}

/// <summary>
/// 回転処理
/// </summary>
void KnightBase::Rotation()
{
	if (CantMove) {
		return;
	}

	//xかzの移動速度があったら(スティックの入力があったら)。
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		//キャラクターの方向を変える。
		m_rot.SetRotationYFromDirectionXZ(m_moveSpeed);
		//回転を教える。
		m_modelRender.SetRotation(m_rot);
	}
}

bool KnightBase::DrawHP(const int playerNumber)
{
	Vector3 toCameraTarget = g_camera3D[playerNumber]->GetTarget() - g_camera3D[playerNumber]->GetPosition();
	Vector3 toMush = m_position - g_camera3D[playerNumber]->GetPosition();
	toCameraTarget.y = 0.0f;
	toMush.y = 0.0f;
	toCameraTarget.Normalize();
	toMush.Normalize();

	float cos = Dot(toCameraTarget, toMush);
	float angle = acos(cos);

	//カメラの後ろにあるなら描画しない
	Vector3 diff = m_player[playerNumber]->GetPosition() - m_position;

	//プレイヤーに向かう距離を計算する
	float playerdistance = diff.Length();

	if (fabsf(angle) < Math::DegToRad(45.0f) && playerdistance < 800.0f && m_player[playerNumber]->GetPosition().y <= 10.0f)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool KnightBase::Invincible()
{
	if (m_invincibleTimer > 0)
	{
		m_invincibleTimer -= g_gameTime->GetFrameDeltaTime();
		return true;
	}
	m_invincibleTimer = 0.0f;
	return false;
}

/// <summary>
/// 攻撃時の当たり判定の処理
/// </summary>
void KnightBase::AtkCollisiton()
{
	//コリジョンオブジェクトを作成する。
	auto collisionObject = NewGO<CollisionObject>(0);
	Vector3 collisionPosition = m_position;
	//座標をプレイヤーの少し前に設定する。
	//collisionPosition += forward * 50.0f;
	//�{�b�N�X��̃R���W������쐬����B
	collisionObject->CreateBox(collisionPosition, //��W�B
		Quaternion::Identity, //��]�B
		Vector3(85.0f, 15.0f, 30.0f) //�傫���B
	);
	collisionObject->SetName("player_attack");
	collisionObject->SetCreatorName(GetName());

	//「Sword」ボーンのワールド行列を取得する。
	Matrix matrix = m_modelRender.GetBone(m_swordBoneId)->GetWorldMatrix();

	//matrix.MakeRotationZ(90.0f);
	//「Sword」ボーンのワールド行列をコリジョンに適用する。
	collisionObject->SetWorldMatrix(matrix);
}


/// <summary>
/// 必殺技発動時の当たり判定の処理
/// </summary>
/// <param name="oldpostion">前フレームの座標</param>
/// <param name="position">現在の座標</param>
void KnightBase::UltimateSkillCollistion(Vector3& oldpostion,Vector3& position)
{	
	
}

void KnightBase::Collision()
{
	if (m_invincibleTimer > 0)
	{
		return;
	}
	//被ダメージ、ダウン中、必殺技、通常攻撃時はダメージ判定をしない。
	if (m_charState == enCharState_Damege || 
		m_charState == enCharState_Death ||
		m_charState == enCharState_UltimateSkill ||
		//m_charState == enCharState_Ult_liberation ||
		m_charState == enCharState_Avoidance)
	{
		return;
	}

	//敵の攻撃用のコリジョンを取得する名前一緒にする
	const auto& Knightcollisions = g_collisionObjectManager->FindCollisionObjects("player_attack");
	//コリジョンの配列をfor文で回す
	for (auto knightcollision : Knightcollisions)
	{
		//このコリジョンを作ったアクターを検索
		m_lastAttackActor = FindGO<Actor>(knightcollision->GetCreatorName());

		//コリジョンを作ったアクターが自分でないなら
		if (knightcollision->IsHit(m_charCon)&& m_lastAttackActor!=this)
		{
			//被ダメージエフェクト再生
			EffectEmitter* EffectKnight_Attack;
			EffectKnight_Attack = NewGO <EffectEmitter>(0);
			EffectKnight_Attack->Init(EnEFK::enEffect_Knight_Attack);
			EffectKnight_Attack->SetScale(Vector3::One * 50.0f);
			EffectKnight_Attack->Play();
			if (m_lastAttackActor->NowCharState() == Actor::enCharState_Attack)
			{
				//逆向きにする
				Quaternion Damegerot=m_rot;
				Damegerot.AddRotationDegZ(180.0f);
				EffectKnight_Attack->SetRotation(Damegerot);
			}
			else if (m_lastAttackActor->NowCharState() == Actor::enCharState_SecondAttack)
			{
				//逆向きにする
				Quaternion Damegerot = m_rot;
				Damegerot.AddRotationDegZ(270.0f);
				EffectKnight_Attack->SetRotation(Damegerot);
			}
			else
			{
				//縦向きにする
				Quaternion Damegerot = m_rot;
				Damegerot.AddRotationDegZ(180.0f);
				EffectKnight_Attack->SetRotation(Damegerot);
			}
			Vector3 damegePosition = m_position;
			damegePosition.y += 50.0f;
			EffectKnight_Attack->SetPosition(damegePosition);
			

			//もし必殺技の溜め中だったら
			if (m_charState == enCharState_Ult_liberation) {
				//エフェクトを止める
				Ult_Swordeffect->Stop();
				MagicCircle->Stop();
				DeleteGO(Ult_Swordeffect);
				DeleteGO(MagicCircle);

				//画面を暗くするフラグをfalseにする
				UltimateDarknessFlag = false;
				m_game->SetUltTimeSkyFlag(false);
				//画面が暗いのをリセットする
				m_game->LightReset();
			}

			//無敵時間リセット
			m_invincibleTimer = 1.0f;
			//ダメージを受ける、やられたら自分を倒した相手にポイントを与える
			Dameged(m_lastAttackActor->GetAtk(), m_lastAttackActor);
			return;
		}
	}
	//必殺技のコリジョンを取得する
	const auto& Ultcollisions = g_collisionObjectManager->FindCollisionObjects("player_UltimateSkill");
	//コリジョンの配列をfor文で回す
	for (auto collision : Ultcollisions)
	{
		if (collision->IsHit(m_charCon))
		{
			//このコリジョンを作ったアクターを検索
			m_lastAttackActor = FindGO<Actor>(collision->GetCreatorName());
			//コリジョンを作ったアクターが自分でないなら
			if (collision->IsHit(m_charCon) && m_lastAttackActor != this)
			{
				//無敵時間リセット
				m_invincibleTimer = 1.0f;
				//ダメージを受ける、やられたら自分を倒した相手にポイントを与える
				Dameged(300, m_lastAttackActor);
				return;
			}
		}
	}

	//被ダメージ、ダウン中、必殺技、通常攻撃時はダメージ判定をしない。
	//エネミーの攻撃はのけぞらないようにする
	if (m_charState == enCharState_Death ||
		m_charState == enCharState_Attack||
		m_charState == enCharState_SecondAttack||
		m_charState == enCharState_LastAttack||
		m_charState == enCharState_Skill||
		m_charState == enCharState_Avoidance||
		m_charState == enCharState_UltimateSkill ||
		m_charState == enCharState_Ult_liberation
		)
	{
		return;
	}
	//敵の攻撃用のコリジョンを取得する名前一緒にする
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("enemy_attack");
	//コリジョンの配列をfor文で回す
	for (auto collision : collisions)
	{
		//コリジョンが自身のキャラコンに当たったら
		if (collision->IsHit(m_charCon))
		{
			//無敵時間リセット
			m_invincibleTimer = 1.0f;
			//hpを10減らす
			Dameged(Enemy_atk, m_Neutral_enemy);
			return;
		}
	}
}

/// <summary>
/// ダメージを受けたときの処理
/// </summary>
/// <param name="damege">敵の攻撃力</param>
void KnightBase::Dameged(int damege, Actor* CharGivePoints)
{
	int hp = m_status.GetHp() - damege;
	m_status.SetHp(hp);
	//無敵時間リセット
	//m_invincibleTimer = 1.0f;

	//もしスキルが使用中ならスキルの移動処理を無くす
	if (m_isSkillReady == true)
	{
		AtkState = false;
		SkillEndFlag = true;
		//ボタンプッシュフラグをfalseにする
		pushFlag = false;
		m_isSkillReady = false;
	}

	//自身のHPが0以下なら
	if (m_status.GetHp() <= 0) {
		//倒されたときの処理に遷移
		//死亡ステート
		m_charState = enCharState_Death;

		EffectEmitter* EffectKnightDeath;
		EffectKnightDeath = NewGO <EffectEmitter>(0);
		EffectKnightDeath->Init(EnEFK::enEffect_Knight_Death);
		EffectKnightDeath->SetScale(Vector3::One * 20.0f);
		
		Vector3 effectPosition = m_position;
		//座標を少し上にする。
		effectPosition.y += 45.0f;
		effectPosition += m_forwardNow * -10;
		EffectKnightDeath->SetPosition(effectPosition);
		EffectKnightDeath->Play();

		//地上にいない
		IsGroundFlag = false;

		//デスボイス再生
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(enSound_Knight_Death);
		se->Play(false);
		//プレイヤーとの距離によって音量調整
		se->SetVolume(SoundSet(player, m_game->GetSoundEffectVolume(), 0.0f));

		m_status.SetHp(0);
		//攻撃された相手が中立の敵以外なら
		if (CharGivePoints != nullptr)
		{
			//倒された相手のポイントを増やす
			CharGivePoints->PointProcess(m_lv);
		}
	}
	else {
		//ダメージステート
		m_charState = enCharState_Damege;

		// 攻撃関連の初期化
		AtkState = false;
		AtkCollistionFlag = false;
		m_AtkTmingState = Num_State;
		pushFlag = false;
		CantMove = false;

		SoundSource * se = NewGO<SoundSource>(0);
		se->Init(enSound_Knight_Receiving_Damage);
		se->Play(false);
		//プレイヤーとの距離によって音量調整
		se->SetVolume(SoundSet(player, m_game->GetSoundEffectVolume(), 0.0f));
		//無敵時間フラグ
		//m_invincibleFlag = true;
	}
}

/// <summary>
/// レベル4で必殺技を使用したときの処理
/// </summary>
void KnightBase::UltimateSkill()
{
	int DownLv = m_lv - 1;
	
	//レベルを1に下げる
	levelDown(m_lv, DownLv);
	//経験値をリセット
	ExpReset(m_lv, m_getExp);
	//レベルの経験値テーブルにする
	ExpTableChamge(m_lv, m_expTable);
}

/// <summary>
/// リスポーンする座標のセット
/// </summary>
void KnightBase::SetRespawn()
{
	//リスポーンする座標0番の取得
	GetRespawnPos();
	//リスポーンする座標のセット
	m_position = m_respawnPos[m_respawnNumber];
	//キャラコン
	m_charCon.SetPosition(m_respawnPos[m_respawnNumber]);
	//剣士
	m_modelRender.SetPosition(m_respawnPos[m_respawnNumber]);
	m_modelRender.SetRotation(m_respawnRotation[m_respawnNumber]);

	//リスポーン時に向いている方向の前方向を取得
	ForwardSet();
}

/// <summary>
/// 自身が倒されたときの処理
/// </summary>
void KnightBase::Death()
{
	//レベルを１下げる
	levelDown(m_lv,1);
	//HPを最大にする
	m_status.SetHp(m_status.GetMaxHp());
	//経験値をリセット
	ExpReset(m_lv, m_getExp);
	//一つ下のレベルの経験値テーブルにする
	ExpTableChamge(m_lv,m_expTable);
}

/// <summary>
/// アニメーション再生時に直線移動させる方向の決定
/// </summary>
void KnightBase::AnimationMove(float Speed)
{
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;

	//移動速度を計算。
	m_Skill_MoveSpeed = Vector3::AxisZ;
	m_rot.Apply(m_Skill_MoveSpeed);
	//移動速度を決める
	m_Skill_MoveSpeed *= Speed;
}

//直線移動させる
void KnightBase::MoveStraight()
{
	//キャラクターコントローラーを使って座標を移動させる。
	m_position = m_charCon.Execute(m_Skill_MoveSpeed, 1.0f / 60.0f);
}

void KnightBase::CreatMagicCircle()
{
	MagicCircle = NewGO<EffectEmitter>(0);
	MagicCircle->Init(EnEFK::enEffect_MasicCircle);
	Vector3 CirclePos = m_position;
	CirclePos.y += 3.0f;
	MagicCircle->SetPosition(CirclePos);
	MagicCircle->SetScale(Vector3::One * 30.0f);
	MagicCircle->Play();
}


/// <summary>
/// アニメーション再生の処理
/// </summary>
void KnightBase::PlayAnimation()
{
	m_modelRender.SetAnimationSpeed(1.0f);

	switch (m_charState)
	{
	//待機
	case enCharState_Idle:
		m_modelRender.PlayAnimation(enAnimationClip_Idle,0.2f);
		break;
	//歩き
	case enCharState_Walk:
		m_modelRender.PlayAnimation(enAnimationClip_Walk, 0.1f);
		break;
	//走る
	case enCharState_Run:
		m_modelRender.PlayAnimation(enAnimationClip_Run,0.2f);
		break;
	//ジャンプ
	case enCharState_Jump:
		m_modelRender.PlayAnimation(enAnimationClip_Jump, 0.2f);
		break;
	//落ちる
	case enCharState_Fall:
		m_modelRender.SetAnimationSpeed(0.9f);
		m_modelRender.PlayAnimation(enAnimationClip_Fall, 0.0f);
		break;
	//一段目
	case enCharState_Attack:
		m_modelRender.PlayAnimation(enAnimationClip_ChainAtk, 0.1f);
		m_modelRender.SetAnimationSpeed(1.4f);
		break;
	//二段目
	case enCharState_SecondAttack:
		m_modelRender.PlayAnimation(enAnimationClip_SecondAtk, 0.1f);
		m_modelRender.SetAnimationSpeed(1.4f);
		break;
	//三段目
	case enCharState_LastAttack:
		m_modelRender.PlayAnimation(enAnimationClip_lastAtk, 0.1f);
		m_modelRender.SetAnimationSpeed(1.3f);
		break;
	//スキル
	case enCharState_Skill:
		m_modelRender.PlayAnimation(enAnimationClip_Skill, 0.3f);
		break;
	//必殺技の溜め
	case enCharState_Ult_liberation:
		m_modelRender.PlayAnimation(enAnimationClip_Ult_liberation);
		break;
	//必殺技
	case enCharState_UltimateSkill:
		//ここ調整必要！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
		m_modelRender.SetAnimationSpeed(0.8f);
		m_modelRender.PlayAnimation(enAnimationClip_UltimateSkill,0.1f);
		break;
	//回避
	case enCharState_Avoidance:
		m_modelRender.SetAnimationSpeed(1.5f);
		m_modelRender.PlayAnimation(enAnimationClip_Avoidance, 0.1f);
		break;
	//被ダメージ
	case enCharState_Damege:
		m_modelRender.PlayAnimation(enAnimationClip_Damege, 0.4f);
		break;
	//デス
	case enCharState_Death:
		m_modelRender.PlayAnimation(enAnimationClip_Death);
	default:
		break;
	}
}

/// <summary>
/// アニメーションのステートの処理 m_charStateとm_charState両方あったらアニメーション可
/// </summary>
void KnightBase::ManageState()
{
	switch (m_charState)
	{
	case enCharState_Idle:
		OnProcessIdleStateTransition();
		break;
	case enCharState_Walk:
		OnProcessIdleStateTransition();
		break;
	case enCharState_Run:
		OnProcessRunStateTransition();
		break;
	case enCharState_Jump:
		OnProcessJumpStateTransition();
		break;
	case enCharState_Fall:
		OnProcessFallStateTransition();
		break;
	case enCharState_Attack:
		OnProcessChainAtkStateTransition();
		break;
	case enCharState_SecondAttack:
		OnProcessSecondAtkStateTransition();
		break;
	case enCharState_LastAttack:
		OnProcessLastAtkStateTransition();
		break;
	case enCharState_Skill:
		OnProcessSkillAtkStateTransition();
		break;
	case enCharState_Ult_liberation:
		OnProcessUlt_liberationStateTransition();
		break;
	case enCharState_UltimateSkill:
		OnProcessUltimateSkillAtkStateTransition();
		break;
	case enCharState_Avoidance:
		OnProcessAvoidanceStateTransition();
		break;
	case enCharState_Damege:
		OnProcessDamegeStateTransition();
		break;
	case enCharState_Death:
		OnProcessDeathStateTransition();
		break;

	}
}

/// <summary>
/// 歩きアニメーションが再生されているなら。
/// </summary>
void KnightBase::OnProcessCommonStateTransition()
{
	//スティックの入力量があったら
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		if (m_lv < 2) {
			m_charState = enCharState_Walk;
		}
		else
			//走りステート
		m_charState = enCharState_Run;

		return;
	}
	else
	{
		//なかったら待機ステート
		m_charState = enCharState_Idle;
		return;
	}
}

/// <summary>
/// Idleアニメーションが再生されているときの処理
/// </summary>
void KnightBase::OnProcessIdleStateTransition()
{
	pushFlag = false;
	AtkState = false;
	OnProcessCommonStateTransition();
}

/// <summary>
/// Runアニメーションが再生されているときの処理
/// </summary>
void KnightBase::OnProcessRunStateTransition()
{
	OnProcessCommonStateTransition();
}

//ジャンプアニメーションが再生されているとき
void KnightBase::OnProcessJumpStateTransition()
{
	pushFlag = false;
	//空中にいるなら
	if (IsAir(m_charCon) == enIsAir && m_charCon.IsOnGround() == false)
	{
		m_AirFlag = true;
	}
	if (m_AirFlag == true)
	{
		if (m_charCon.IsOnGround() == true)
		{
			pushFlag = false;
			m_AirFlag = false;
			m_charState = enCharState_Idle;
			OnProcessCommonStateTransition();
		}

	}
	// 応急処置
	if (m_position.y < 10) {
		m_charState = enCharState_Idle;
		OnProcessCommonStateTransition();
	}

	if (!m_modelRender.IsPlayingAnimation() && m_position.y <= 500.0f)
	{
		m_charState = enCharState_Fall;
	}
}

/// <summary>
/// FirstAtkアニメーションが再生されているときの処理
/// </summary>
void KnightBase::OnProcessChainAtkStateTransition()
{
	//チェインアタックのアニメーション再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//二段目のアタック入力をしていたなら
		if (m_AtkTmingState == SecondAtk_State)
		{
			//攻撃を二段目にする
			m_charState = enCharState_SecondAttack;
		}
		else
		{
			//待機ステート
			//攻撃を始めたかの判定をfalseにする
			AtkState = false;
			//ボタンプッシュフラグをfalseにする
			pushFlag = false;
			m_charState = enCharState_Idle;
			OnProcessCommonStateTransition();
		}
	}
}

/// <summary>
/// SecondAtkアニメーションが再生されているときの処理
/// </summary>
void KnightBase::OnProcessSecondAtkStateTransition()
{
	//チェインアタックのアニメーション再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//二段目のアタック入力をしていたなら
		if (m_AtkTmingState == LastAtk_State)
		{
			//攻撃を二段目にする
			m_charState = enCharState_LastAttack;

			LastAttackMove();
		}
		else
		{
			//待機ステート
		//攻撃を始めたかの判定をfalseにする
			AtkState = false;
			//ボタンプッシュフラグをfalseにする
			pushFlag = false;
			m_charState = enCharState_Idle;
			OnProcessCommonStateTransition();
		}
	}
}

/// <summary>
/// LastAtkアニメーションが再生されているときの処理
/// </summary>
void KnightBase::OnProcessLastAtkStateTransition()
{
	//アニメーション再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//待機ステート
		//攻撃を始めたかの判定をfalseにする
		AtkState = false;
		//ボタンプッシュフラグをfalseにする
		pushFlag = false;
		CantMove = false;
		m_charState = enCharState_Idle;
		OnProcessCommonStateTransition();
	}
}

/// <summary>
/// スキルのアニメーション再生されているときの処理
/// </summary>
void KnightBase::OnProcessSkillAtkStateTransition()
{
	//スキルのアニメーション再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		AtkState = false;
		SkillEndFlag = true;
		//ボタンプッシュフラグをfalseにする
		pushFlag = false;
		m_isSkillReady = false;
		//待機ステート
		m_charState = enCharState_Idle;
		OnProcessCommonStateTransition();
	}
}

/// <summary>
/// 必殺技の溜めアニメーションが再生されているときの処理
/// </summary>
void KnightBase::OnProcessUlt_liberationStateTransition()
{
	//必殺技の溜めのアニメーション再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//AIがカメラで見られるようにする
		ChangeChaseCamera(true);
		//必殺技ステートに移行
		m_charState = enCharState_UltimateSkill;
		

		//アルティメットSE
		////プレイヤーとの距離によって音量調整
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(enSound_Hand);
		se->SetVolume(SoundSet(player, m_game->GetSoundEffectVolume(), 0.0f));
		se->Play(false);
	
		
	}
}

/// <summary>
/// 必殺技アニメーションが再生されているときの処理
/// </summary>
void KnightBase::OnProcessUltimateSkillAtkStateTransition()
{
	//必殺技アニメーション再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		
	}
}

//回避のアニメーションが再生されているときの処理
void KnightBase::OnProcessAvoidanceStateTransition()
{
	//回避のアニメーション再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		AvoidanceFlag = false;
		AvoidanceEndFlag = true;
		//ボタンプッシュフラグをfalseにする
		pushFlag = false;
		//待機ステート
		OnProcessCommonStateTransition();
	}
}

/// <summary>
/// Damegeアニメーションが再生されているときの処理
/// </summary>
void KnightBase::OnProcessDamegeStateTransition()
{
	//ダメージを受けたときのアニメーション再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		OnProcessCommonStateTransition();
	}
}

/// <summary>
/// Deathアニメーションが再生されているときの処理
/// </summary>
void KnightBase::OnProcessDeathStateTransition()
{
	//Deathアニメーション再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		EffectEmitter* DeathOrb = NewGO<EffectEmitter>(0);
		DeathOrb->Init(EnEFK::enEffect_Knight_Death_Blue);
		DeathOrb->SetPosition(m_position);
		DeathOrb->SetScale(Vector3::One * 10.0f);
		DeathOrb->Play();

		//リスポーンする座標に自身の座標をセット
		SetRespawn();
		Death();
		pushFlag = false;
		AtkState = false;
		CantMove = false;
		AvoidanceFlag = false;
		//リスポーン待機フラグを立てる
		m_DeathToRespwanFlag = true;
		//リスポーンするまでの時間を設定
		m_respwanTimer = 3.0f;
		//待機ステート
		//m_charState = enCharState_Idle;
		OnProcessCommonStateTransition();
	}
}

void KnightBase::OnProcessFallStateTransition()
{
		m_charState = enCharState_Idle;
		OnProcessCommonStateTransition();
}

void KnightBase::UltEnd() {
	AtkState = false;
	//ボタンプッシュフラグをfalseにする
	pushFlag = false;
	//レベルを下げる
	//UltimateSkill();
	//必殺技使用時のフラグを戻す
	m_UseUltimaitSkillFlag = false;
	//カウンターリセット
	/*m_OnGroundCharCounter = 0;*/

	m_game->SetUltCanUseFlag(true);
	m_game->SetUltCanUseTimer(15.0f);

	//待機ステート
	m_charState = enCharState_Idle;
	OnProcessCommonStateTransition();
}

