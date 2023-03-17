#pragma once
#include "Bloom.h"

namespace nsK2EngineLow {
	class PostEffect
	{
	public:
		void InitBloom(RenderTarget& mainRenderTarget);
		void Render(RenderContext& rc,RenderTarget& mainRenderTarget);

	private:
		Bloom m_bloom;
	};
}
