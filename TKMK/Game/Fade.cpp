#include "stdafx.h"
#include "Fade.h"

namespace
{
	const float BLACK_SPRITE_WIDTH = 1920.0f;
	const float BLACK_SPRITE_HEIGHT = 1080.0f;
	const float BLACK_SPRITE_WIDTH_HALF = 1920.0f / 2;
	const Vector4 MUL_COLOR_VALUE = Vector4(1.0f, 1.0f, 1.0f, 0.0f);
	const Vector3 BLACK_SPRITE_HALF_LEFT = Vector3(-480.0f, 0.0f, 1.0f);
	const Vector3 BLACK_SPRITE_HALF_RIGHT = Vector3(480.0f, 0.0f, 1.0f);
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
	
	for (int i = 0; i < m_blackSprite.size(); i++)
	{
		m_blackSprite[i].SetRotation(Quaternion::Identity);
		m_blackSprite[i].SetScale(Vector3::One);
		m_blackSprite[i].SetMulColor(MUL_COLOR_VALUE);
	}
	m_blackSprite[enFadeSpriteType_Full].SetPosition(Vector3::Zero);
	m_blackSprite[enFadeSpriteType_Left].SetPosition(BLACK_SPRITE_HALF_LEFT);
	m_blackSprite[enFadeSpriteType_Right].SetPosition(BLACK_SPRITE_HALF_RIGHT);

	return true;
}

void Fade::Update()
{
	FadeUpdate();
}

void Fade::FadeUpdate()
{
	for (int i = 0; i < m_fadeSpriteCount; i++) {
		switch (m_fadeState[i])
		{
		case enFadeState_fadeIn:
			m_currentAlpha[i] += m_fadeSpeed[i] * g_gameTime->GetFrameDeltaTime();

			if (m_currentAlpha[i] >= 1.0f)
			{
				m_currentAlpha[i] = 1.0f;
				m_fadeState[i] = enFadeState_fadeIdle;
			}
			break;

		case enFadeState_fadeOut:
			m_currentAlpha[i] -= m_fadeSpeed[i] * g_gameTime->GetFrameDeltaTime();

			if (m_currentAlpha[i] <= 0.0f)
			{
				m_currentAlpha[i] = 0.0f;
				m_fadeState[i] = enFadeState_fadeIdle;
			}
			break;

		case enFadeState_fadeIdle:
			//‰½‚à‚µ‚È‚¢
			break;
		}
	}
}

void Fade::Render(RenderContext& rc)
{
	for (int i = 0; i < m_fadeSpriteCount; i++)
	{
		if (m_currentAlpha[i] > 0.0f)
		{
			m_blackSprite[i].SetMulColor({ 1.0f, 1.0f, 1.0f, m_currentAlpha[i] });
			m_blackSprite[i].Draw(rc, true);
		}
	}
}

