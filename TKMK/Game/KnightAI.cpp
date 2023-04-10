#include "stdafx.h"
#include "KnightAI.h"
#include "Game.h"
#include "KnightPlayer.h"
#include "Neutral_Enemy.h"
#include "Actor.h"
#include "WizardUlt.h"


KnightAI::KnightAI()
{
	m_Status.Init("Knight");
	SetModel();
	//アニメーションイベント用の関数を設定する。
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});
	//リスポーンする座標0番の取得
	GetRespawnPos();
	respawnNumber = 1;        //リスポーンする座標の番号
	m_respawnPos[respawnNumber].y /*+= m_position_YUp*/;
	//リスポーンする座標のセット
	//キャラコン
	m_charCon.SetPosition(m_respawnPos[respawnNumber]);
	//剣士
	m_modelRender.SetPosition(m_respawnPos[respawnNumber]);
	m_knightPlayer = FindGO<KnightPlayer>("m_knightplayer");
	m_neutral_Enemys = FindGOs<Neutral_Enemy>("Neutral_Enemy");
	//スフィアコライダーを初期化。
	m_sphereCollider.Create(1.0f);
	m_position = m_charCon.Execute(m_moveSpeed, 0.1f / 60.0f);
	
}
KnightAI::~KnightAI()
{

}
void KnightAI::Update()
{
	SearchEnemy();
	Attack();
	//ステート
	ManageState();
	//アニメーションの再生
	PlayAnimation();
	Collition();
	Rotation();
	m_position.y = m_position_YUp;
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetRotation(m_rot);
	m_modelRender.Update();
}
void KnightAI::SearchEnemy()
{
	//特定のアニメーションが再生中なら
	if (IsEnableMove() == false)
	{
		//抜け出す　移動処理を行わない
		return;
	}
	//アクターたちの情報を取得する
	std::vector<Actor*>& actors = m_game->GetActors();
	Actor* m_actor = nullptr;
	Actor* m_Lvactor = nullptr;
	float minactorDistance = INFINITY;
	float maxactorDistance = 0.0f;
	for (auto actor : actors)
	{
		if (actor == this) {
			//for文の一番最初に戻る
			continue;
		}
		//取得したエネミーたちの座標を取得
		Vector3 actorPos = actor->GetPosition();
		Vector3 diff = m_position - actorPos;
		float distance = diff.Length();
		if (distance < minactorDistance)
		{

			minactorDistance = distance;
			m_actor = actor;
		}
	}
	//エネミーたちの情報を取得する
	std::vector<Neutral_Enemy*>& enemys = m_game->GetNeutral_Enemys();
	Neutral_Enemy* m_neutral_enemy = nullptr;
	Neutral_Enemy* m_farneutral_enemy = nullptr;
	//一番小さい
	float minenemyDistance = INFINITY;
	//一番デカい
	float maxenemyDistance = 0.0f;
	for (auto enemy : enemys)
	{
		for (auto actor : actors)
		{
			if (actor == this) {
				//for文の一番最初に戻る
				continue;
			}
			//取得したエネミーたちの座標を取得
			Vector3 enemyPos = enemy->GetPosition();
			//自分の座標引くエネミーの座標
			Vector3 diff = m_position - enemyPos;
			diff.y = 0.0f;
			float mindistance = diff.Length();
			
			if (mindistance < minenemyDistance)
			{
				minenemyDistance = mindistance;
				m_neutral_enemy = enemy;
			}
			Vector3 actorPos = actor->GetPosition();
			Vector3 diff2 = actorPos - enemyPos;
			float maxdistance = diff2.Length();
			if (maxdistance > maxenemyDistance)
			{
				maxenemyDistance = maxdistance;
				m_farneutral_enemy = enemy;
			}

		}
	}
	if (Lv >= 1 && Lv <= 3)
	{
		Vector3 diff = m_neutral_enemy->GetPosition() - m_position;
		diff.Normalize();
		//移動速度を設定する。
		m_moveSpeed = diff * m_Status.Speed;
		m_position = m_charCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
		/*if (m_Status.Hp < m_Status.MaxHp )
		{
			Vector3 diff = m_farneutral_enemy->GetPosition() - m_position;
			diff.Normalize();
			m_moveSpeed = diff * m_Status.Speed;
			m_position = m_charCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
		}*/
	}
	else 
	{
		Vector3 diff = m_actor->GetPosition() - m_position;
		diff.y = 0.0f;
		diff.Normalize();
		//移動速度を設定する。
		m_moveSpeed = diff * m_Status.Speed;
		m_position = m_charCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
		
	}


}
const bool KnightAI::CanAttackenemy()
{
	//エネミーたちの情報を取得する
	std::vector<Neutral_Enemy*>& enemys = m_game->GetNeutral_Enemys();

	for (auto Enemys : enemys)
	{
		//取得したエネミーたちの座標を取得
		Vector3 enemyPos = Enemys->GetPosition();
		//取得した座標を自分のと引く
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
	//アクターたちの情報を取得する
	std::vector<Actor*>& actors = m_game->GetActors();
	for (auto Actors : actors)
	{
		if (Actors == this) {
			//for文の一番最初に戻る
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
	
	if (CanAttackenemy() || CanAttackActor()) {
		//狙う方を変える
		Vector3 targetPos = TargetChange();
		//連打で攻撃できなくなる

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
	if (pushFlag == false && SkillEndFlag == false && SkillState == false )
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
	if (pushFlag == false && Lv >= 4&& UltFlug==true)
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
void KnightAI::Collition()
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
	else
	{
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
				//Dameged(Enemy_atk);

			}
		}
	}

}

void KnightAI::AvoidanceSprite()
{

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
		abort();	//呼ぶな
	}

	if (m_targetEnemy != nullptr && m_targetActor == nullptr) {
		//エネミーの座標を渡す
		return m_targetEnemy->GetPosition();
	}
	if (m_targetActor != nullptr && m_targetEnemy == nullptr) {
		//アクターの座標を渡す
		UltFlug = true;
		return m_targetActor->GetPosition();
	}
	//中立の敵とアクタークラスの敵両方いる場合は、
	//かつアクタークラスの敵が自分よりレベル低い時中立の敵を倒しに行く
	if (m_targetActor->GetLevel() > Lv) {
		return m_targetEnemy->GetPosition();
	}

	return m_targetActor->GetPosition();

}
void KnightAI::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);

}

