#pragma once

namespace nsK2EngineLow {
	class ModelRender;
	class RenderContext;

	class RenderingEngine
	{
	public:
		void AddModelList(ModelRender* modelRender)
		{
			m_modelList.push_back(modelRender);
		}

		void ModelRendering(RenderContext& rc);

		void Execute(RenderContext& rc);

	private:
		std::vector<ModelRender*> m_modelList;

	};
}