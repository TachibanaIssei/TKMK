#pragma once
#include "Bloom.h"

namespace nsK2EngineLow {
	class PostEffect : public Noncopyable
	{
	public:
		void Init(RenderTarget& mainRenderTarget);
		void Render(RenderContext& rc,RenderTarget& mainRenderTarget);

	private:
		void InitBloom(RenderTarget& mainRenderTarget);

	private:
		Bloom		m_bloom;
	};
}
