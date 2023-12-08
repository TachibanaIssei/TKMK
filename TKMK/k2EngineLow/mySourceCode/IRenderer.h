#pragma once
namespace nsK2EngineLow {
	/// <summary>
	/// レンダーラーの基底クラス
	/// </summary>
	class IRenderer : public Noncopyable
	{
	public:
		/// <summary>
		/// シャドウマップへの描画パスから呼ばれる処理。
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		/// <param name="ligNo">ライト番号</param>
		/// <param name="lvpMatrix">ライトビュープロジェクション行列</param>
		virtual void OnRenderShadowMap(
			RenderContext& rc,
			const int ligNo,
			const int shadowMapNo,
			const Matrix& lvpMatrix,
			const int viewportNumber
		)
		{

		}
		/// <summary>
		/// ZPrepassから呼ばれる処理
		/// </summary>
		/// <param name="rc"></param>
		virtual void OnZPrepass(RenderContext& rc)
		{

		}
		/// <summary>
		/// フォワードレンダーパスから呼ばれる処理
		/// </summary>
		/// <param name="rc"></param>
		virtual void  OnForwardRender(RenderContext& rc)
		{

		}
		/// <summary>
		/// G-Buffer描画パスから呼ばれる処理。
		/// </summary>
		virtual void OnRenderToGBuffer(RenderContext& rc)
		{

		}
		/// <summary>
		/// 半透明オブジェクト描画パスから呼ばれる処理。
		/// </summary>
		/// <param name="rc"></param>
		virtual void OnTlanslucentRender(RenderContext& rc)
		{

		}
		/// <summary>
		/// 2Dオブジェクトから呼ばれる処理
		/// </summary>
		/// <param name="rc"></param>
		virtual void OnRender2D(RenderContext& rc)
		{

		}

		virtual void OnRenderShadowModel(RenderContext& rc, Camera& camera, const int number)
		{

		}
	};
}