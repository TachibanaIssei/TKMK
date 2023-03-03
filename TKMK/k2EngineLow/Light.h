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
		Vector3 ptColor;		//カラー
		float	ptRange;		//影響範囲
	};

	struct Light
	{
		DirectionalLight directionalLight;	//ディレクションライトの配列
		float pad0;
		Vector3 cameraEyePos;	//カメラ座標
		float pad1;
		Vector3 ambientLight;	//環境光
	};

	class SceneLight {
	public:
		void Init();
		Light& GetSceneLight()
		{
			return m_light;
		}
		void SetDirectionLight(int lightNo, Vector3 direction, Vector3 color)
		{
			m_light.directionalLight.direction = direction;
			m_light.directionalLight.color = color;
		}
		void SetAmbient(Vector3 ambient)
		{
			m_light.ambientLight = ambient;
		}
	private:
		Light m_light;
	};
}