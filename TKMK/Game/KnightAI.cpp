#include "stdafx.h"
#include "KnightAI.h"
#include "Game.h"
#include "KnightPlayer.h"
#include "Neutral_Enemy.h"
#include "Actor.h"


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

	m_knightAIPoslevel.Init("Assets/level3D/knightAIPos.tkl", [&](LevelObjectData& objData) {

		if (objData.ForwardMatchName(L"Pos") == true) {
			SetPatrolPos(objData.position, objData.number);
			return true;
		}
	});
}
KnightAI::~KnightAI()
{

}
void KnightAI::Move()
{
	Vector3 diff = m_forward;
	diff.Normalize();
	////移動速度を設定する。
	m_moveSpeed = diff * m_Status.Speed;
	m_forward.Normalize();
	Vector3 moveSpeed = m_forward * m_Status.Speed;
	m_position = m_charCon.Execute(moveSpeed, g_gameTime->GetFrameDeltaTime());
	m_modelRender.SetPosition(m_position);

}
void KnightAI::Update()
{
	
	Patrol();
	SearchEnemy();
	ChaseEnemy();
	Attack();
	//ステート
	ManageState();
	//アニメーションの再生
	PlayAnimation();
	Collition();
	Rotation();
	m_position = m_charCon.Execute(m_moveSpeed, 0.1f / 60.0f);

	//剣士のY座標が腰なのでY座標を上げる
	m_position.y = m_position_YUp;
	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();
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
void KnightAI::Collition()
{
	//被ダメージ、ダウン中、必殺技、通常攻撃時はダメージ判定をしない。
	if (m_playerState == enKnightState_Damege ||
		m_playerState == enKnightState_Death ||
		m_playerState == enKnightState_UltimateSkill ||
		m_playerState == enKnightState_ChainAtk ||
		m_playerState == enKnightState_Skill ||
		m_playerState == enKnightState_Avoidance)
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
				Dameged(Enemy_atk);

			}
		}
	}

}
void KnightAI::AvoidanceSprite()
{

}
void KnightAI::LevelMove()
{
	if (Lv >= 1 || Lv <= 3)
	{
		if (m_isSearchEnemy == true)
		{
			ChaseEnemy();
			if (m_SearchPlayer_OR_AI == true)
			{

				m_isSearchEnemy = false;
				ChasePlayer_OR_AI();
			}
		}
	}
}
void KnightAI::ChasePlayer_OR_AI()
{

}
void KnightAI::ChaseEnemy()
{
	if (m_isSearchEnemy == true)
	{
		PL = false;
			//エネミーの座標を取得
			Vector3 AIpos = m_Neutral_Enemy->GetPosition();
			Vector3 diff = AIpos - m_position;
			diff.Normalize();
			m_moveSpeed = diff * m_Status.Speed;
			m_position = m_charCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
			if (m_charCon.IsOnGround()) {
				//地面についた。
				m_moveSpeed.y = 0.0f;
			}
			Vector3 modelPosition = m_position;
			//ちょっとだけモデルの座標を挙げる。
			modelPosition.y += 2.5f;
			m_modelRender.SetPosition(modelPosition);
		
	}
	else
	{

	}
	//Vector3 diff = m_Neutral_Enemy->GetPosition() - m_position;
	//nearPos = Vector3::Zero;
	//for (int i = 0; i <= enemyAmount; i++)
	//{
	//	if (nearPos.Length() > m_enemyPositions.AIpos[i].Length())
	//	{
	//		nearPos = m_enemyPositions.AIpos[i];
	//	}
	//}

	//nearPos.Normalize();
	////移動速度を設定する。
	//m_moveSpeed = nearPos * m_Status.Speed;
	//m_position = m_charCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	//if (m_charCon.IsOnGround()) {
	//	//地面についた。
	//	m_moveSpeed.y = 0.0f;
	//}
	//Vector3 modelPosition = m_position;
	////ちょっとだけモデルの座標を挙げる。
	//modelPosition.y += 2.5f;
	//m_modelRender.SetPosition(modelPosition);
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

//衝突したときに呼ばれる関数オブジェクト(壁用)
struct SweepResultWall :public btCollisionWorld::ConvexResultCallback
{
	bool isHit = false;						//衝突フラグ。

	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		//壁とぶつかってなかったら。
		if (convexResult.m_hitCollisionObject->getUserIndex() != enCollisionAttr_Wall) {
			//衝突したのは壁ではない。
			return 0.0f;
		}

		//壁とぶつかったら。
		//フラグをtrueに。
		isHit = true;
		return 0.0f;
	}
};
void KnightAI::SearchEnemy()
{
	for (auto enemy : m_neutral_Enemys)
	{		
		//エネミーの座標を取得
		Vector3 AIpos = enemy->GetPosition();
		Vector3 diff = AIpos - m_position;
		diff.Normalize();
		float angle = acosf(diff.Dot(m_forward));
		//プレイヤーが視界内に居なかったら。
		if (Math::PI * 0.1f <= fabsf(angle))
		{
			m_isSearchEnemy = true;
			m_Neutral_Enemy = enemy;

			return;
		}
		
	}

	//m_forward = Vector3::AxisZ;
	//m_rotation.Apply(m_forward);
	//int i = 0;
	//enemyAmount = 0;
	//for (auto enemy : m_neutral_Enemys)
	//{
	//	//エネミーの座標を取得
	//	Vector3 AIpos = enemy->GetPosition();
	//	m_enemyPositions.AIpos[i] = enemy->GetPosition();
	//	Vector3 diff = m_enemyPositions.AIpos[i] - m_position;
	//	if (diff.Length() >= 10000.0f)
	//	{
	//		m_enemyPositions.foundFlag[i] = false;
	//		//forを最初に戻る。
	//		continue;
	//	}

	//	diff.Normalize();
	//	float angle = acosf(diff.Dot(m_forward));
	//	//プレイヤーが視界内に居なかったら。
	//	if (Math::PI * 0.35f <= fabsf(angle))
	//	{
	//		//プレイヤーは見つかっていない。
	//		m_enemyPositions.foundFlag[i] = false;
	//		//return;
	//	}
	//	else {
	//		//視界内なら
	//		m_enemyPositions.foundFlag[i] = true;
	//		//PL = false;
	//	}
	//	enemyAmount++;
	//	i++;

		//}
		//btTransform start, end;
		//start.setIdentity();
		//end.setIdentity();
		////始点はエネミーの座標。
		//start.setOrigin(btVector3(m_position.x, m_position.y + 70.0f, m_position.z));
		////終点はプレイヤーの座標。
		//end.setOrigin(btVector3(AIPos.x, AIPos.y + 70.0f, AIPos.z));

		//SweepResultWall callback;
		////コライダーを始点から終点まで動かして。
		////衝突するかどうかを調べる。
		//PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);
		////壁と衝突した！
		//if (callback.isHit == true)
		//{
		//	//プレイヤーは見つかっていない。
		//	return;
		//}

		//壁と衝突してない！！
		//プレイヤー見つけたフラグをtrueに。
		
	
}

void KnightAI::Patrol()
{
	if (PL == true)
	{
		if (P < 0 || P > 4)
		{
			Vector3 newForward = m_patrolPos[0] - m_position;
			Vector3 distance = newForward;
			newForward.Normalize();
			m_forward = newForward;
			Move();
			if (distance.Length() <= 100.0f)
			{
				P = 0;
			}
		}
		if (P == 0)
		{
			Vector3 newForward = m_patrolPos[0] - m_position;
			Vector3 distance = newForward;
			newForward.Normalize();
			m_forward = newForward;
			Move();
			if (distance.Length() <= 100.0f)
			{

				//1からにしかったら+１しろ
				int ram = rand() % 100 /*+ 1*/;
				if (ram >= 0)
				{
					P = 1;
				}
				if (ram > 25)
				{
					P = 2;
				}
				if (ram > 50)
				{
					P = 3;
				}
				if (ram > 75)
				{
					P = 4;
				}
			}

		}
		if (P == 1)
		{
			//position1に向かうコード
			//もしもposition1に到着したらP=1;
			//patrol=true;
			Vector3 newForward = m_patrolPos[1] - m_position;
			Vector3 distance = newForward;
			newForward.Normalize();
			m_forward = newForward;
			Move();
			if (distance.Length() <= 100.0f)
			{

				//1からにしかったら+１しろ
				int ram = rand() % 100 /*+ 1*/;
				if (ram < 50)
				{
					P = 2;
				}
				if (ram > 50)
				{
					P = 4;
				}
			}

		}
		if (P == 2)
		{
			//position1に向かうコード
			//もしもposition1に到着したらP=1;
			//patrol=true;
			Vector3 newForward = m_patrolPos[2] - m_position;
			Vector3 distance = newForward;
			newForward.Normalize();
			m_forward = newForward;
			Move();
			if (distance.Length() <= 100.0f)
			{

				//1からにしかったら+１しろ
				int ram = rand() % 100 /*+ 1*/;
				if (ram < 50)
				{
					P = 1;
				}
				if (ram > 50)
				{
					P = 3;
				}
			}

		}
		if (P == 3)
		{
			//position1に向かうコード
			//もしもposition1に到着したらP=1;
			//patrol=true;
			Vector3 newForward = m_patrolPos[3] - m_position;
			Vector3 distance = newForward;
			newForward.Normalize();
			m_forward = newForward;
			Move();
			if (distance.Length() <= 100.0f)
			{

				//1からにしかったら+１しろ
				int ram = rand() % 100 /*+ 1*/;
				if (ram < 50)
				{
					P = 2;
				}
				if (ram > 50)
				{
					P = 4;
				}
			}

		}
		if (P == 4)
		{
			//position1に向かうコード
			//もしもposition1に到着したらP=1;
			//patrol=true;
			Vector3 newForward = m_patrolPos[4] - m_position;
			Vector3 distance = newForward;
			newForward.Normalize();
			m_forward = newForward;
			Move();
			if (distance.Length() <= 100.0f)
			{

				//1からにしかったら+１しろ
				int ram = rand() % 100 /*+ 1*/;
				if (ram < 50)
				{
					P = 1;
				}
				if (ram > 50)
				{
					P = 3;
				}
			}

		}
	}
	else
	{
		ChaseEnemy();
	}
}

const bool KnightAI ::CanAttack()const
{


		//エネミーの座標を取得
		Vector3 AIpos = m_Neutral_Enemy->GetPosition();
		Vector3 diff = AIpos - m_position;

		if (diff.LengthSq() <= 50.0f * 50.0f)
		{
			//攻撃できる
			return true;
		}
		//攻撃できない
		return false;
	
		
	////中立の敵からプレイヤーに向かうベクトルを計算する
	//Vector3 diff = nearPos - m_position;
	////距離が近かったら
	//if (diff.LengthSq() <= 50.0f * 50.0f)
	//{
	//	//攻撃できる
	//	return true;
	//}
	////攻撃できない
	//return false;
}
void KnightAI::Attack()
{
	if (CanAttack() == true) {
		//連打で攻撃できなくなる

		//一段目のアタックをしていないなら
		if ( AtkState == false)
		{
		
			m_playerState = enKnightState_ChainAtk;

			//FirstAtkFlag = true;
			//コンボを1増やす
			//ComboState++;
			
			AtkState = true;
		}
		//一段目のアタックのアニメーションがスタートしたなら
		if (m_AtkTmingState == FirstAtk_State)
		{

			//ステートを二段目のアタックのアニメーションスタートステートにする
			m_AtkTmingState = SecondAtk_State;

		}

		if (m_AtkTmingState == SecondAtkStart_State)
		{

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

void KnightAI::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);

}

