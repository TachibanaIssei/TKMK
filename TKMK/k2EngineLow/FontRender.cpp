#include "k2EngineLowPreCompile.h"
#include "FontRender.h"

nsK2EngineLow::FontRender::FontRender()
{
}

nsK2EngineLow::FontRender::~FontRender()
{
}

void nsK2EngineLow::FontRender::Draw(RenderContext& rc)
{
	g_renderingEngine->AddFontList(this);
}
