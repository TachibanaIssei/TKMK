#pragma once

namespace nsK2EngineLow {
	class ModelRender;
	class SpriteRender;
	class FontRender;
	class RenderContext;

	struct DirectionalLight
	{
		Vector3 lightDirection;	//ライトの方向
		float pad0;
		Vector3 ligColor;		//ライトのカラー
		float pad1;
		Vector3 CameraEyePos;	//カメラ座標
	};

	class RenderingEngine
	{
	public:
		/// <summary>
		/// モデルレンダークラスをリストに追加する
		/// </summary>
		/// <param name="modelRender">モデルレンダー</param>
		void AddModelList(ModelRender* modelRender)
		{
			m_modelList.push_back(modelRender);
		}
		/// <summary>
		/// スプライトレンダークラスをリストに追加する
		/// </summary>
		/// <param name="spriteRender">スプライトレンダー</param>
		void AddSpriteList(SpriteRender* spriteRender)
		{
			m_spriteList.push_back(spriteRender);
		}
		/// <summary>
		/// フォントレンダークラスをリストに追加する
		/// </summary>
		/// <param name="fontRender">フォントレンダー</param>
		void AddFontList(FontRender* fontRender)
		{
			m_fontList.push_back(fontRender);
		}

		/// <summary>
		/// モデルを描画する
		/// </summary>
		/// <param name="rc">レンダーコンテキスト</param>
		void ModelRendering(RenderContext& rc);
		/// <summary>
		/// スプライトを描画する
		/// </summary>
		/// <param name="rc">レンダーコンテキスト</param>
		void SpriteRendering(RenderContext& rc);
		/// <summary>
		/// フォントを描画する
		/// </summary>
		/// <param name="rc">レンダーコンテキスト</param>
		void FontRendering(RenderContext& rc);

		/// <summary>
		/// 描画処理を実行
		/// </summary>
		/// <param name="rc">レンダーコンテキスト</param>
		void Execute(RenderContext& rc);

		/// <summary>
		/// ディレクションライトを取得する
		/// </summary>
		/// <returns>ディレクションライト</returns>
		DirectionalLight* GetDirectionalLight()
		{
			return &m_directionLight;
		}

		/// <summary>
		/// ディレクションライトを設定する
		/// </summary>
		/// <param name="dir">ライトの方向</param>
		/// <param name="color">ライトの色</param>
		void SetDirectionLight(Vector3 dir,Vector3 color = {0.5f,0.5f,0.5f})
		{
			m_directionLightDir = dir;
			
			m_directionLightColor = color;
		}
		

		void MakeDirectionLight();

	private:
		std::vector<ModelRender*>	m_modelList;											//モデルクラスのリスト
		std::vector<SpriteRender*>	m_spriteList;											//スプライトクラスのリスト
		std::vector<FontRender*>	m_fontList;												//フォントクラスのリスト

		DirectionalLight			m_directionLight;										//ディレクションライト
		Vector3						m_directionLightDir		= Vector3{ 1.0f,-1.0f,-1.0f };	//ディレクションライトの方向
		Vector3						m_directionLightColor	= Vector3{0.5f,0.5f,0.5f};		//ディレクションライトの色
	};
}