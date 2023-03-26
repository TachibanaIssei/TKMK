#include "stdafx.h"
#include "WizardBase.h"
#include "Status.h"
#include "GameUI.h"


WizardBase::WizardBase()
{
	//ステータスを読み込む
	m_Status.Init("Wizard");
	Lv = 1;                    //レベル
	AtkSpeed = 10;              //攻撃速度

	Cooltime = 15;            //スキルのクールタイム
	SkillTimer = Cooltime;

	AvoidanceCoolTime = 2;     ///回避のクールタイム
	AvoidanceTimer = AvoidanceCoolTime;

	Point = 0;                 //敵を倒して手に入れたポイント
	GetExp = 0;                //中立の敵を倒したときの経験値
	ExpTable = 5;              //経験値テーブル
}

WizardBase::~WizardBase()
{

}

/// <summary>
/// 魔法使いのモデルやアニメーション、キャラコンの初期化
/// </summary>
void WizardBase::SetModel()
{
	//プレイヤー
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/Wizard/Wizard_Idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Walk].Load("Assets/animData/Wizard/Wizard_Walk.tka");
	m_animationClips[enAnimationClip_Walk].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Run].Load("Assets/animData/Wizard/Wizard_Run.tka");
	m_animationClips[enAnimationClip_Run].SetLoopFlag(true);
	m_animationClips[enWizardState_Attack].Load("Assets/animData/Wizard/Wizard_Attack.tka");
	m_animationClips[enWizardState_Attack].SetLoopFlag(false);
	m_animationClips[enWizardState_Avoidance].Load("Assets/animData/Wizard/Wizard_Avoidance.tka");
	m_animationClips[enWizardState_Avoidance].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Damege].Load("Assets/animData/Wizard/Wizard_Damege.tka");
	m_animationClips[enAnimationClip_Damege].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Death].Load("Assets/animData/Wizard/Wizard_Death.tka");
	m_animationClips[enAnimationClip_Death].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Skill].Load("Assets/animData/Wizard/Wizard_Warp.tka");
	m_animationClips[enAnimationClip_Skill].SetLoopFlag(false);


	//魔法使いのモデルを読み込み
	m_modelRender.Init("Assets/modelData/character/Wizard/Wizard.tkm", m_animationClips, enAnimationClip_Num, enModelUpAxisZ);

	m_position = { 0.0f,0.0f,0.0f };
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetScale(Vector3(0.7f, 0.7f, 0.7f));

	m_rot.SetRotationY(0.0f);
	m_modelRender.SetRotation(m_rot);

	m_charCon.Init(
		10.0f,
		39.0f,
		m_position
	);
}

/// <summary>
/// 中立の敵を倒したときの経験値の処理
/// </summary>
/// <param name="GetExp">中立の敵の経験値</param>
void WizardBase::ExpProcess(int Exp)
{
	//もしレベルが10(Max)なら
	if (Lv == 10)return;
	//自身の経験値に敵を倒したときに手に入れる経験値を足す
	GetExp += Exp;
	//手に入れた経験値より経験値テーブルのほうが大きかったら
	if (GetExp < ExpTable) return;      //抜け出す
	else {
		//経験値テーブルより手に入れた経験値のほうが大きかったら
		//レベルアップ
		LevelUp(LvUpStatus, m_Status, Lv);
		//レベルに合わせてレベルの画像を変更する
		gameUI->LevelFontChange(Lv);
		switch (Lv)
		{
		case 2:
			ExpTable = 10;
			break;
		case 3:
			ExpTable = 20;
			break;
		case 4:
			ExpTable = 30;
			break;
		case 5:
			ExpTable = 40;
			break;
		case 6:
			ExpTable = 50;
			break;
		case 7:
			ExpTable = 60;
			break;
		case 8:
			ExpTable = 70;
			break;
		case 9:
			ExpTable = 80;
			break;
		default:
			break;
		}
	}
}

/// <summary>
/// 回転処理
/// </summary>
void WizardBase::Rotation()
{
	//xかzの移動速度があったら(スティックの入力があったら)。
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		//キャラクターの方向を変える。
		m_rot.SetRotationYFromDirectionXZ(m_moveSpeed);
		//絵描きさんに回転を教える。
		m_modelRender.SetRotation(m_rot);
	}
}

/// <summary>
/// ダメージを受けたときの処理
/// </summary>
/// <param name="damege">敵の攻撃力</param>
void WizardBase::Dameged(int damege)
{
	m_Status.Hp -= damege;
	//自身のHPが0以下なら
	if (m_Status.Hp <= 0) {
		//倒されたときの処理に遷移
		//死亡ステート
		m_wizardState = enWizardState_Death;
		m_Status.Hp = 0;
		//Death();
		//SetRespawn();


	}
	else {
		//ダメージステート
		m_wizardState = enWizardState_Damege;
		//無敵時間フラグ
		//invincibleFlag = true;
	}
}

/// <summary>
/// 
/// </summary>
void WizardBase::Death()
{
	////死亡ステート
	//m_playerState = enKnightState_Death;
	//レベルを１下げる
	levelDown(LvUpStatus, m_Status, Lv, 1);
	//HPを最大にする
	m_Status.Hp = m_Status.MaxHp;
	//経験値をリセット
	ExpReset(Lv, GetExp);
	//一つ下のレベルの経験値テーブルにする
	ExpTableChamge(Lv, ExpTable);


	//レベルに合わせてレベルの画像を変更する
	gameUI->LevelFontChange(Lv);
}

/// <summary>
/// スキルの処理
/// </summary>
/// <param name="position"></param>
/// <param name="rotation"></param>
/// <param name="charCon"></param>
void WizardBase::Skill(Vector3& position,Quaternion& rotation, CharacterController& charCon)
{
	m_moveSpeed = Vector3::AxisZ;
	//回転も
	rotation.Apply(m_moveSpeed);
	position += m_moveSpeed * 500.0f;
	m_moveSpeed *= 1000.0f;
	rotation.AddRotationDegY(360.0f);


	Vector3 WarpPos;
	//ワープした座標がオブジェクトとかぶっているなら
	m_WarpCollisionSolver.Execute
	(
		WarpPos,
		position,
		oldPosition
	);

	//キャラクターコントローラーを使って座標を移動させる。
	charCon.SetPosition(position);
}

/// <summary>
/// 必殺技の処理
/// </summary>
void WizardBase::UltimateSkill()
{

}

/// <summary>
/// アニメーション再生時に直線移動させる方向の決定
/// </summary>
/// <param name="moveSpeed">スティックの移動量と乗算させたいスピードの値</param>
/// <param name="stickL">スティックの移動の入力量</param>
void WizardBase::AnimationMove(float moveSpeed, Vector3 stickL)
{
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;

	//移動の入力量がないなら
	if (stickL.x == 0.0f && stickL.y == 0.0f) {
		//前に移動
		stickL.x = 0.0f;
		stickL.y = 1.0f;
	}

	m_Skill_Forward = Vector3::Zero;
	m_Skill_Right = Vector3::Zero;

	//カメラの前方向と右方向のベクトルを持ってくる。
	m_Skill_Forward = g_camera3D->GetForward();
	m_Skill_Right = g_camera3D->GetRight();
	//y方向には移動させない。
	m_Skill_Forward.y = 0.0f;
	m_Skill_Right.y = 0.0f;

	//左スティックの入力量とstatusのスピードを乗算。
	m_Skill_Right *= stickL.x * moveSpeed;
	m_Skill_Forward *= stickL.y * moveSpeed;
}

//直線移動させる
void WizardBase::MoveStraight(Vector3& right, Vector3& forward)
{
	//移動処理
	//移動速度にスティックの入力量を加算する。
	//Vector3 m_SkillSpeed; 
	m_moveSpeed = right + forward;
	//キャラクターコントローラーを使って座標を移動させる。
	m_position = m_charCon.Execute(m_moveSpeed, 1.0f / 60.0f);
}

/// <summary>
/// リスポーンする座標のセット
/// </summary>
void WizardBase::SetRespawn()
{
	//リスポーンする座標2番の取得
	GetRespawnPos();
	//リスポーンする座標のセット
	//キャラコン
	m_charCon.SetPosition(m_respawnPos[respawnNumber]);
	//魔法使い
	m_modelRender.SetPosition(m_respawnPos[respawnNumber]);
}

/// <summary>
/// アニメーション再生の処理
/// </summary>
void WizardBase::PlayAnimation()
{
	m_modelRender.SetAnimationSpeed(1.0f);

	switch (m_wizardState)
	{
	case enWizardState_Idle:
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.4f);
		break;
	case enWizardState_Walk:
		m_modelRender.PlayAnimation(enAnimationClip_Walk, 0.1f);
		break;
	case enWizardState_Run:
		m_modelRender.SetAnimationSpeed(0.9f);
		m_modelRender.PlayAnimation(enAnimationClip_Run, 0.2f);
		break;
	case enWizardState_Attack:
		m_modelRender.PlayAnimation(enAnimationClip_Atk, 0.2f);
		break;
	case enWizardState_Skill:
		m_modelRender.SetAnimationSpeed(1.2f);
		m_modelRender.PlayAnimation(enAnimationClip_Skill, 0.6f);
		break;
	case enWizardState_UltimateSkill:
		m_modelRender.PlayAnimation(enAnimationClip_UltimateSkill, 0.1);
		break;
	case enWizardState_Avoidance:
		m_modelRender.SetAnimationSpeed(1.5f);
		m_modelRender.PlayAnimation(enAnimationClip_Avoidance, 0.1f);
		break;
	case enWizardState_Damege:
		m_modelRender.PlayAnimation(enAnimationClip_Damege, 0.4f);
		break;
	case enWizardState_Death:
		m_modelRender.SetAnimationSpeed(1.2f);
		m_modelRender.PlayAnimation(enAnimationClip_Death, 0.4f);
	default:
		break;
	}
}

/// <summary>
/// アニメーションのステートの処理
/// </summary>
void WizardBase::ManageState()
{
	switch (m_wizardState)
	{
	case enWizardState_Idle:
		OnProcessIdleStateTransition();
		break;
	case enWizardState_Walk:
		OnProcessIdleStateTransition();
		break;
	case enWizardState_Run:
		OnProcessRunStateTransition();
		break;
	case enWizardState_Attack:
		OnProcessAttackStateTransition();
		break;
	case enWizardState_Skill:
		OnProcessSkillAtkStateTransition();
		break;
	case enWizardState_UltimateSkill:
		OnProcessUltimateSkillAtkStateTransition();
		break;
	case enWizardState_Avoidance:
		OnProcessAvoidanceStateTransition();
		break;
	case enWizardState_Damege:
		OnProcessDamegeStateTransition();
		break;
	case enWizardState_Death:
		OnProcessDeathStateTransition();
		break;

	}
}

/// <summary>
/// 歩きアニメーションが再生されているなら。
/// </summary>
void WizardBase::OnProcessCommonStateTransition()
{
	//スティックの入力量があったら
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		if (Lv < 4) {
			m_wizardState = enWizardState_Walk;
		}
		else
			//走りステート
			m_wizardState = enWizardState_Run;

		return;
	}
	else
	{
		//なかったら待機ステート
		m_wizardState = enWizardState_Idle;
		return;
	}
}

void WizardBase::OnProcessIdleStateTransition()
{
	OnProcessCommonStateTransition();
}

void WizardBase::OnProcessRunStateTransition()
{
	OnProcessCommonStateTransition();
}

void WizardBase::OnProcessAttackStateTransition()
{
	//アタックのアニメーション再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//待機ステート
		//攻撃を始めたかの判定をfalseにする
		//AtkState = false;
		//ボタンプッシュフラグをfalseにする
		pushFlag = false;
		m_wizardState = enWizardState_Idle;
		OnProcessCommonStateTransition();
	}
}

void WizardBase::OnProcessSkillAtkStateTransition()
{
	//スキルのアニメーションが終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//待機ステート
		//ボタンプッシュフラグをfalseにする
		pushFlag = false;
		//スキルエンドフラグをtrueにする
		SkillEndFlag = true;
		m_wizardState = enWizardState_Idle;
		OnProcessCommonStateTransition();
	}
}

void WizardBase::OnProcessUltimateSkillAtkStateTransition()
{

}

void WizardBase::OnProcessAvoidanceStateTransition()
{
	//回避のアニメーション再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		AvoidanceFlag = false;
		AvoidanceEndFlag = true;
		//ボタンプッシュフラグをfalseにする
		pushFlag = false;
		//待機ステート
		m_wizardState = enWizardState_Idle;
		OnProcessCommonStateTransition();
	}
}

void WizardBase::OnProcessDamegeStateTransition()
{
	//ダメージを受けたときのアニメーション再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//待機ステート
		m_wizardState = enWizardState_Idle;
		//無敵時間ステート
		//invincibleFlag = false;
		OnProcessCommonStateTransition();
	}
}

void WizardBase::OnProcessDeathStateTransition()
{
	//ダメージを受けたときのアニメーション再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//リスポーンする座標に自身の座標をセット
		SetRespawn();
		Death();
		//待機ステート
		m_wizardState = enWizardState_Idle;
		OnProcessCommonStateTransition();
	}
}


