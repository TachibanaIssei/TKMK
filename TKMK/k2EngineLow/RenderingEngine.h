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
		/// ディレクションライトの光の方向を取得する
		/// </summary>
		/// <returns>光の方向</returns>
		Vector3& GetDirLigDirection()
		{
			return m_sceneLight.GetDirLigDirection();
		}

		/// <summary>
		/// 環境光を設定
		/// </summary>
		/// <param name="ambient">環境光</param>
		void SetAmbient(Vector3 ambient)
		{
			m_sceneLight.SetAmbient(ambient);
		}

		/// <summary>
		/// ポイントライトを設定する
		/// </summary>
		/// <param name="pos">ライトの位置</param>
		/// <param name="color">ライトの色</param>
		/// <param name="range">xにライトの影響範囲,yに影響範囲に累乗するパラメータ</param>
		void SetPointLight(Vector3 pos, Vector3 color, Vector3 range)
		{
			m_sceneLight.SetPointLight(pos, color, range);
		}
		void SetPointLightPosition(Vector3 pos)
		{
			m_sceneLight.SetPointLightPosition(pos);
		}
		/// <summary>
		/// スポットライトを設定する
		/// </summary>
		/// <param name="pos">位置</param>
		/// <param name="color">色</param>
		/// <param name="range">xに影響範囲,yに影響範囲に累乗するパラメータ</param>
		/// <param name="direction">照射方向</param>
		/// <param name="angle">xは照射角度,ｙは影響に累乗するパラメータ</param>
		void SetSpotLight(Vector3 pos, Vector3 color, Vector3 range, Vector3 direction, Vector3 angle)
		{
			m_sceneLight.SetSpotLight(pos, color, range, direction, angle);
		}
		void SetSpotLightPosition(Vector3 pos)
		{
			m_sceneLight.SetSpotLightPosition(pos);
		}
		Vector3& GetSpotLightDirection()
		{
			return m_sceneLight.GetSpotLightDirection();
		}

		/// <summary>
		/// 半球ライトを設定する
		/// </summary>
		/// <param name="groundColor">地面色</param>
		/// <param name="skyColor">天球色</param>
		/// <param name="groundNormal">地面の法線</param>
		void SetHemiLight(Vector3 groundColor, Vector3 skyColor, Vector3 groundNormal)
		{
			SetHemiLightGroundColor(groundColor);
			SetHemiLightSkyColor(skyColor);
			SetHemiLightGroundNormal(groundNormal);
			UseHemiLight();
		}
		/// <summary>
		/// 半球ライトの地面色を設定
		/// </summary>
		/// <param name="groundColor">地面色</param>
		void SetHemiLightGroundColor(Vector3 groundColor)
		{
			m_sceneLight.SetHemiLightGroundColor(groundColor);
		}
		/// <summary>
		/// 半球ライトの天球色を設定
		/// </summary>
		/// <param name="skyColor">天球色</param>
		void SetHemiLightSkyColor(Vector3 skyColor)
		{
			m_sceneLight.SetHemiLightSkyColor(skyColor);
		}
		/// <summary>
		/// 半球ライトの地面の法線
		/// </summary>
		/// <param name="normal">地面の法線</param>
		void SetHemiLightGroundNormal(Vector3 normal)
		{
			m_sceneLight.SetHemiLightGroundNormal(normal);
		}
		/// <summary>
		/// 半球ライトを使用する
		/// </summary>
		void UseHemiLight()
		{
			m_sceneLight.UseHemiLight();
		}
		/// <summary>
		/// 半球ライトを使用しない
		/// </summary>
		void UnUseHemiLight()
		{
			m_sceneLight.UnUseHemiLight();
		}

	private:
		std::vector<ModelRender*>	m_modelList;				//モデルクラスのリスト
		std::vector<SpriteRender*>	m_spriteList;				//スプライトクラスのリスト
		std::vector<FontRender*>	m_fontList;					//フォントクラスのリスト

		SceneLight					m_sceneLight;				//シーンライト
	};
}