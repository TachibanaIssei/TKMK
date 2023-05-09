#include "stdafx.h"
#include "KnightUlt.h"
#include "graphics/effect/EffectEmitter.h"
#include "Game.h"
#include "Actor.h"
#include "GameCamera.h"
#include "KnightAI.h"

//壁に当たったら消すようにするtodo
namespace {
	const Vector3 CollsionSize = Vector3(300.0f, 120.0f, 15.0f);//強化なし
	const Vector3 CollsionSize2 = Vector3(360.0f, 120.0f, 15.0f);//強化1
	const Vector3 CollsionSize3 = Vector3(420.0f, 120.0f, 15.0f);//強化2

	const float SphereSize = 20.0f;
}

KnightUlt::KnightUlt()
{
	//スフィアコライダーを初期化。
	m_sphereCollider.Create(1.0f);
	// ゲームカメラを探す
	m_gameCamera = FindGO<GameCamera>("gamecamera");
}

KnightUlt::~KnightUlt()
{
	Ulteffect->Stop();
	//DeleteGO(explosion);
	DeleteGO(Ulteffect);
	DeleteGO(UltCollision);
	DeleteGO(UltDeleteJudgeCollision);
	// 全てを終わらせる！！！！！！！
	m_game->SetStopFlag(false);
	m_actor->UltEnd();
	m_gameCamera->GameCameraUltEnd();
}

bool KnightUlt::Start()
{

	//最初の座標
	m_firstposition = m_position;
	//移動速度を計算。
	m_moveSpeed = Vector3::AxisZ;
	m_rotation.Apply(m_moveSpeed);
	//生成する座標(剣士の少し前)
	m_position += m_moveSpeed * 20.0f;
	Quaternion effectRot = m_rotation;
	m_rotation.AddRotationDegY(360.0f);

	//もし生成する座標が壁に当たっているか壁の向こうなら生成しない
	if (MakeCheck() == true)
	{
		DeleteGO(this);
	}

	//移動速度を決める
	m_moveSpeed *= 940.0f;

	//当たり判定の生成
	SetCollision();
	//消去する時間の設定
	SetDeletetime();
	//エフェクトの生成
	MakeUlt();

	//壁の当たり判定コリジョンオブジェクトを作成する。
	UltDeleteJudgeCollision = NewGO<CollisionObject>(0);
	UltDeleteJudgeCollision->CreateSphere(m_position, m_rotation, SphereSize);
	//コリジョンの名前
	UltDeleteJudgeCollision->SetName("judgeCollision");
	//懲り所オブジェクトが自動で削除されないようにする。
	UltDeleteJudgeCollision->SetIsEnableAutoDelete(false);

	
	Ulteffect = NewGO<EffectEmitter>(0);
	//斬撃エフェクトの再生。
	if (ColorNumb == 2)
	{	
		Ulteffect->Init(enEffect_Knight_Ult_Delete_Blue);
	}
	else if (ColorNumb == 0)
	{
		Ulteffect->Init(enEffect_Knight_Ult_Red);
	}
	else if (ColorNumb == 1)
	{
		Ulteffect->Init(enEffect_Knight_Ult_Green);
	}
	else
	{
		Ulteffect->Init(enEffect_Knight_Ult_Yellow);
	}
	
	Ulteffect->SetScale({ 4.0f,4.0f,4.0f });
	Ulteffect->SetPosition(m_position);
	Ulteffect->SetRotation(effectRot);
	Ulteffect->Play();
	//Ulteffect->Update();
	//前方向
	m_forward = m_moveSpeed;
	return true;
}

void KnightUlt::Update()
{
	//時間が切れたら
	if (m_timer > DeleteTime) {
		//自身を削除する。
		DeleteGO(this);
	}

	//座標を移動させる。
	m_position += m_moveSpeed * g_gameTime->GetFrameDeltaTime();
	

	//タイマーを加算する。
	m_timer += g_gameTime->GetFrameDeltaTime();

		//コリジョンオブジェクトに座標を設定する
		UltCollision->SetPosition(m_position);
		UltCollision->Update();
		//壁判定用の当たり判定の座標を設定する
		UltDeleteJudgeCollision->SetPosition(m_position);
		UltDeleteJudgeCollision->Update();
		//エフェクトの座標を設定
		Ulteffect->SetPosition(m_position);
		Ulteffect->Update();

	

	////タイマーが消去時間の4/5に達したら
	//if (m_timer > DeleteTime * (3.0f / 5.0f)&& explosionflag==false) {
	//	//爆発エフェクト生成
	//	explosion = NewGO<EffectEmitter>(0);
	//	explosion->Init(enEffect_Knight_Ult_Delete_Blue);
	//	explosion->SetPosition(m_position);
	//	//移動速度を計算。
	//	m_moveSpeed = Vector3::AxisZ;
	//	m_rotation.Apply(m_moveSpeed);
	//	Quaternion effectRot = m_rotation;
	//	explosion->SetRotation(effectRot);
	//	m_rotation.AddRotationDegY(360.0f);
	//	explosion->SetScale(Vector3::One * 20.0f);
	//	explosion->Play();
	//	explosionflag = true;
	//}

	//if (explosionflag == true)
	//{
	//	explosion->SetPosition(m_position);
	//	explosion->Update();
	//}
	//モデルの座標を設定
	/*model.SetPosition(m_position);
	model.Update();*/


	
	
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

//生成するかチェックする(壁があったら生成しない)
bool KnightUlt::MakeCheck()
{
	m_Checkposition = m_position;
	m_Checkposition += m_moveSpeed * 60.0f;

	btTransform start, end;
	start.setIdentity();
	end.setIdentity();
	//始点はキャラの座標。
	start.setOrigin(btVector3(m_firstposition.x, m_firstposition.y + 70.0f, m_firstposition.z));
	//終点は必殺技の座標。
	end.setOrigin(btVector3(m_Checkposition.x, m_Checkposition.y + 70.0f, m_Checkposition.z));

	//壁の判定を返す
	SweepResultWall callback_Wall;
	//コライダーを始点から終点まで動かして。
	//壁と衝突するかどうかを調べる。
	PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback_Wall);
	//壁と衝突した！
	if (callback_Wall.isHit == true)
	{
		return true;
	}

	return false;
}

/// <summary>
	/// 当たり判定の設定
	/// </summary>
void KnightUlt::SetCollision()
{
	Vector3 UltCollisionSize;
	if (CharLevel < 6)
	{
		UltCollisionSize = CollsionSize;
	}
	//レベルが7以下なら
	//必殺技一段階強化
	else if (CharLevel < 8)
	{
		UltCollisionSize = CollsionSize2;
	}
	//レベルが10以下なら
	//必殺技二段階強化
	else if (CharLevel <= 10)
	{
		UltCollisionSize = CollsionSize3;
	}
	//球状のコリジョンを作成する。
	UltCollision->CreateBox(m_position, m_rotation, UltCollisionSize);
	//コリジョンの名前
	UltCollision->SetName("player_UltimateSkill");

	//懲り所オブジェクトが自動で削除されないようにする。
	UltCollision->SetIsEnableAutoDelete(false);
}

void KnightUlt::MakeUlt()
{
	Vector3 UltScale = Vector3::Zero;
	if (CharLevel < 6)
	{
		UltScale = Vector3::One;
	}
	//レベルが7以下なら
	//必殺技一段階強化
	else if (CharLevel < 8)
	{
		UltScale = Vector3(1.2f, 1.0f, 1.0f);
	}
	//レベルが10以下なら
	//必殺技二段階強化
	else if (CharLevel <= 10)
	{
		UltScale = Vector3(1.4f, 1.0f, 1.0f);
	}

	/*model.InitBackGround("Assets/modelData/character/Knight/Knight_Ult.tkm");
	model.SetPosition(m_position);
	model.SetRotation(m_rotation);
	model.SetScale(UltScale);
	model.Update();*/
}

void KnightUlt::Render(RenderContext& rc)
{
	//model.Draw(rc);
}