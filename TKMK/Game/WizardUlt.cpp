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

	gameCamera = FindGO<GameCamera>("gamecamera");
	gameCamera2 = FindGO<GameCamera>("gamecamera2P");
	
	//当たったキャラがプレイヤーなら
	if (m_targetActor->IsMatchName(m_playerName) == true) {
		gameCamera->SetPlayerShakeFlag(true);
	}

	return true;
}

void WizardUlt::Update()
{
	if (m_timer > 0.7f)
	{
		//プレイヤーでないと実行しない
		if (m_ThisCreatPlayerFlag == true)
		{
			//必殺技を食らったアクターをリストから削除する
			m_CreatMeActor->EraseDamegeUltActor(m_targetActor);

			//カメラがもう一度雷に打たれていないキャラを探すようにする
			gameCamera->ChangeMoveCameraState(GameCamera::enUltRotCameraState);
			gameCamera->ChangeTunderCameraFlag(false);

			//画面分割していたら
			if (gameCamera2 != nullptr) {
				gameCamera2->ChangeMoveCameraState(GameCamera::enUltRotCameraState);
				gameCamera2->ChangeTunderCameraFlag(false);
			}
		}

		//自分が必殺技を打った最後の雷なら
		if (UltEndFlag == true)
		{
			//攻撃対象のアクターのリストをクリアする
			m_CreatMeActor->DamegeUltActorClear();
			gameCamera->GameCameraUltEnd();

			if (gameCamera2 != nullptr) {
				gameCamera2->GameCameraUltEnd();
			}
		}

		//自身を消す
		DeleteGO(this);
	}

	if (m_timer > 0.6f)
	{
		m_cameraShakeFlag = false;
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

	if (m_targetActor->GetLevel() - m_CreatMeActor->GetLevel() <= 3)
	{
		m_targetActor->LevelDownProcess(1);
	}

	
	else if (m_targetActor->GetLevel() - m_CreatMeActor->GetLevel() <= 5)
	{
		m_targetActor->LevelDownProcess(2);
	}

	//int downlevel = 0;
	////レベル7以上なら
	//if (m_CharLevel7AndAdoveFlag == true) {

	//	
	//	downlevel = m_CreatMeActor->GetLevel();

	//	downlevel -= 6;
	//	
	//	m_targetActor->LevelDownProcess(downlevel);

	//}
}
