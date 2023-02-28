#pragma once

namespace nsK2EngineLow {
	class ModelRender;
	class SpriteRender;
	class FontRender;
	class RenderContext;

	class RenderingEngine
	{
	public:
		void AddModelList(ModelRender* modelRender)
		{
			m_modelList.push_back(modelRender);
		}
		void AddSpriteList(SpriteRender* spriteRender)
		{
			m_spriteList.push_back(spriteRender);
		}
		void AddFontList(FontRender* fontRender)
		{
			m_fontList.push_back(fontRender);
		}

		void ModelRendering(RenderContext& rc);
		void SpriteRendering(RenderContext& rc);
		void FontRendering(RenderContext& rc);

		void Execute(RenderContext& rc);

	private:
		std::vector<ModelRender*> m_modelList;
		std::vector<SpriteRender*> m_spriteList;
		std::vector<FontRender*> m_fontList;

	};
}