#include "stdafx.h"
#include "Fade.h"

namespace
{
	const float BLACK_SPRITE_WIDTH = 1920.0f;
	const float BLACK_SPRITE_HEIGHT = 1080.0f;
	const float BLACK_SPRITE_WIDTH_HALF = 1920.0f / 2;
	const Vector4 MUL_COLOR_VALUE = Vector4(1.0f, 1.0f, 1.0f, 0.0f);
	const Vector3 BLACK_SPRITE_HALF_LEFT = Vector3(-480.0f, 0.0f, 0.0f);
	const Vector3 BLACK_SPRITE_HALF_RIGHT = Vector3(480.0f, 0.0f, 0.0f);
}

Fade::Fade()
{

}

Fade::~Fade()
{

}

bool Fade::Start()
{
	m_blackSprite[enFadeSpriteType_Full].Init("Assets/sprite/Fade/Fade_Black.DDS", BLACK_SPRITE_WIDTH, BLACK_SPRITE_HEIGHT);
	m_blackSprite[enFadeSpriteType_Left].Init("Assets/sprite/Fade/Fade_Black.DDS", BLACK_SPRITE_WIDTH_HALF, BLACK_SPRITE_HEIGHT);
	m_blackSprite[enFadeSpriteType_Right].Init("Assets/sprite/Fade/Fade_Black.DDS", BLACK_SPRITE_WIDTH_HALF, BLACK_SPRITE_HEIGHT);
	
	m_blackSprite[enFadeSpriteType_Full].SetPosition(Vector3::Zero);
	m_blackSprite[enFadeSpriteType_Left].SetPosition(BLACK_SPRITE_HALF_LEFT);
	m_blackSprite[enFadeSpriteType_Right].SetPosition(BLACK_SPRITE_HALF_RIGHT);

	for (int i = 0; i < m_blackSprite.size(); i++)
	{
		m_blackSprite[i].SetRotation(Quaternion::Identity);
		m_blackSprite[i].SetScale(Vector3::One);
		m_blackSprite[i].SetMulColor(MUL_COLOR_VALUE);

		m_blackSprite[i].Update();
	}

	m_tipSprite.Init("Assets/sprite/Fade/TipsWon.DDS", BLACK_SPRITE_WIDTH, BLACK_SPRITE_HEIGHT);
	m_tipSprite.SetMulColor(MUL_COLOR_VALUE);

	return true;
}

void Fade::Update()
{
	FadeUpdate();
}

void Fade::FadeUpdate()
{
	if (m_fadeSpriteCategory == enFadeSpriteCategory_Black)
	{
		for (int i = 0; i < m_fadeSpriteCount; i++) {
			switch (m_blackFadeState[i])
			{
			case enFadeState_fadeIn:
				m_blackSpriteCurrentAlpha[i] += m_blackFadeSpeed[i] * g_gameTime->GetFrameDeltaTime();

				if (m_blackSpriteCurrentAlpha[i] >= 1.0f)
				{
					m_blackSpriteCurrentAlpha[i] = 1.0f;
					m_blackFadeState[i] = enFadeState_fadeIdle;
				}
				break;

			case enFadeState_fadeOut:
				m_blackSpriteCurrentAlpha[i] -= m_blackFadeSpeed[i] * g_gameTime->GetFrameDeltaTime();

				if (m_blackSpriteCurrentAlpha[i] <= 0.0f)
				{
					m_blackSpriteCurrentAlpha[i] = 0.0f;
					m_blackFadeState[i] = enFadeState_fadeIdle;
				}
				break;

			case enFadeState_fadeIdle:
				//何もしない
				break;
			}
		}
	}
	else if (m_fadeSpriteCategory == enFadeSpriteCategory_Tip)
	{
		switch (m_tipFadeState)
		{
		case enFadeState_fadeIn:
			m_tipSpriteCurrentAlpha += m_tipFadeSpeed * g_gameTime->GetFrameDeltaTime();

			if (m_tipSpriteCurrentAlpha >= 1.0f)
			{
				m_tipSpriteCurrentAlpha = 1.0f;
				m_tipFadeState = enFadeState_fadeIdle;
			}
			break;

		case enFadeState_fadeOut:
			m_tipSpriteCurrentAlpha -= m_tipFadeSpeed * g_gameTime->GetFrameDeltaTime();

			if (m_tipSpriteCurrentAlpha <= 0.0f)
			{
				m_tipSpriteCurrentAlpha = 0.0f;
				m_tipFadeState = enFadeState_fadeIdle;
			}
			break;

		case enFadeState_fadeIdle:
			//何もしない
			break;
		}
	}
}

void Fade::Render(RenderContext& rc)
{
	//黒のフェード画像
	if (m_fadeSpriteCategory == enFadeSpriteCategory_Black)
	{
		for (int i = 0; i < m_fadeSpriteCount; i++)
		{
			if (m_blackSpriteCurrentAlpha[i] > 0.0f)
			{
				m_blackSprite[i].SetMulColor({ 1.0f, 1.0f, 1.0f, m_blackSpriteCurrentAlpha[i] });
				m_blackSprite[i].Draw(rc, true);
			}
		}
	}
	//Tipのフェード画像
	else if (m_fadeSpriteCategory == enFadeSpriteCategory_Tip)
	{
		if (m_tipSpriteCurrentAlpha > 0.0f)
		{
			m_tipSprite.SetMulColor({ 1.0f,1.0f,1.0f,m_tipSpriteCurrentAlpha });
			m_tipSprite.Draw(rc, true);
		}
	}
}

void Fade::StartFadeIn(const float speed, const EnFadeSpriteType spriteType, const EnFadeSpriteCategory spriteCategory)
{
	if (spriteCategory == enFadeSpriteCategory_Black)
	{
		m_blackFadeSpeed[spriteType] = speed;
		m_blackFadeState[spriteType] = enFadeState_fadeIn;
	}
	else if (spriteCategory == enFadeSpriteCategory_Tip)
	{
		m_tipFadeSpeed = speed;
		m_tipFadeState = enFadeState_fadeIn;
	}
	m_fadeSpriteCategory = spriteCategory;
}

void Fade::StartFadeOut(const float speed, const EnFadeSpriteType spriteType, const EnFadeSpriteCategory spriteCategory)
{
	if (spriteCategory == enFadeSpriteCategory_Black)
	{
		m_blackFadeSpeed[spriteType] = speed;
		m_blackFadeState[spriteType] = enFadeState_fadeOut;
	}
	else if (spriteCategory == enFadeSpriteCategory_Tip)
	{
		m_tipFadeSpeed = speed;
		m_tipFadeState = enFadeState_fadeOut;
	}
	m_fadeSpriteCategory = spriteCategory;
}

