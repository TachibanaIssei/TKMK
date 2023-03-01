#pragma once
namespace nsK2EngineLow {
	struct DirectionalLight
	{
		Vector3 lightDirection;	//ライトの方向
		float pad0;
		Vector3 ligColor;		//ライトのカラー
		float pad1;
		Vector3 CameraEyePos;	//カメラ座標
	};

	class Light
	{
	public:
		void Init();

		/// <summary>
		/// ディレクションライトを取得する
		/// </summary>
		/// <returns>ディレクションライト</returns>
		DirectionalLight& GetDirectionalLight()
		{
			return m_directionLight;
		}

		/// <summary>
		/// ディレクションライトを設定する
		/// </summary>
		/// <param name="dir">ライトの方向</param>
		/// <param name="color">ライトの色</param>
		void SetDirectionLight(Vector3 dir, Vector3 color = { 0.5f,0.5f,0.5f })
		{
			//ライトの方向
			m_directionLight.lightDirection = dir;
			//ライトの色
			m_directionLight.ligColor = color;
			//視点の位置を設定
			m_directionLight.CameraEyePos = g_camera3D->GetPosition();
		}

	private:
		DirectionalLight			m_directionLight;										//ディレクションライト
	};
}