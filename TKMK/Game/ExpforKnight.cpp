#include "stdafx.h"
#include "ExpforKnight.h"
#include "Game.h"
#include "GameUI.h"
#include "Actor.h"

namespace {
	//1人用
	const Vector2 EXPERIENCE_BAR_SOLO_POS = Vector2(900.0f, -620.0f);	//経験値バーの座標
	//2人用
	const Vector2 EXPERIENCE_BAR_DUO_LEFT_POS = Vector2(300.0f, -620.0f);	//左
	const Vector2 EXPERIENCE_BAR_DUO_RIGHT_POS = Vector2(-300.0f, -620.0f);	//右
	//3～4人用
	const Vector2 EXPERIENCE_BAR_LEFT_UP_POS = Vector2(300.0f, -270.0f);	//左上
	const Vector2 EXPERIENCE_BAR_LEFT_DOWN_POS = Vector2(300.0f, -270.0f);	//左下
	const Vector2 EXPERIENCE_BAR_RIGHT_UP_POS = Vector2(300.0f, -270.0f);	//右上
	const Vector2 EXPERIENCE_BAR_RIGHT_DOWN_POS = Vector2(300.0f, -270.0f);	//右下

	const float EXPEIENCE_SPRITE_W_H = 128.0f;	//経験値オーブ画像の解像度
}
ExpforKnight::ExpforKnight()
{

}

ExpforKnight::~ExpforKnight()
{
}

bool ExpforKnight::Start()
{
	m_game = FindGO<Game>("game");

	//プレイヤーが経験値を獲得する画像
	m_NormalExp.Init("Assets/sprite/Exp.DDS", EXPEIENCE_SPRITE_W_H, EXPEIENCE_SPRITE_W_H, AlphaBlendMode_Add);

	m_experienceBarPosition = SetExperienceBarPosition();
	SetPlayerNumber();

	//ワールド座標をスクリン座標に変換
	if (g_renderingEngine->GetGameMode() == RenderingEngine::enGameMode_SoloPlay)
	{
		g_camera3D[0]->CalcScreenPositionFromWorldPosition(m_effectScreenPosition, m_enemyPos);
	}
	else
	{
		if (m_playerActor->IsMatchName("knightplayer"))
		{
			g_camera3D[0]->CalcScreenPositionFromWorldPosition(m_effectScreenPosition, m_enemyPos);
		}
		else if (m_playerActor->IsMatchName("knightplayer2"))
		{
			g_camera3D[1]->CalcScreenPositionFromWorldPosition(m_effectScreenPosition, m_enemyPos);
		}
		else if (m_playerActor->IsMatchName("knightplayer3"))
		{
			g_camera3D[2]->CalcScreenPositionFromWorldPosition(m_effectScreenPosition, m_enemyPos);
		}
		else if (m_playerActor->IsMatchName("knightplayer4"))
		{
			g_camera3D[3]->CalcScreenPositionFromWorldPosition(m_effectScreenPosition, m_enemyPos);
		}
		else if (m_playerActor->IsMatchName("knightplayer4"))
		{
			g_camera3D[3]->CalcScreenPositionFromWorldPosition(m_effectScreenPosition, m_enemyPos);
		}
		else if (m_playerActor->IsMatchName("KnightAI1"))
		{
			g_camera3D[3]->CalcScreenPositionFromWorldPosition(m_effectScreenPosition, m_enemyPos);
		}
	}
	m_center.x = (m_effectScreenPosition.x - m_experienceBarPosition.x) / 2.0f;
	m_center.y = (m_effectScreenPosition.y - m_experienceBarPosition.y) / 2.0f;
	m_center.x -= 200.0f;
	m_center.y += 800.0f;
	if (m_isRabbit)
	{
		m_random = rand() % 601 - 300;
		m_center.x += m_random;
		m_random = rand() % 601 - 300;
		m_center.y += m_random;
	}

	else
	{
		m_random = rand() % 401 - 200;
		m_center.x += m_random;
		m_random = rand() % 401 - 200;
		m_center.y += m_random;
	}

	return true;
}

void ExpforKnight::OnDestroy()
{
	GameUI* gameUI = FindGO<GameUI>("m_gameUI");
	gameUI->ChangeEXPUpFlag(true);
}

void ExpforKnight::Update()
{
	Bezier();

	if (m_timer >= 0.98)
	{
		DeleteGO(this);
	}
	if (m_leftTimer >= 1)
	{
		DeleteGO(this);
	}
}

void ExpforKnight::Bezier()
{

	m_movePos.Lerp(m_leftTimer, m_effectScreenPosition, m_center);
	m_movePos2.Lerp(m_timer, m_center, m_experienceBarPosition);
	m_movePos3.Lerp(m_timer, m_movePos, m_movePos2);

	m_NormalExp.SetPosition(m_movePos3.x, m_movePos3.y, 0.0f);
	m_NormalExp.Update();

	if (m_leftTimer > 0.2f && m_leftTimer < 0.24f)
	{
		m_leftTimer += 0.02f * 0.2f;
		m_timer += 0.03f * 0.2f;
	}
	else
	{
		m_leftTimer += 0.02f;
		m_timer += 0.03f;
	}


}

void ExpforKnight::SetPlayerNumber()
{
	if (m_playerActor->IsMatchName("knightplayer"))
	{
		m_playerActorNumber = 0;
	}
	else if (m_playerActor->IsMatchName("knightplayer2"))
	{
		m_playerActorNumber = 1;
	}
	else if (m_playerActor->IsMatchName("knightplayer3"))
	{
		m_playerActorNumber = 2;
	}
	else if (m_playerActor->IsMatchName("knightplayer4"))
	{
		m_playerActorNumber = 3;
	}
	else if (m_playerActor->IsMatchName("KnightAI1"))
	{
		m_playerActorNumber = 3;
	}
}

Vector2 ExpforKnight::SetExperienceBarPosition()
{
	Vector2 position = Vector2::Zero;

	if (g_renderingEngine->GetGameMode() == RenderingEngine::enGameMode_SoloPlay)
	{
		position = EXPERIENCE_BAR_SOLO_POS;
	}
	else if (g_renderingEngine->GetGameMode() == RenderingEngine::enGameMode_DuoPlay)
	{
		if (m_playerActor->IsMatchName("knightplayer"))
		{
			position = EXPERIENCE_BAR_DUO_LEFT_POS;
		}
		else if (m_playerActor->IsMatchName("knightplayer2"))
		{
			position = EXPERIENCE_BAR_DUO_RIGHT_POS;
		}
	}
	else if (g_renderingEngine->GetGameMode() == RenderingEngine::enGameMode_TrioPlay || g_renderingEngine->GetGameMode() == RenderingEngine::enGameMode_QuartetPlay)
	{
		if (m_playerActor->IsMatchName("knightplayer"))
		{
			position = EXPERIENCE_BAR_LEFT_UP_POS;
		}
		else if (m_playerActor->IsMatchName("knightplayer2"))
		{
			position = EXPERIENCE_BAR_RIGHT_UP_POS;
		}
		else if (m_playerActor->IsMatchName("knightplayer3"))
		{
			position = EXPERIENCE_BAR_LEFT_DOWN_POS;
		}
		else if (m_playerActor->IsMatchName("knightplayer4"))
		{
			position = EXPERIENCE_BAR_RIGHT_DOWN_POS;
		}
		else if (m_playerActor->IsMatchName("KnightAI1"))
		{
			position = EXPERIENCE_BAR_RIGHT_DOWN_POS;
		}
	}

	return position;
}
void ExpforKnight::Render(RenderContext& rc)
{
	if (m_game->GetStopFlag())
	{
		return;
	}
	if (g_renderingEngine->GetGameMode() == RenderingEngine::enGameMode_SoloPlay)
	{
		m_NormalExp.Draw(rc);
	}
	else
	{
		m_NormalExp.Draw(rc, false, m_playerActorNumber);
	}
}

void ExpforKnight::Init(Vector3& pos, Actor* playerActor, const bool rabbitFlag)
{
	SetPosition(pos);
	SetIsRabbitFlag(rabbitFlag);
	m_playerActor = playerActor;
}