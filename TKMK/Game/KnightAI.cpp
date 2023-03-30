#include "stdafx.h"
#include "KnightAI.h"
#include "Game.h"
#include "Neutral_Enemy.h"
#include "Actor.h"
#include "KnightBase.h"

KnightAI::KnightAI()
{
	//剣士のステータス
	m_Status.Init("Knight");
	SetModel();
	//アニメーションイベント用の関数を設定する。
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});
	//リスポーンする座標0番の取得
	GetRespawnPos();
	respawnNumber = 1;        //リスポーンする座標の番号
	m_respawnPos[respawnNumber].y;
	//リスポーンする座標のセット
	//キャラコン
	m_charCon.SetPosition(m_respawnPos[respawnNumber]);
	//剣士
	m_modelRender.SetPosition(m_respawnPos[respawnNumber]);
}
KnightAI::~KnightAI()
{

}
void KnightAI::Update()
{


	Attack();
	//ステート
	ManageState();
	//アニメーションの再生
	PlayAnimation();
	Collition();
	Rotation();
	m_position = m_charCon.Execute(m_moveSpeed, 0.1f / 60.0f);

	//m_rot.AddRotationDegY(2.0f);
	//Vector3 moveSpeed = Vector3::AxisZ;
	//m_rot.Apply(moveSpeed);
	//m_position += moveSpeed * 10.0f;
	//m_modelRender.SetRotation(m_rot);

	//剣士のY座標が腰なのでY座標を上げる
	m_position.y = m_position_YUp;
	m_modelRender.SetPosition(m_position);
	//m_charCon.SetPosition(m_position);
	m_modelRender.SetRotation(m_rot);
	m_modelRender.Update();
}
const bool KnightAI::CanAttackenemy()
{
	//エネミーたちの情報をゲームから取得する
	std::vector<Neutral_Enemy*>& enemys = m_game->GetNeutral_Enemys();

	for (auto Enemys : enemys)
	{
		//エネミーたちの座標を取得
		Vector3 enemyPos = Enemys->GetPosition();
		//エネミーの座標をと自分の引く
		Vector3 diff = enemyPos - m_position;
		
		if (diff.LengthSq() <= 70.0f * 70.0f)
		{
			m_targetEnemy = Enemys;

			return true;
		}
	}
	m_targetEnemy = nullptr;

	return false;
}
const bool KnightAI::CanAttackActor()
{
	std::vector<Actor*>& actors = m_game->GetActors();
	for (auto Actors : actors)
	{
		if (Actors == this) {
			//これは自分
			continue;
		}

		Vector3 actorPos = Actors->GetPosition();
		Vector3 diff = actorPos - m_position;

		if (diff.LengthSq() <= 70.0f * 70.0f)
		{
			m_targetActor = Actors;
			return true;
		}
	}
	m_targetActor = nullptr;
	return false;
}
void KnightAI::Attack()
{
	if (CanAttackenemy()|| CanAttackActor()) {
		//連打で攻撃できなくなる
		Vector3 targetPos = TargetChange();

		//一段目のアタックをしていないなら
		if (AtkState == false)
		{
			Vector3 diff = targetPos - m_position;
			m_rot.SetRotationYFromDirectionXZ(diff);
			m_knightState = enKnightState_ChainAtk;
			//FirstAtkFlag = true;
			//コンボを1増やす
			//ComboState++;
			AtkState = true;
		}
		//一段目のアタックのアニメーションがスタートしたなら
		if (m_AtkTmingState == FirstAtk_State)
		{
			Vector3 diff = targetPos - m_position;
			m_rot.SetRotationYFromDirectionXZ(diff);
			//ステートを二段目のアタックのアニメーションスタートステートにする
			m_AtkTmingState = SecondAtk_State;

		}

		if (m_AtkTmingState == SecondAtkStart_State)
		{
			Vector3 diff = targetPos - m_position;
			m_rot.SetRotationYFromDirectionXZ(diff);
			//ステートを三段目のアタックのアニメーションスタートステートにする
			m_AtkTmingState = LastAtk_State;

		}

	}
	//スキルを発動する処理
	//Bボタンが押されたら
	if (pushFlag == false && SkillEndFlag == false && SkillState == false && g_pad[0]->IsTrigger(enButtonB))
	{

		//移動速度を上げる
		m_Status.Speed += 120.0f;

		/*AnimationMove(SkillSpeed);*/
		pushFlag = true;
		SkillState = true;
		//AtkCollistionFlag = true;
	}

	//必殺技を発動する処理
	//Xボタンが押されたら
	if (pushFlag == false && Lv >= 4 && g_pad[0]->IsTrigger(enButtonX))
	{
		pushFlag = true;
		//アニメーション再生、レベルを３
		UltimateSkill();



		//アルティメットSE
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(16);
		se->Play(false);
		se->SetVolume(0.3f);

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
	if (AtkCollistionFlag == true) AtkCollisiton();

}
/// <summary>
/// 攻撃時の当たり判定の処理
/// </summary>
void KnightAI::AtkCollisiton()
{
	//コリジョンオブジェクトを作成する。
	auto collisionObject = NewGO<CollisionObject>(0);
	Vector3 collisionPosition = m_position;
	//座標をプレイヤーの少し前に設定する。
	//collisionPosition += forward * 50.0f;
	//ボックス状のコリジョンを作成する。
	collisionObject->CreateBox(collisionPosition, //座標。
		Quaternion::Identity, //回転。
		Vector3(70.0f, 15.0f, 15.0f) //大きさ。
	);
	collisionObject->SetName("player_attack");
	collisionObject->SetCreatorName(GetName());

	//「Sword」ボーンのワールド行列を取得する。
	Matrix matrix = m_modelRender.GetBone(m_swordBoneId)->GetWorldMatrix();

	//matrix.MakeRotationZ(90.0f);
	//「Sword」ボーンのワールド行列をコリジョンに適用する。
	collisionObject->SetWorldMatrix(matrix);
}
void KnightAI::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//一段目のアタックのアニメーションが始まったら
	if (wcscmp(eventName, L"FirstAttack_Start") == 0)
	{
		m_AtkTmingState = FirstAtk_State;
		//剣のコリジョンを生成
		AtkCollistionFlag = true;
		//剣１段目音
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(13);
		se->Play(false);
		se->SetVolume(0.3f);
	}
	//二段目のアタックのアニメーションが始まったら
	if (wcscmp(eventName, L"SecondAttack_Start") == 0)
	{
		m_AtkTmingState = SecondAtkStart_State;
		//剣のコリジョンを生成
		AtkCollistionFlag = true;
		//剣２段目音
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(14);
		se->Play(false);
		se->SetVolume(0.3f);
	}
	//三段目のアタックのアニメーションが始まったら
	if (wcscmp(eventName, L"LastAttack_Start") == 0)
	{
		m_AtkTmingState = LastAtk_State;
		//剣のコリジョンを生成
		AtkCollistionFlag = true;
		//剣３段目音
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(15);
		se->Play(false);
		se->SetVolume(0.3f);
	}
	//スキルのアニメーションが始まったら
	if (wcscmp(eventName, L"SkillAttack_Start") == 0)
	{
		m_Status.Atk += 20;
		//m_AtkTmingState = LastAtk_State;
		//剣のコリジョンを生成
		AtkCollistionFlag = true;

		//スキル音を発生
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(11);
		se->Play(false);
		se->SetVolume(0.3f);
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
			m_knightState = enKnightState_Idle;
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
			m_knightState = enKnightState_Idle;
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
		m_knightState = enKnightState_Idle;
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

void KnightAI::Rotation()
{
	if (fabsf(m_moveSpeed.x) < 0.001f
		&& fabsf(m_moveSpeed.z) < 0.001f) {
		//m_moveSpeed.xとm_moveSpeed.zの絶対値がともに0.001以下ということは
		//このフレームではキャラは移動していないので旋回する必要はない。
		return;
	}
	//atan2はtanθの値を角度(ラジアン単位)に変換してくれる関数。
	//m_moveSpeed.x / m_moveSpeed.zの結果はtanθになる。
	//atan2を使用して、角度を求めている。
	//これが回転角度になる。
	float angle = atan2(-m_moveSpeed.x, m_moveSpeed.z);
	//atanが返してくる角度はラジアン単位なので
	//SetRotationDegではなくSetRotationを使用する。
	m_rot.SetRotationY(-angle);

	//回転を設定する。
	m_modelRender.SetRotation(m_rot);

	//プレイヤーの前ベクトルを計算する。
	m_forward = Vector3::AxisZ;
	m_rot.Apply(m_forward);
}

const Vector3 KnightAI::TargetChange() 
{
	if (m_targetEnemy == nullptr && m_targetActor == nullptr) {
		abort();	// 呼べません！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
	}

	if (m_targetEnemy != nullptr && m_targetActor == nullptr) {
		// そもそもターゲットは中立のみ
		return m_targetEnemy->GetPosition();
	}
	if (m_targetActor != nullptr && m_targetEnemy == nullptr) {
		// そもそもターゲットはアクターのみ
		return m_targetActor->GetPosition();
	}

	// ここからターゲットを決める計算を行う

	// もしアクターのレベルが自分より高いなら、中立を狙う
	if (m_targetActor->GetLevel() > Lv) {
		return m_targetEnemy->GetPosition();
	}

	return m_targetActor->GetPosition();

}

void KnightAI::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}











