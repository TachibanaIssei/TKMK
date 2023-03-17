#include "k2EngineLowPreCompile.h"
#include "PostEffect.h"

void nsK2EngineLow::PostEffect::InitBloom(RenderTarget& mainRenderTarget)
{
	m_bloom.Init(mainRenderTarget);
}

void nsK2EngineLow::PostEffect::Render(RenderContext& rc, RenderTarget& mainRenderTarget)
{
	m_bloom.OnRender(rc, mainRenderTarget);
}
