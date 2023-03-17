#include "stdafx.h"
#include "KnightAI.h"
#include "Game.h"
#include "KnightPlayer.h"


KnightAI::KnightAI()
{
	SetModel();
	//アニメーションイベント用の関数を設定する。
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});
	//リスポーンする座標0番の取得
	GetRespawnPos();
	respawnNumber = 1;        //リスポーンする座標の番号
	m_respawnPos[respawnNumber].y += m_position_YUp;
	//リスポーンする座標のセット
	//キャラコン
	m_charCon.SetPosition(m_respawnPos[respawnNumber]);
	//剣士
	m_modelRender.SetPosition(m_respawnPos[respawnNumber]);
	m_knightPlayer = FindGO<KnightPlayer>("m_knightplayer");
	//スフィアコライダーを初期化。
	m_sphereCollider.Create(1.0f);
}
KnightAI::~KnightAI()
{

}
void KnightAI::Update()
{
	Rotation();
	SearchPlayer();

	m_modelRender.Update();
}
void KnightAI::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{

}
void KnightAI::AvoidanceSprite()
{

}

void KnightAI::Rotation()
{
	m_rotation.AddRotationDegY(1.0f);
	m_modelRender.SetRotation(m_rotation);

	Vector3 diff = m_knightPlayer->GetPosition() - m_position;
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
void KnightAI::SearchPlayer()
{
	m_isSearchPlayer = false;

	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);

	Vector3 playerPosition = m_knightPlayer->GetPosition();
	Vector3 diff = playerPosition - m_position;

	diff.Normalize();
	float angle = acosf(diff.Dot(m_forward));
	//プレイヤーが視界内に居なかったら。
	if (Math::PI * 0.35f <= fabsf(angle))
	{
		//プレイヤーは見つかっていない。
		return;
	}

	btTransform start, end;
	start.setIdentity();
	end.setIdentity();
	//始点はエネミーの座標。
	start.setOrigin(btVector3(m_position.x, m_position.y + 70.0f, m_position.z));
	//終点はプレイヤーの座標。
	end.setOrigin(btVector3(playerPosition.x, playerPosition.y + 70.0f, playerPosition.z));

	SweepResultWall callback;
	//コライダーを始点から終点まで動かして。
	//衝突するかどうかを調べる。
	PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);
	//壁と衝突した！
	if (callback.isHit == true)
	{
		//プレイヤーは見つかっていない。
		return;
	}

	//壁と衝突してない！！
	//プレイヤー見つけたフラグをtrueに。
	m_isSearchPlayer = true;
}
void KnightAI::Attack()
{

}

void KnightAI::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
	if (m_isSearchPlayer == false)
	{
		m_fontRender.SetText(L"見つからない・・・");
	}
	else
	{
		m_fontRender.SetText(L"見つけた！");
	}
	m_fontRender.Draw(rc);
}

