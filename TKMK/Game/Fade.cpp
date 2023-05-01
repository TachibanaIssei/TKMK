#include "stdafx.h"
#include "Fade.h"

Fade::Fade()
{

}

Fade::~Fade()
{

}

bool Fade::Start()
{
	BlackSprite.Init("Assets/sprite/Fade/Fade_Black.DDS", 1920.0f, 1080.0f);

	BlackSprite.SetPosition(Vector3::Zero);
	BlackSprite.SetRotation(Quaternion::Identity);
	BlackSprite.SetScale(Vector3::One);
	BlackSprite.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 0.0f));

	return true;
}

void Fade::Update()
{
	switch (m_FadeState)
	{
	case FadeIn:
		m_currentAlpha += m_fadeSpeed * g_gameTime->GetFrameDeltaTime();

		if (m_currentAlpha >= 1.0f)
		{
			m_currentAlpha = 1.0f;
			m_FadeState = FadeIdle;
		}

		break;

	case FadeOut:
		m_currentAlpha -= m_fadeSpeed * g_gameTime->GetFrameDeltaTime();

		if (m_currentAlpha <= 0.0f)
		{
			m_currentAlpha = 0.0f;
			m_FadeState = FadeIdle;
		}
		break;

	case FadeIdle:

		break;

	default:
		break;
	}
}

void Fade::Render(RenderContext& rc)
{
	if (m_currentAlpha > 0.0f)
	{
		BlackSprite.SetMulColor({ 1.0f, 1.0f, 1.0f, m_currentAlpha });
		BlackSprite.Draw(rc);
	}
	
}
