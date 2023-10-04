#include "stdafx.h"
#include "EnemyHpBar.h"

#include "Status.h"

namespace
{
	const float HP_WINDOW_WIDTH = 1152.0f;
	const float HP_WINDOW_HEIGHT = 648.0f;
	const float HP_BER_WIDTH = 178.0f;
	const float HP_BER_HEIGHT = 22.0f;
	const Vector3 HP_BER_SIZE = Vector3(HP_BER_WIDTH, HP_BER_HEIGHT, 0.0f);
	const float LEVEL_FONT_ADD_POS_X = -30.0f;
	const float LEVEL_FONT_ADD_POS_Y = 50.0f;
}

EnemyHpBar::~EnemyHpBar()
{
}

bool EnemyHpBar::Start()
{
	m_HP_Bar.Init("Assets/sprite/zako_HP_bar.DDS", HP_BER_WIDTH, HP_BER_HEIGHT);
	m_HP_Back.Init("Assets/sprite/zako_HP_background.DDS", HP_WINDOW_WIDTH, HP_WINDOW_HEIGHT);
	m_HP_Frame.Init("Assets/sprite/HP_flame_mushroom.DDS", HP_WINDOW_WIDTH, HP_WINDOW_HEIGHT);

	//HPバーの上に表示するレベル
	m_fontLv.SetColor(g_vec4White);
	m_fontLv.SetShadowParam(true, 0.2f, g_vec4Black);
	m_fontLv.SetScale(0.5f);

	return true;
}

void EnemyHpBar::Update()
{
	m_HP_Bar.Update();
	m_HP_Frame.Update();
	m_HP_Back.Update();
}

void EnemyHpBar::Render(RenderContext& rc)
{
	if (m_drawFlag)
	{
		m_HP_Back.Draw(rc, false, m_viewportNo);
		m_HP_Bar.Draw(rc, false, m_viewportNo);
		m_HP_Frame.Draw(rc, false, m_viewportNo);
		m_fontLv.Draw(rc, m_viewportNo);
	}
}

void EnemyHpBar::Init(const int viewportNo)
{
	m_viewportNo = viewportNo;
}

void EnemyHpBar::CalcHpBarPosition(const int cameraNo, Status* status, const Vector3& characterPosition, const int lv)
{
	if (status->GetHp() < 0)
	{
		status->SetHp(0);
	}

	Vector3 scale = Vector3::One;
	scale.x = float(status->GetHp()) / float(status->GetMaxHp());
	m_HP_Bar.SetScale(scale);

	Vector2 levelFontPos = Vector2::Zero;
	Vector3 berPosition = characterPosition;
	berPosition.y += 75.0f;
	//座標を変換する
	g_camera3D[cameraNo]->CalcScreenPositionFromWorldPosition(levelFontPos, berPosition);
	g_camera3D[cameraNo]->CalcScreenPositionFromWorldPositionMultiPlay(m_HPBer_Pos, berPosition);
	g_camera3D[cameraNo]->CalcScreenPositionFromWorldPositionMultiPlay(m_HPWindow_Pos, berPosition);
	g_camera3D[cameraNo]->CalcScreenPositionFromWorldPositionMultiPlay(m_HPBack_Pos, berPosition);

	m_levelFontPos = levelFontPos;
	m_levelFontPos.x += LEVEL_FONT_ADD_POS_X;
	m_levelFontPos.y += LEVEL_FONT_ADD_POS_Y;

	//HPバー画像を左寄せに表示する
	Vector3 BerSizeSubtraction = LeftAlignHPBar(HP_BER_SIZE, scale);	//画像の元の大きさ
	m_HPBer_Pos.x -= BerSizeSubtraction.x;

	//HPバーの横に表示するレベル
	wchar_t level[255];
	swprintf_s(level, 255, L"Lv%d", lv);
	m_fontLv.SetText(level);
	m_fontLv.SetPosition(m_levelFontPos.x, m_levelFontPos.y, 0.0f);

	//HPバーの座標
	m_HP_Bar.SetPosition(Vector3(m_HPBer_Pos.x, m_HPBer_Pos.y, 0.0f));
	m_HP_Frame.SetPosition(Vector3(m_HPWindow_Pos.x, m_HPWindow_Pos.y, 0.0f));
	m_HP_Back.SetPosition(Vector3(m_HPBack_Pos.x, m_HPBack_Pos.y, 0.0f));

	m_HP_Bar.Update();
	m_HP_Frame.Update();
	m_HP_Back.Update();
}

Vector3 EnemyHpBar::LeftAlignHPBar(const Vector3& size, const Vector3& scale)
{
	Vector3 hpBerSize = size;								//画像の元の大きさ
	Vector3 changeBerSize = Vector3::Zero;					//画像をスケール変換したあとの大きさ
	Vector3 BerSizeSubtraction = Vector3::Zero;				//画像の元と変換後の差

	changeBerSize.x = hpBerSize.x * scale.x;
	BerSizeSubtraction.x = hpBerSize.x - changeBerSize.x;
	BerSizeSubtraction.x /= 2.0f;

	return BerSizeSubtraction;
}
