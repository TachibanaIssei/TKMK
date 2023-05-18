#include "stdafx.h"
#include "WizardUlt.h"
#include "Player.h"
#include "KnightAI.h"
#include "Actor.h"
#include "Game.h"
#include "GameCamera.h"

WizardUlt::WizardUlt()
{

}

WizardUlt::~WizardUlt()
{
	DeleteGO(UltCollision);
	// 全てを終わらせる！！！！！！！
	//地上にいることを示すカウンターを減らす
	m_CreatMeActor->SubOnGroundCharCounter();
	

	//ターゲットの地上にいるフラグをfalseにする	
	m_targetActor->ChangeGroundChackflag(false);

	//プレイヤーでないと実行しない
	if (m_ThisCreatPlayerFlag == true)
	{
		gameCamera = FindGO<GameCamera>("gamecamera");
		//カメラがもう一度雷に打たれていないキャラを探すようにする
		gameCamera->ChangeMoveCameraState(GameCamera::m_enUltRotCameraState);
		gameCamera->ChangeTunderCameraFlag(false);

	}
	
	//自分が必殺技を打った最後の雷なら
	if (UltEndFlag == true)
	{
		//攻撃対象のアクターのリストをクリアする
		m_CreatMeActor->DamegeUltActorClear();
		gameCamera->GameCameraUltEnd();
	}
}

bool WizardUlt::Start()
{
	CreatCollision();
	//必殺技を打ったキャラの名前
	m_CreatMeActor = FindGO<Actor>(UltCollision->GetCreatorName());

	//攻撃対象のキャラの名前
	m_targetActor = FindGO<Actor>(SetTargetActorName());

	Thunder = NewGO<EffectEmitter>(0);
	Thunder->Init(enEffect_Knight_Thunder);

	Vector3 ThunderPos = m_position;
	ThunderPos.y = 0.0f;
	Thunder->SetPosition(ThunderPos);
	Thunder->SetScale(Vector3::One * 12.0f);
	Thunder->Play();

	return true;
}

void WizardUlt::Update()
{
	if (m_timer > 0.7f)
	{
		//自身を消す
		DeleteGO(this);
	}

	if (Thunder->IsPlay() == false)
	{
		
	}
	
	//一秒ごとに雷を落とす
	if (m_timer > 0.3f&& FallTunderFlag==false)
	{
		//攻撃対象のキャラにダメージを与える処理
		Damege();
		FallTunderFlag = true;
	}
	
	m_timer += g_gameTime->GetFrameDeltaTime();
	
}

void WizardUlt::Move()
{
	m_position = m_targetActor->GetPosition();
	m_position.y += 100.0f;

	UltCollision->SetPosition(m_position);
}

//雷を落とす
void WizardUlt::FallThunder()
{
	//for (auto actor : m_game->GetActors())
	//{
	//	//生成するキャラと必殺技を打ったキャラの名前が同じなら処理を飛ばす
	//	if (m_CreatMeActor->GetName() == actor->GetName())
	//	{
	//		continue;
	//	}

	//	//地上にいるAIにだけ雷を落とす(エフェクト生成)
	//	if (actor->IsGroundIn() == true)
	//	{
	//		Thunder = NewGO<EffectEmitter>(0);
	//		Thunder->Init(enEffect_Knight_Thunder);

	//		Vector3 ThunderPos = actor->GetPosition();
	//		ThunderPos.y = 0.0f;
	//		Thunder->SetPosition(ThunderPos);
	//		Thunder->SetScale(Vector3::One * 12.0f);
	//		Thunder->Play();
	//	}

	//	//タイマーリセット
	//	m_timer = 0.0f;
	//}
}

void WizardUlt::Damege()
{
	//攻撃対象のキャラにダメージを与える
	m_targetActor->Dameged(m_UltDamege, m_CreatMeActor);
	int downlevel = 1;

}
