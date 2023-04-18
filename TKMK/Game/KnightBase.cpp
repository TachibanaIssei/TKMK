#include "stdafx.h"
#include "KnightBase.h"
#include "Status.h"
#include "GameUI.h"

KnightBase::KnightBase()
{
	//ステータスを読み込む
	m_Status.Init("Knight");
	Lv=1;                    //レベル
	AtkSpeed=20;              //攻撃速度

	Cooltime=5;            //スキルのクールタイム
	SkillTimer = Cooltime;

	AvoidanceCoolTime = 2;     ///回避のクールタイム
	AvoidanceTimer = AvoidanceCoolTime;

	Point=0;                 //敵を倒して手に入れたポイント
	GetExp=0;                //中立の敵を倒したときの経験値
	ExpTable=5;              //経験値テーブル
	//respawnNumber = 0;        //リスポーンする座標の番号

	
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
	m_animationClips[enAnimationClip_ChainAtk].Load("Assets/animData/Knight/Knight_ChainAttack2.tka");
	m_animationClips[enAnimationClip_ChainAtk].SetLoopFlag(false);
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
	m_animationClips[enAnimationClip_Fall].Load("Assets/animData/Knight/Knight_fall.tka");
	m_animationClips[enAnimationClip_Fall].SetLoopFlag(false);

	//剣士モデルを読み込み
	m_modelRender.Init("Assets/modelData/character/Knight/Knight_02.tkm", m_animationClips, enAnimationClip_Num, enModelUpAxisZ);

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
}

/// <summary>
/// 中立の敵を倒したときの経験値の処理
/// </summary>
/// <param name="GetExp">中立の敵の経験値</param>
void KnightBase::ExpProcess(int Exp)
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
		LevelUp(LvUPStatus,m_Status,Lv);
		//レベルに合わせてレベルの画像を変更する
		m_gameUI->LevelFontChange(Lv);
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
/// 移動処理
/// </summary>

/// <summary>
/// 回転処理
/// </summary>
void KnightBase::Rotation()
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
	//	//コリジョンオブジェクトを作成する。
	//UltCollision = NewGO<CollisionObject>(0);
	//UltCollision->SetName("Knight_UltimateSkill");
	////コリジョン生成していないなら
	//if (UltCollisionSetFlag == false)
	//{
	//	//コリジョンの座標をプレイヤーと同じに設定
	//	UltCollisionPos = position;
	//	UltCollisionPos.y += 50.0f;

	//	//前フレームの座標を代入
	//	Vector3 oldPosition = oldpostion;
	//	//前フレームの座標から現在のフレームに向かうベクトルを計算する
	//	collisionRot = position - oldPosition;

	//	//Y方向のベクトルを0.0fにする
	//	collisionRot.y = 0.0f;

	//	//正規化
	//	collisionRot.Normalize();

	//	Quaternion rot;
	//	//Y軸回りの回転クォータニオンを作成
	//	rot.SetRotationYFromDirectionXZ(collisionRot);
	//	//ベクトルにクォータニオンを適応
	//	rot.Apply(oldpostion);

	//	//コリジョンオブジェクトを作成する。
	//	collisionObject = NewGO<CollisionObject>(0);
	//	Vector3 collitionPosition = position;
	//	collitionPosition.y += 50.0f;
	//	//collisionPosition.y += 50.0f;
	//	//ボックス状のコリジョンを作成する。
	//	collisionObject->CreateBox(collitionPosition, //座標。
	//		Quaternion(rot), //回転。
	//		Vector3(300.0f, 50.0f, 15.0f) //大きさ。
	//	);
	//	collisionObject->SetIsEnableAutoDelete(false);

	//	collisionObject->SetName("player_UltimateSkill");
	//	collisionObject->SetCreatorName(GetName());

	//	UltCollisionSetFlag = true;
	//}
	//else
	//{
	//	//移動速度設定
	//	UltCollisionPos += collisionRot * 4.0f;
	//	//座標を設定
	//	collisionObject->SetPosition(UltCollisionPos);

	//}
}

void KnightBase::Collition()
{
	//被ダメージ、ダウン中、必殺技、通常攻撃時はダメージ判定をしない。
	if (m_knightState == enKnightState_Damege || 
		m_knightState == enKnightState_Death ||
		m_knightState == enKnightState_UltimateSkill ||
		m_knightState == enKnightState_ChainAtk ||
		m_knightState == enKnightState_Skill ||
		m_knightState == enKnightState_Avoidance)
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
				//エネミーの攻撃力を取ってくる

				//hpを10減らす
				Dameged(Enemy_atk, m_Neutral_enemy);

			}
		}
	
	//被ダメージ、ダウン中、必殺技、通常攻撃時はダメージ判定をしない。
	if (m_knightState == enKnightState_Damege ||
		m_knightState == enKnightState_Death ||
		m_knightState == enKnightState_UltimateSkill ||
		m_knightState == enKnightState_ChainAtk ||
		m_knightState == enKnightState_Skill ||
		m_knightState == enKnightState_Avoidance)
	{
		return;
	}
	//自分の剣の当たり判定に当たっている
	//当たり判定の名前を変えないといけない
	//敵の攻撃用のコリジョンを取得する名前一緒にする
	const auto& Knightcollisions = g_collisionObjectManager->FindCollisionObjects("player_attack");
	//コリジョンの配列をfor文で回す
	for (auto knightcollision : Knightcollisions)
	{
		//このコリジョンを作ったアクターを検索
		m_lastAttackActor = FindGO<Actor>(knightcollision->GetCreatorName());
		//�R���W���������g�̃L�����R���ɓ��������
		if (knightcollision->IsHit(m_charCon)&& m_lastAttackActor!=this)
		{
			//���m�̍U���͕�HP��炷�B
			//�|���ꂽ����̃|�C���g�𑝂₷
			Dameged(m_lastAttackActor->GetAtk(), m_lastAttackActor);
		}
	}

}

/// <summary>
/// ダメージを受けたときの処理
/// </summary>
/// <param name="damege">敵の攻撃力</param>
void KnightBase::Dameged(int damege, Actor* CharGivePoints)
{
	m_Status.Hp -= damege;
	//自身のHPが0以下なら
	if (m_Status.Hp <= 0) {
		//倒されたときの処理に遷移
		//死亡ステート
		m_knightState = enKnightState_Death;
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(17);
		se->Play(false);
		se->SetVolume(0.5f);
		m_Status.Hp = 0;
		//攻撃された相手が中立の敵以外なら
		if (CharGivePoints != nullptr)
		{
			//倒された相手のポイントを増やす
			CharGivePoints->PointProcess(Lv);
		}
	}
	else {
		//ダメージステート
		m_knightState = enKnightState_Damege;
		SoundSource * se = NewGO<SoundSource>(0);
		se->Init(12);
		se->Play(false);
		se->SetVolume(0.5f);
		//無敵時間フラグ
		//invincibleFlag = true;
	}
}

/// <summary>
/// レベル4で必殺技を使用したときの処理
/// </summary>
void KnightBase::UltimateSkill()
{
	//レベルを3下げる
levelDown(LvUPStatus, m_Status, Lv, 3);
	//経験値をリセット
	ExpReset(Lv, GetExp);
	//レベルの経験値テーブルにする
	ExpTableChamge(Lv, ExpTable);

	//レベルに合わせてレベルの画像を変更する
	//m_gameUI->LevelFontChange(Lv);

	m_knightState = enKnightState_UltimateSkill;

}

/// <summary>
/// リスポーンする座標のセット
/// </summary>
void KnightBase::SetRespawn()
{
	//リスポーンする座標0番の取得
	GetRespawnPos();
	//リスポーンする座標のセット
	//キャラコン
	m_charCon.SetPosition(m_respawnPos[respawnNumber]);
	//剣士
	m_modelRender.SetPosition(m_respawnPos[respawnNumber]);
	m_modelRender.SetRotation(m_respawnRotation[respawnNumber]);
}

/// <summary>
/// 自身が倒されたときの処理
/// </summary>
void KnightBase::Death()
{
	////死亡ステート
	//m_knightState = enKnightState_Death;
	//レベルを１下げる
	levelDown(LvUPStatus, m_Status, Lv,1);
	//HPを最大にする
	m_Status.Hp = m_Status.MaxHp;
	//経験値をリセット
	ExpReset(Lv, GetExp);
	//一つ下のレベルの経験値テーブルにする
	ExpTableChamge(Lv,ExpTable);

	//レベルに合わせてレベルの画像を変更する
	//m_gameUI->LevelFontChange(Lv);
}

/// <summary>
/// アニメーション再生時に直線移動させる方向の決定
/// </summary>
/// <param name="moveSpeed">スティックの移動量と乗算させたいスピードの値</param>
/// <param name="stickL">スティックの移動の入力量</param>
void KnightBase::AnimationMove(float moveSpeed,Vector3 stickL)
{
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;

	//移動の入力量がないなら
	if (stickL.x == 0.0f&& stickL.y == 0.0f) {
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
void KnightBase::MoveStraight(Vector3& right, Vector3& forward)
{
	Vector3 SkillSpeed = Vector3::Zero;
	//移動処理
	//移動速度にスティックの入力量を加算する。
	//Vector3 m_SkillSpeed; 
	SkillSpeed = right + forward;
	//キャラクターコントローラーを使って座標を移動させる。
	m_position = m_charCon.Execute(SkillSpeed, 1.0f / 60.0f);
}

/// <summary>
/// アニメーション再生の処理
/// </summary>
void KnightBase::PlayAnimation()
{
	m_modelRender.SetAnimationSpeed(1.0f);

	switch (m_knightState)
	{
	case enKnightState_Idle:
		m_modelRender.PlayAnimation(enAnimationClip_Idle,0.4f);
		break;
	case enKnightState_Walk:
		m_modelRender.PlayAnimation(enAnimationClip_Walk, 0.1f);
		break;
	case enKnightState_Run:
		m_modelRender.PlayAnimation(enAnimationClip_Run,0.2f);
		break;
	case enKnightState_Jump:
		m_modelRender.PlayAnimation(enAnimationClip_Jump, 0.2f);
		break;
	case enKnightState_Fall:
		m_modelRender.PlayAnimation(enAnimationClip_Fall, 0.2f);
		break;
	case enKnightState_ChainAtk:
		m_modelRender.PlayAnimation(enAnimationClip_ChainAtk, 0.3f);
		break;
	case enKnightState_Skill:
		m_modelRender.PlayAnimation(enAnimationClip_Skill, 0.3f);
		break;
	case enKnightState_UltimateSkill:
		//ここ調整必要！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
		m_modelRender.SetAnimationSpeed(1.2f);
		m_modelRender.PlayAnimation(enAnimationClip_UltimateSkill,0.1);
		break;
	case enKnightState_Avoidance:
		m_modelRender.SetAnimationSpeed(1.5f);
		m_modelRender.PlayAnimation(enAnimationClip_Avoidance, 0.1f);
		break;
	case enKnightState_Damege:
		m_modelRender.PlayAnimation(enAnimationClip_Damege, 0.4f);
		break;
	case enKnightState_Death:
		m_modelRender.PlayAnimation(enAnimationClip_Death, 0.4f);
	default:
		break;
	}
}

/// <summary>
/// アニメーションのステートの処理
/// </summary>
void KnightBase::ManageState()
{
	switch (m_knightState)
	{
	case enKnightState_Idle:
		OnProcessIdleStateTransition();
		break;
	case enKnightState_Walk:
		OnProcessIdleStateTransition();
		break;
	case enKnightState_Run:
		OnProcessRunStateTransition();
		break;
	case enKnightState_Jump:
		OnProcessJumpStateTransition();
		break;
	case enKnightState_Fall:
		OnProcessFallStateTransition();
		break;
	case enKnightState_ChainAtk:
		OnProcessChainAtkStateTransition();
		break;
	case enKnightState_Skill:
		OnProcessSkillAtkStateTransition();
		break;
	case enKnightState_UltimateSkill:
		OnProcessUltimateSkillAtkStateTransition();
		break;
	case enKnightState_Avoidance:
		OnProcessAvoidanceStateTransition();
		break;
	case enKnightState_Damege:
		OnProcessDamegeStateTransition();
		break;
	case enKnightState_Death:
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
		if (Lv < 2) {
			m_knightState = enKnightState_Walk;
		}
		else
			//走りステート
		m_knightState = enKnightState_Run;

		return;
	}
	else
	{
		//なかったら待機ステート
		m_knightState = enKnightState_Idle;
		return;
	}
}

/// <summary>
/// Idleアニメーションが再生されているときの処理
/// </summary>
void KnightBase::OnProcessIdleStateTransition()
{
	OnProcessCommonStateTransition();
}

/// <summary>
/// Runアニメーションが再生されているときの処理
/// </summary>
void KnightBase::OnProcessRunStateTransition()
{
	OnProcessCommonStateTransition();
}

void KnightBase::OnProcessJumpStateTransition()
{
	//��ŏ��
	pushFlag = false;
	//�t���O�ŋ󒆂ɂ��邩����
	//�󒆂ɂ���
	if (IsAir(m_charCon) == enIsAir && m_charCon.IsOnGround() == false)
	{
		m_AirFlag = true;
	}

	if (m_AirFlag == true)
	{
		if (m_charCon.IsOnGround() == true)
		{
			//�{�^���v�b�V���t���O��false�ɂ���
			pushFlag = false;
			m_AirFlag = false;
			m_knightState = enKnightState_Idle;
			OnProcessCommonStateTransition();
		}

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
		//待機ステート
		//攻撃を始めたかの判定をfalseにする
		AtkState = false;
		//ボタンプッシュフラグをfalseにする
		pushFlag = false;
		m_knightState = enKnightState_Idle;
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
		SkillState = false;
		//待機ステート
		m_knightState = enKnightState_Idle;
		OnProcessCommonStateTransition();
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
		AtkState = false;
		//ボタンプッシュフラグをfalseにする
		pushFlag = false;
		//待機ステート
		m_knightState = enKnightState_Idle;
		OnProcessCommonStateTransition();
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
		m_knightState = enKnightState_Idle;
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
		//待機ステート
		m_knightState = enKnightState_Idle;
		//無敵時間ステート
		//invincibleFlag = false;
		OnProcessCommonStateTransition();
	}
}

/// <summary>
/// Deathアニメーションが再生されているときの処理
/// </summary>
void KnightBase::OnProcessDeathStateTransition()
{
	//ダメージを受けたときのアニメーション再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//リスポーンする座標に自身の座標をセット
		SetRespawn();
		Death();
		//待機ステート
		m_knightState = enKnightState_Idle;
		OnProcessCommonStateTransition();
	}
}

void KnightBase::OnProcessFallStateTransition()
{
	if (m_charCon.IsOnGround())
	{
		//�ҋ@�X�e�[�g
		m_knightState = enKnightState_Idle;
		OnProcessCommonStateTransition();
	}
}