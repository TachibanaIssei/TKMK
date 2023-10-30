#include "k2EngineLowPreCompile.h"
#include "PostEffect.h"
#include "graphics/effect/EffectEngine.h"

namespace nsK2EngineLow
{
	void PostEffect::Init(RenderTarget& mainRenderTarget)
	{
		InitBloom(mainRenderTarget);
	}

	void PostEffect::InitBloom(RenderTarget& mainRenderTarget)
	{
		m_bloom.Init(mainRenderTarget);
	}

	void PostEffect::Render(RenderContext& rc, RenderTarget& mainRenderTarget)
	{
		m_bloom.OnRender(rc, mainRenderTarget);
	}
}