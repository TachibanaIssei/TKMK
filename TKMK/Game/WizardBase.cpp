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
	/*m_animationClips[enAnimationClip_Run].Load("Assets/animData/Knight/run.tka");
	m_animationClips[enAnimationClip_Run].SetLoopFlag(true);*/

	//魔法使いのモデルを読み込み
	m_modelRender.Init("Assets/modelData/character/Wizard/Wizard.tkm", m_animationClips, enAnimationClip_Num, enModelUpAxisZ);

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
}

void WizardBase::ExpProcess(int Exp)
{

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

void WizardBase::Dameged(int damege)
{

}

void WizardBase::Death()
{

}

void WizardBase::UltimateSkill()
{

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
		m_modelRender.PlayAnimation(enAnimationClip_Run, 0.2f);
		break;
	case enWizardState_Attack:
		m_modelRender.PlayAnimation(enAnimationClip_Atk, 0.3f);
		break;
	case enWizardState_Skill:
		m_modelRender.PlayAnimation(enAnimationClip_Skill, 0.3f);
		break;
	case enWizardState_UltimateSkill:
		//ここ調整必要！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
		//m_modelRender.SetAnimationSpeed(1.2f);
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
		//if (Lv < 2) {
			m_wizardState = enWizardState_Walk;
		//}
		//else
			//走りステート
			//m_wizardState = enWizardState_Run;

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

}

void WizardBase::OnProcessSkillAtkStateTransition()
{

}

void WizardBase::OnProcessUltimateSkillAtkStateTransition()
{

}

void WizardBase::OnProcessAvoidanceStateTransition()
{

}

void WizardBase::OnProcessDamegeStateTransition()
{

}

void WizardBase::OnProcessDeathStateTransition()
{

}


