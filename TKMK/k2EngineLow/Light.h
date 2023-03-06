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
		Vector3 cameraEyePos;	//カメラ座標
		float pad3;
		Vector3 ambientLight;	//環境光
	};

	class SceneLight {
	public:
		void Init();
		Light& GetSceneLight()
		{
			return m_light;
		}
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
		/// 環境光を設定する
		/// </summary>
		/// <param name="ambient">環境光の強さ</param>
		void SetAmbient(Vector3 ambient)
		{
			m_light.ambientLight = ambient;
		}
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
		void SetSpotLightPosition(Vector3 pos)
		{
			m_light.spotLight.position = pos;
		}
		Vector3& GetSpotLightDirection()
		{
			return m_light.spotLight.direction;
		}
	private:
		Light m_light;
	};
}