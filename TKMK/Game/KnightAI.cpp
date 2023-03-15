#include "stdafx.h"
#include "KnightAI.h"
#include "Game.h"
namespace {
	const Vector2 AVOIDANCE_BAR_POVOT = Vector2(1.0f, 1.0f);
	const Vector3 AVOIDANCE_BAR_POS = Vector3(98.0f, -397.0f, 0.0f);

	const Vector3 AVOIDANCE_FLAME_POS = Vector3(0.0f, -410.0f, 0.0f);
}


KnightAI::KnightAI()
{
	SetModel();
	//アニメーションイベント用の関数を設定する。
	/*m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});*/
	//リスポーンする座標0番の取得
	GetRespawnPos();
	respawnNumber = 1;        //リスポーンする座標の番号
	m_respawnPos[respawnNumber].y += m_position_YUp;
	//リスポーンする座標のセット
	//キャラコン
	m_charCon.SetPosition(m_respawnPos[respawnNumber]);
	//剣士
	m_modelRender.SetPosition(m_respawnPos[respawnNumber]);

	//スキルのクールタイムを表示するフォントの設定
	/*Skillfont.SetPosition(805.0f, -400.0f, 0.0f);
	Skillfont.SetScale(2.0f);
	Skillfont.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	Skillfont.SetRotation(0.0f);
	Skillfont.SetShadowParam(true, 2.0f, g_vec4Black);*/

	//回避のフレームの設定
	//m_Avoidance_flameRender.Init("Assets/sprite/avoidance_flame.DDS", 300, 50);
	//m_Avoidance_flameRender.SetPosition(AVOIDANCE_FLAME_POS);
	////回避のバーの設定
	//m_Avoidance_barRender.Init("Assets/sprite/avoidance_bar.DDS", 194, 26);
	//m_Avoidance_barRender.SetPivot(AVOIDANCE_BAR_POVOT);
	//m_Avoidance_barRender.SetPosition(AVOIDANCE_BAR_POS);
}
KnightAI::~KnightAI()
{

}
void KnightAI::Update()
{
	m_modelRender.Update();
}
void KnightAI::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{

}
void KnightAI::AvoidanceSprite()
{

}
void KnightAI::Attack()
{

}

void KnightAI::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}

