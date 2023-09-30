#include "k2EngineLowPreCompile.h"
#include "FontRender.h"

void nsK2EngineLow::FontRender::Draw(RenderContext& rc, const int viewportNo)
{
	if (viewportNo == -1)
	{
		g_renderingEngine->AddFontList(this);
	}
	else
	{
		g_renderingEngine->AddFontDrawViewportList(this, viewportNo);
	}
}
