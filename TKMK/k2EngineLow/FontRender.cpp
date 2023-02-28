#include "k2EngineLowPreCompile.h"
#include "FontRender.h"

void nsK2EngineLow::FontRender::Draw(RenderContext& rc)
{
	g_renderingEngine->AddFontList(this);
}
