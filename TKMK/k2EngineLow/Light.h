#pragma once
namespace nsK2EngineLow {
	/// <summary>
	/// ディレクションライト構造体
	/// </summary>
	struct DirectionalLight
	{
		Vector3 lightDirection;	//ライトの方向
		Vector3 ligColor;		//ライトのカラー
		Vector3 CameraEyePos;	//カメラ座標
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
		
		void SetPointLight(Vector3 pos, Vector3 color = { 0.5f,0.5f,0.5 }, float range = 100.0f)
		{
			//ライトの位置
			m_pointLight.ptPosition = pos;
			//ライトの影響範囲
			m_pointLight.ptRange = range;
			//ライトの色
			m_pointLight.ptColor = color;
			
		}

	private:
		DirectionalLight			m_directionLight;		//ディレクションライト
		PointLight					m_pointLight;			//ポイントライト
	};
}