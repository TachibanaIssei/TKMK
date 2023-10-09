#include "k2EngineLowPreCompile.h"
#include "FontRender.h"

void nsK2EngineLow::FontRender::Draw(RenderContext& rc, const bool drawTiming, const int viewportNo)
{
	if (viewportNo == -1)
	{
		if (drawTiming)
		{
			g_renderingEngine->AddFontList(this, drawTiming);
		}
		else
		{
			g_renderingEngine->AddFontList(this);
		}
	}
	else
	{
		g_renderingEngine->AddFontDrawViewportList(this, viewportNo);
	}
}
