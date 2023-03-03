#pragma once

#include "Light.h"

namespace nsK2EngineLow {
	class ModelRender;
	class SpriteRender;
	class FontRender;
	class RenderContext;

	class RenderingEngine
	{
	public:
		void Init();

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

		SceneLight& GetSceneLight()
		{
			return m_sceneLight;
		}

		/// <summary>
		/// ディレクションライトを設定
		/// </summary>
		/// <param name="lightNo">ライト番号</param>
		/// <param name="direction">ライト方向</param>
		/// <param name="color">ライト色</param>
		void SetDirectionLight(int lightNo, Vector3 direction, Vector3 color)
		{
			m_sceneLight.SetDirectionLight(lightNo, direction, color);
		}

		/// <summary>
		/// 環境光を設定
		/// </summary>
		/// <param name="ambient">環境光</param>
		void SetAmbient(Vector3 ambient)
		{
			m_sceneLight.SetAmbient(ambient);
		}

	private:
		std::vector<ModelRender*>	m_modelList;				//モデルクラスのリスト
		std::vector<SpriteRender*>	m_spriteList;				//スプライトクラスのリスト
		std::vector<FontRender*>	m_fontList;					//フォントクラスのリスト

		SceneLight					m_sceneLight;				//シーンライト
	};
}