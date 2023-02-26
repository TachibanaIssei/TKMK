#include "k2EngineLowPreCompile.h"
#include "FontRender.h"

void nsK2EngineLow::FontRender::Draw(RenderContext& rc)
{
	m_font.Begin(rc);
	m_font.Draw(m_text, Vector2(m_position.x, m_position.y), m_color, m_rotation, m_scale, m_pivot);
	m_font.End(rc);

}
