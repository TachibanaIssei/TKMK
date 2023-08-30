#include "stdafx.h"
#include "Fade.h"
namespace
{
	const float SPRITE_WIDTH = 1920.0f;
	const float SPRITE_HEIGHT = 1080.0f;
	const float MAX_ALPHA = 1.0f;
	const float MIN_ALPHA = 0.0f;
	const Vector3 SPRITE_COLOR = Vector3::One;
}

Fade::Fade()
{
}

Fade::~Fade()
{
}

bool Fade::Start()
{
	BlackSprite.Init("Assets/sprite/Fade/Fade_Black.DDS", SPRITE_WIDTH, SPRITE_HEIGHT);
	BlackSprite.SetMulColor(Vector4(SPRITE_COLOR.x, SPRITE_COLOR.y, SPRITE_COLOR.z, MIN_ALPHA));
	TipSprite.Init("Assets/sprite/Fade/TipsWon.DDS", SPRITE_WIDTH, SPRITE_HEIGHT);
	TipSprite.SetMulColor(Vector4(SPRITE_COLOR.x, SPRITE_COLOR.y, SPRITE_COLOR.z, MIN_ALPHA));
	return true;
}

void Fade::Update()
{
	switch (m_FadeState)
	{
	case FadeIn:
		m_currentAlpha += m_fadeSpeed * g_gameTime->GetFrameDeltaTime();
		if (m_currentAlpha >= MAX_ALPHA)
		{
			m_currentAlpha = MAX_ALPHA;
			m_FadeState = FadeIdle;
		}
		break;
	case FadeOut:
		m_currentAlpha -= m_fadeSpeed * g_gameTime->GetFrameDeltaTime();
		if (m_currentAlpha <= MIN_ALPHA)
		{
			m_currentAlpha = MIN_ALPHA;
			m_FadeState = FadeIdle;
		}
		break;

	case FadeIdle:
		break;
	}
}

void Fade::Render(RenderContext& rc)
{
	if (m_currentAlpha > MIN_ALPHA)
	{
		switch (m_enFadeSprite)
		{
		case enFadeSprite_BlackSprite:
			BlackSprite.SetMulColor({ SPRITE_COLOR.x, SPRITE_COLOR.y, SPRITE_COLOR.z, m_currentAlpha });
			BlackSprite.Draw(rc, true);
			break;
		case enFadeSprite_TipSprite:
			TipSprite.SetMulColor({ SPRITE_COLOR.x, SPRITE_COLOR.y, SPRITE_COLOR.z, m_currentAlpha });
			TipSprite.Draw(rc, true);
		}
	}
}