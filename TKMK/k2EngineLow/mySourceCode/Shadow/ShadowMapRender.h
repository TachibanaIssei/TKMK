#pragma once
#include "CascadeShadowMapMatrix.h"

namespace nsK2EngineLow
{
	class IRenderere;
	class ShadowMapRender : public Noncopyable
	{
	public:
		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="isSoftShadow"></param>
		void Init(const bool isSoftShadow);
		/// <summary>
		/// 描画
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		/// <param name="ligNo">ライト番号</param>
		/// <param name="lightDirection">ライトの方向</param>
		/// <param name="renderObjects">描画オブジェクト</param>
		void Render(
			RenderContext& rc,
			int ligNo,
			Vector3& lightDirection,
			std::vector<IRenderer*> renderObjects,
			const int cameraNumber
		);

		/// <summary>
			/// シャドウマップを取得
			/// </summary>
			/// <param name="areaNo">エリア番号</param>
			/// <returns></returns>
		Texture& GetShadowMap(const int areaNo)
		{
			if (m_isSoftShadow)
			{
				return m_gausBlur[areaNo].GetBokeTexture();
			}
			return m_shadowMaps[areaNo].GetRenderTargetTexture();
		}
		/// <summary>
		/// ライトビュープロジェクション行列を取得
		/// </summary>
		/// <param name="areaNo"></param>
		/// <returns></returns>
		const Matrix& GetLVPMatrix(const int areaNo) const
		{
			return m_cascadeShadowMapMatrix.GetLightViewProjectionCropMatrix(areaNo);
		}

	private:
		CascadeShadowMapMatrix		m_cascadeShadowMapMatrix;										//カスケードシャドウマップの行列を扱うオブジェクト
		RenderTarget				m_shadowMaps[NUM_SHADOW_MAP];									//レンダーターゲット
		float						m_cascadeAreaRateArray[NUM_SHADOW_MAP] = { 0.05f, 0.3f,1.0f };	//カスケードシャドウの近～遠距離の比率
		GaussianBlur				m_gausBlur[NUM_SHADOW_MAP];										//シャドウマップにブラーをかける。ソフトシャドウで使用
		bool						m_isSoftShadow = false;											//ソフトシャドウを行う？
	};
}