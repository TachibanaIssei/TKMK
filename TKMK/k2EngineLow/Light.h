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
		Vector3 ptPosition;		//位置
		int isUse = false;		//使用中フラグ
		Vector3 ptColor;		//カラー
		float pad1;
		Vector3	ptRange;		//xに影響範囲,yに影響率に累乗するパラメータ
	};

	/// <summary>
	/// ライト構造体
	/// </summary>
	struct Light
	{
		DirectionalLight directionalLight;	//ディレクションライトの配列
		float pad0;
		PointLight pointLight;
		float pad1;
		Vector3 cameraEyePos;	//カメラ座標
		float pad2;
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
			m_light.pointLight.ptPosition = pos;
			m_light.pointLight.ptColor = color;
			m_light.pointLight.ptRange = range;
			m_light.pointLight.isUse = true;
		}
		/// <summary>
		/// ポイントライトの座標を設定する
		/// </summary>
		/// <param name="pos"></param>
		void SetPointLightPosition(Vector3 pos)
		{
			m_light.pointLight.ptPosition = pos;
		}
	private:
		Light m_light;
	};
}