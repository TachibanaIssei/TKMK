#pragma once
namespace nsK2EngineLow {
	const int MAX_DIRECTIONAL_LIGHT = 4;	//ディレクションライトの最大数

	/// <summary>
	/// ディレクションライト構造体
	/// </summary>
	struct DirectionalLight
	{
		Vector3 direction;	//ライトの方向
		float pad0;
		Vector3 color;		//ライトのカラー
	};

	/// <summary>
	/// ポイントライト構造体
	/// </summary>
	struct PointLight
	{
		Vector3 position;		//位置
		int isUse = false;		//使用中フラグ
		Vector3 color;			//カラー
		float pad1;
		Vector3	range;			//xに影響範囲,yに影響率に累乗するパラメータ
	};

	/// <summary>
	/// スポットライト構造体
	/// </summary>
	struct SpotLight 
	{
		Vector3 position;	//位置
		float pad0;
		Vector3 angle;		//xに照射角度,yに影響に累乗するパラメータ
		float pad1;
		Vector3 color;		//色
		int isUse = false;	//使用中フラグ
		Vector3 range;		//xに影響範囲,yに影響率に累乗するパラメータ
		float pad2;
		Vector3 direction;	//照射方向
	};

	/// <summary>
	/// 半球ライト
	/// </summary>
	struct HemisphereLight
	{
		Vector3 groundColor;	//地面色
		int		isUse = false;	//使用フラグ
		Vector3 skyColor;		//天球色
		float pad1;
		Vector3 groundNormal;	//地面法線
	};

	/// <summary>
	/// ライト構造体
	/// </summary>
	struct Light
	{
		DirectionalLight directionalLight;	//ディレクションライトの配列
		float pad0;
		PointLight pointLight;	//ポイントライト
		float pad1;
		SpotLight spotLight;	//スポットライト
		float pad2;
		HemisphereLight hemisphereLight;
		float pad3;
		Vector3 cameraEyePos;	//カメラ座標
		float pad4;
		Vector3 ambientLight;	//環境光
	};

	class SceneLight {
	public:
		void Init();
		Light& GetSceneLight()
		{
			return m_light;
		}

		////////////////////////////////////////////////////////////////////////////////////////
		///ディレクションライトの関数
		////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// ディレクションを設定する
		/// </summary>
		/// <param name="lightNo">ライト番号</param>
		/// <param name="direction">ライト方向</param>
		/// <param name="color">ライトの色</param>
		void SetDirectionLight(int lightNo, Vector3 direction, Vector3 color)
		{
			m_light.directionalLight.direction = direction;
			m_light.directionalLight.color = color;
		}
		/// <summary>
		/// ディレクションライトの光の方向を取得する
		/// </summary>
		/// <returns>光の方向</returns>
		Vector3& GetDirLigDirection()
		{
			return m_light.directionalLight.direction;
		}

		////////////////////////////////////////////////////////////////////////////////////////
		///環境光の関数
		////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// 環境光を設定する
		/// </summary>
		/// <param name="ambient">環境光の強さ</param>
		void SetAmbient(Vector3 ambient)
		{
			m_light.ambientLight = ambient;
		}

		////////////////////////////////////////////////////////////////////////////////////////
		///ポイントライトの関数
		////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// ポイントライトを設定する
		/// </summary>
		/// <param name="pos">ライトの位置</param>
		/// <param name="color">ライトの色</param>
		/// <param name="range">xにライトの影響範囲,yに影響範囲に累乗するパラメータ</param>
		void SetPointLight(Vector3 pos, Vector3 color,Vector3 range)
		{
			m_light.pointLight.position = pos;
			m_light.pointLight.color = color;
			m_light.pointLight.range = range;
			m_light.pointLight.isUse = true;
		}
		/// <summary>
		/// ポイントライトの座標を設定する
		/// </summary>
		/// <param name="pos"></param>
		void SetPointLightPosition(Vector3 pos)
		{
			m_light.pointLight.position = pos;
		}

		////////////////////////////////////////////////////////////////////////////////////////
		///スポットライトの関数
		////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// スポットライトを設定する
		/// </summary>
		/// <param name="pos"></param>
		/// <param name="color"></param>
		/// <param name="range"></param>
		/// <param name="direction"></param>
		/// <param name="angle"></param>
		void SetSpotLight(Vector3 pos, Vector3 color, Vector3 range, Vector3 direction, Vector3 angle)
		{
			m_light.spotLight.position = pos;
			m_light.spotLight.color = color;
			m_light.spotLight.range = range;
			m_light.spotLight.direction = direction;
			m_light.spotLight.angle = angle;
			m_light.spotLight.isUse = true;
		}
		/// <summary>
		/// スポットライトの位置を設定
		/// </summary>
		/// <param name="pos">座標</param>
		void SetSpotLightPosition(Vector3 pos)
		{
			m_light.spotLight.position = pos;
		}
		/// <summary>
		/// スポットライトの光の方向を取得
		/// </summary>
		/// <returns>光の方向</returns>
		Vector3& GetSpotLightDirection()
		{
			return m_light.spotLight.direction;
		}

		////////////////////////////////////////////////////////////////////////////////////////
		///半球ライトの関数
		////////////////////////////////////////////////////////////////////////////////////////
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
			m_light.hemisphereLight.groundColor = groundColor;
		}
		/// <summary>
		/// 半球ライトの天球色を設定
		/// </summary>
		/// <param name="skyColor">天球色</param>
		void SetHemiLightSkyColor(Vector3 skyColor)
		{
			m_light.hemisphereLight.skyColor = skyColor;
		}
		/// <summary>
		/// 半球ライトの地面の法線
		/// </summary>
		/// <param name="normal">地面の法線</param>
		void SetHemiLightGroundNormal(Vector3 normal)
		{
			m_light.hemisphereLight.groundNormal = normal;
		}
		/// <summary>
		/// 半球ライトを使用する
		/// </summary>
		void UseHemiLight()
		{
			m_light.hemisphereLight.isUse = true;
		}
		/// <summary>
		/// 半球ライトを使用しない
		/// </summary>
		void UnUseHemiLight()
		{
			m_light.hemisphereLight.isUse = false;
		}

	private:
		Light m_light;
	};
}