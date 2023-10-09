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
	const float LEVEL_SPRITE_WIDTH = 320.0f;
	const float LEVEL_SPRITE_HEIGHT = 100.0f;
	const Vector3 LEVEL_SPRITE_SCALE = Vector3(0.6f, 0.6f, 1.0f);
	const Vector2 LEVEL_SPRITE_PIVOT = Vector2(0.5f, 0.25f);		//HPバーと座標を合わせるためにピボットをHPバーと合わせる
}

EnemyHpBar::~EnemyHpBar()
{
}

bool EnemyHpBar::Start()
{
	InitLvSprite();
	m_hpBar.Init("Assets/sprite/zako_HP_bar.DDS", HP_BER_WIDTH, HP_BER_HEIGHT);
	m_hpBack.Init("Assets/sprite/zako_HP_background.DDS", HP_WINDOW_WIDTH, HP_WINDOW_HEIGHT);
	return true;
}

void EnemyHpBar::Update()
{
	m_hpBar.Update();
	m_hpBack.Update();
	m_lvSprite[m_level].Update();
}

void EnemyHpBar::Render(RenderContext& rc)
{
	if (m_drawFlag)
	{
		m_hpBack.Draw(rc, false, m_viewportNo);
		m_hpBar.Draw(rc, false, m_viewportNo);
		m_lvSprite[m_level].Draw(rc, false, m_viewportNo);
	}
}

void EnemyHpBar::Init(const int viewportNo)
{
	m_viewportNo = viewportNo;
}

void EnemyHpBar::CalcHpBarPosition(const int cameraNo, Status* status, const Vector3& characterPosition, const int lv)
{
	SetCharacterLevel(lv);

	if (status->GetHp() < 0)
	{
		status->SetHp(0);
	}

	Vector3 scale = Vector3::One;
	scale.x = float(status->GetHp()) / float(status->GetMaxHp());
	m_hpBar.SetScale(scale);

	Vector2 levelFontPos = Vector2::Zero;
	Vector3 berPosition = characterPosition;
	berPosition.y += 75.0f;
	//座標を変換する
	g_camera3D[cameraNo]->CalcScreenPositionFromWorldPositionMultiPlay(m_levelSpritePosition, berPosition);
	g_camera3D[cameraNo]->CalcScreenPositionFromWorldPositionMultiPlay(m_hpBerPosition, berPosition);
	g_camera3D[cameraNo]->CalcScreenPositionFromWorldPositionMultiPlay(m_hpBackPosition, berPosition);

	//HPバー画像を左寄せに表示する
	Vector3 BerSizeSubtraction = LeftAlignHPBar(HP_BER_SIZE, scale);	//画像の元の大きさ
	m_hpBerPosition.x -= BerSizeSubtraction.x;

	//HPバーの座標
	m_lvSprite[m_level].SetPosition(Vector3(m_levelSpritePosition.x, m_levelSpritePosition.y, 0.0f));
	m_hpBar.SetPosition(Vector3(m_hpBerPosition.x, m_hpBerPosition.y, 0.0f));
	m_hpBack.SetPosition(Vector3(m_hpBackPosition.x, m_hpBackPosition.y, 0.0f));

	m_hpBar.Update();
	m_hpBack.Update();
	m_lvSprite[m_level].Update();

}

void EnemyHpBar::InitLvSprite()
{
	m_lvSprite[enCharacterLevel_One].Init("Assets/sprite/gameUI/HPBar_flame01.DDS", LEVEL_SPRITE_WIDTH, LEVEL_SPRITE_HEIGHT);
	m_lvSprite[enCharacterLevel_Two].Init("Assets/sprite/gameUI/HPBar_flame02.DDS", LEVEL_SPRITE_WIDTH, LEVEL_SPRITE_HEIGHT);
	m_lvSprite[enCharacterLevel_Three].Init("Assets/sprite/gameUI/HPBar_flame03.DDS", LEVEL_SPRITE_WIDTH, LEVEL_SPRITE_HEIGHT);
	m_lvSprite[enCharacterLevel_Four].Init("Assets/sprite/gameUI/HPBar_flame04.DDS", LEVEL_SPRITE_WIDTH, LEVEL_SPRITE_HEIGHT);
	m_lvSprite[enCharacterLevel_Five].Init("Assets/sprite/gameUI/HPBar_flame05.DDS", LEVEL_SPRITE_WIDTH, LEVEL_SPRITE_HEIGHT);
	m_lvSprite[enCharacterLevel_Six].Init("Assets/sprite/gameUI/HPBar_flame06.DDS", LEVEL_SPRITE_WIDTH, LEVEL_SPRITE_HEIGHT);
	m_lvSprite[enCharacterLevel_Seven].Init("Assets/sprite/gameUI/HPBar_flame07.DDS", LEVEL_SPRITE_WIDTH, LEVEL_SPRITE_HEIGHT);
	m_lvSprite[enCharacterLevel_Eight].Init("Assets/sprite/gameUI/HPBar_flame08.DDS", LEVEL_SPRITE_WIDTH, LEVEL_SPRITE_HEIGHT);
	m_lvSprite[enCharacterLevel_Nine].Init("Assets/sprite/gameUI/HPBar_flame09.DDS", LEVEL_SPRITE_WIDTH, LEVEL_SPRITE_HEIGHT);
	m_lvSprite[enCharacterLevel_Ten].Init("Assets/sprite/gameUI/HPBar_flame10.DDS", LEVEL_SPRITE_WIDTH, LEVEL_SPRITE_HEIGHT);

	for (int i = 0; i < m_maxLevel; i++)
	{
		m_lvSprite[i].SetScale(LEVEL_SPRITE_SCALE);
		m_lvSprite[i].SetPivot(LEVEL_SPRITE_PIVOT);
	}
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

void EnemyHpBar::SetCharacterLevel(const int level)
{
	switch (level)
	{
	case 1:
		m_level = enCharacterLevel_One;
		break;
	case 2:
		m_level = enCharacterLevel_Two;
		break;
	case 3:
		m_level = enCharacterLevel_Three;
		break;
	case 4:
		m_level = enCharacterLevel_Four;
		break;
	case 5:
		m_level = enCharacterLevel_Five;
		break;
	case 6:
		m_level = enCharacterLevel_Six;
		break;
	case 7:
		m_level = enCharacterLevel_Seven;
		break;
	case 8:
		m_level = enCharacterLevel_Eight;
		break;
	case 9:
		m_level = enCharacterLevel_Nine;
		break;
	case 10:
		m_level = enCharacterLevel_Ten;
		break;
	default:
		m_level = enCharacterLevel_One;
		break;
	}
}
