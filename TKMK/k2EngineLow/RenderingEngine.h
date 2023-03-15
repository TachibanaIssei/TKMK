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

	////////////////////////////////////////////////////////////////////////////////
	///ディレクションライトの関数
	////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// ディレクションライトを設定
		/// </summary>
		/// <param name="lightNo">ライト番号</param>
		/// <param name="direction">ライト方向</param>
		/// <param name="color">ライト色</param>
		void SetDirectionLight(int lightNo, Vector3 direction, Vector4 color)
		{
			m_sceneLight.SetDirectionLight(lightNo, direction, color);
		}
		/// <summary>
		/// ディレクションライトの光の方向を設定する
		/// </summary>
		/// <param name="direction">方向</param>
		void SetDirLightDirection(Vector3 direction)
		{
			m_sceneLight.SetDirLightDirection(direction);
		}
		/// <summary>
		/// ディレクションライトの光の色を設定する
		/// </summary>
		/// <param name="color">色</param>
		void SetDirLightColor(Vector4 color)
		{
			m_sceneLight.SetDirLightColor(color);
		}

		/// <summary>
		/// ディレクションライトの光の方向を取得する
		/// </summary>
		/// <returns>光の方向</returns>
		const Vector3& GetDirLigDirection() const
		{
			return m_sceneLight.GetDirLigDirection();
		}
		/// <summary>
		/// ディレクションライトの光の色を取得する
		/// </summary>
		/// <returns>色</returns>
		const Vector4& GetDirLigColor() const
		{
			return m_sceneLight.GetDirLigColor();
		}

	////////////////////////////////////////////////////////////////////////////////
	///環境光の関数
	////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// 環境光を設定
		/// </summary>
		/// <param name="ambient">環境光</param>
		void SetAmbient(Vector4 ambient)
		{
			m_sceneLight.SetAmbient(ambient);
		}

	////////////////////////////////////////////////////////////////////////////////////////
	///カメラの位置の関数
	////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// カメラの位置を設定する
		/// </summary>
		/// <param name="eyePos"></param>
		void SetEyePos(Vector3 eyePos)
		{
			m_sceneLight.SetEyePos(eyePos);
		}

	////////////////////////////////////////////////////////////////////////////////
	///ポイントライトの関数
	////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// ポイントライトを設定する
		/// </summary>
		/// <param name="pos">ライトの位置</param>
		/// <param name="color">ライトの色</param>
		/// <param name="range">xにライトの影響範囲,yに影響範囲に累乗するパラメータ</param>
		void SetPointLight(Vector3 pos, Vector4 color, Vector3 range)
		{
			m_sceneLight.SetPointLight(pos, color, range);
		}
		/// <summary>
		/// ポイントライトの座標を設定する
		/// </summary>
		/// <param name="pos"></param>
		void SetPointLightPosition(Vector3 pos)
		{
			m_sceneLight.SetPointLightPosition(pos);
		}
		/// <summary>
		/// ポイントライトの色を設定する
		/// </summary>
		/// <param name="color">色</param>
		void SetPointLightColor(Vector4 color)
		{
			m_sceneLight.SetPointLightColor(color);
		}
		/// <summary>
		/// 影響範囲と累乗するパラメータを設定
		/// </summary>
		/// <param name="attn">Xに影響範囲,Yに累乗するパラメータ</param>
		void SetPointLightAttn(Vector3 attn)
		{
			m_sceneLight.SetPointLightAttn(attn);
		}
		/// <summary>
		/// ポイントライトを使用する
		/// </summary>
		void UsePointLight()
		{
			m_sceneLight.UsePointLight();
		}
		/// <summary>
		/// ポイントライトを使用しない
		/// </summary>
		void UnUsePointLight()
		{
			m_sceneLight.UnUsePointLight();
		}

		/// <summary>
		/// ポイントライトの位置を取得する
		/// </summary>
		/// <returns>座標</returns>
		const Vector3& GetPointLightPosition() const
		{
			return m_sceneLight.GetPointLightPosition();
		}
		/// <summary>
		/// ポイントライトの光の色を取得
		/// </summary>
		/// <returns>色</returns>
		const Vector4& GetPointLightColor() const
		{
			return m_sceneLight.GetPointLightColor();
		}
		/// <summary>
		/// スポットライトの影響範囲と累乗するパラメータを取得
		/// </summary>
		/// <returns>Xに影響範囲,Yに累乗するパラメータ</returns>
		const Vector3& GetPointLightAttn() const
		{
			return m_sceneLight.GetPointLightAttn();
		}
		/// <summary>
		/// ポイントライトは使用中?
		/// </summary>
		/// <returns>使用中ならtrue</returns>
		const int PointLightIsUse() const
		{
			return m_sceneLight.PointLightIsUse();
		}

	////////////////////////////////////////////////////////////////////////////////
	///スポットライトの関数
	////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// スポットライトを設定する
		/// </summary>
		/// <param name="pos">位置</param>
		/// <param name="color">色</param>
		/// <param name="range">xに影響範囲,yに影響範囲に累乗するパラメータ</param>
		/// <param name="direction">照射方向</param>
		/// <param name="angle">xは照射角度,ｙは影響に累乗するパラメータ</param>
		void SetSpotLight(Vector3 pos, Vector4 color, Vector3 attn, Vector3 direction, Vector3 angle)
		{
			m_sceneLight.SetSpotLight(pos, color, attn, direction, angle);
		}
		/// <summary>
		/// スポットライトの位置を設定する
		/// </summary>
		/// <param name="pos">座標</param>
		void SetSpotLightPosition(Vector3 pos)
		{
			m_sceneLight.SetSpotLightPosition(pos);
		}
		/// <summary>
		/// スポットライトのライト色の設定
		/// </summary>
		/// <param name="color">色</param>
		void SetSpotLightColor(Vector4 color)
		{
			m_sceneLight.SetSpotLightColor(color);
		}
		/// <summary>
		/// 影響範囲と累乗するパラメータを設定
		/// </summary>
		/// <param name="attn">Xに影響範囲,Yに累乗するパラメータ</param>
		void SetSpotLightAttn(Vector3 attn)
		{
			m_sceneLight.SetSpotLightAttn(attn);
		}
		/// <summary>
		/// スポットライトのライトの方向を設定
		/// </summary>
		/// <param name="direction">方向</param>
		void SetSpotLightDirection(Vector3 direction)
		{
			m_sceneLight.SetSpotLightDirection(direction);
		}
		/// <summary>
		/// スポットライトのライトの角度を設定
		/// </summary>
		/// <param name="angle">角度</param>
		void SetSpotLightAngle(Vector3 angle)
		{
			m_sceneLight.SetSpotLightAngle(angle);
		}
		/// <summary>
		/// スポットライトを使用する
		/// </summary>
		void UseSpotLight()
		{
			m_sceneLight.UseSpotLight();
		}
		/// <summary>
		/// スポットライトを使用しない
		/// </summary>
		void UnUseSpotLight()
		{
			m_sceneLight.UnUseSpotLight();
		}

		/// <summary>
		/// スポットライトの位置を取得
		/// </summary>
		/// <returns>座標</returns>
		const Vector3& GetSpotLightPosition() const
		{
			return m_sceneLight.GetSpotLightPosition();
		}
		/// <summary>
		/// スポットライトの光の色を取得
		/// </summary>
		/// <returns>色</returns>
		const Vector4& GetSpotLightColor() const
		{
			return m_sceneLight.GetSpotLightColor();
		}
		/// <summary>
		/// スポットライトの影響範囲と累乗するパラメータを取得
		/// </summary>
		/// <returns></returns>
		const Vector3& GetSpotLightAttn()const
		{
			return m_sceneLight.GetSpotLightAttn();
		}
		/// <summary>
		/// スポットライトの光の方向を取得する
		/// </summary>
		/// <returns>方向</returns>
		const Vector3& GetSpotLightDirection() const
		{
			return m_sceneLight.GetSpotLightDirection();
		}
		/// <summary>
		/// スポットライトの角度を取得する
		/// </summary>
		/// <returns>角度</returns>
		const Vector3& GetSpotLightAngle() const
		{
			return m_sceneLight.GetSpotLightAngle();
		}
		/// <summary>
		/// スポットライトは使用中？
		/// </summary>
		/// <returns>使用中の場合true</returns>
		const int SpotLightIsUse() const
		{
			return m_sceneLight.SpotLightIsUse();
		}

	////////////////////////////////////////////////////////////////////////////////
	///半球ライトの関数
	////////////////////////////////////////////////////////////////////////////////
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

		/// <summary>
		/// 半球ライトの地面色を取得する
		/// </summary>
		/// <returns>地面色</returns>
		const Vector3& GetHemiLightGroundColor() const
		{
			return m_sceneLight.GetHemiLightGroundColor();
		}
		/// <summary>
		/// 半球ライトの天球色を取得する
		/// </summary>
		/// <returns></returns>
		const Vector3& GetHemiLightSkyColor() const
		{
			return m_sceneLight.GetHemiLightSkyColor();
		}
		/// <summary>
		/// 半球ライトの地面の法線を取得する
		/// </summary>
		/// <returns></returns>
		const Vector3& GetHemiLightGroundNormal() const
		{
			return m_sceneLight.GetHemiLightGroundNormal();
		}
		/// <summary>
		/// 半球ライトは使用中?
		/// </summary>
		/// <returns>使用中のときtrue</returns>
		const int HemiLightIsUse() const
		{
			return m_sceneLight.HemiLightIsUse();
		}

	private:
		std::vector<ModelRender*>	m_modelList;				//モデルクラスのリスト
		std::vector<SpriteRender*>	m_spriteList;				//スプライトクラスのリスト
		std::vector<FontRender*>	m_fontList;					//フォントクラスのリスト

		SceneLight					m_sceneLight;				//シーンライト
	};
}