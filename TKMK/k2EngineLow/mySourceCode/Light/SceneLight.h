#pragma once
namespace nsK2EngineLow {
	/// <summary>
	/// ディレクションライト構造体
	/// </summary>
	struct DirectionalLight
	{
		Vector3 direction;	//ライトの方向
		int castShadow;		//影をキャストする？
		Vector4 color;		//ライトのカラー
	};

	struct Light
	{
		std::array<DirectionalLight, MAX_DIRECTIONAL_LIGHT> directionalLight;	//ディレクショナルライトの配列
		Matrix mViewProjInv;	//ビュープロジェクション行列の逆行列
		Vector3 eyePos;			//カメラの位置
		float pad0;				//パディング0
		Vector3 ambientLight;	//環境光
		float pad1;				//パディング1
	};

	class SceneLight {
	public:
		/// <summary>
		/// 初期化
		/// </summary>
		void Init();
		/// <summary>
		/// シーンライトを取得
		/// </summary>
		/// <returns></returns>
		Light& GetSceneLight()
		{
			return m_light;
		}
		/// <summary>
		/// ディレクションライトを設定する
		/// </summary>
		/// <param name="lightNo">ライト番号</param>
		/// <param name="direction">ライトの方向</param>
		/// <param name="color">ライトの色</param>
		void SetDirectionLight(const int lightNo, const Vector3 direction, const Vector4 color)
		{
			m_light.directionalLight[lightNo].direction = direction;
			m_light.directionalLight[lightNo].color = color;
		}
		/// <summary>
		/// ディレクションライトのキャストシャドウフラグを設定
		/// </summary>
		/// <param name="lightNo">ライト番号</param>
		/// <param name="flag"></param>
		void SetDirectionLightCastShadow(const int lightNo, const bool flag)
		{
			m_light.directionalLight[lightNo].castShadow = flag;
		}
		/// <summary>
		/// キャストシャドウフラグを取得
		/// </summary>
		/// <param name="lightNo">ライト番号</param>
		/// <returns>trueだったら影を落とす</returns>
		const bool IsCastShadow(const int lightNo) const
		{
			return m_light.directionalLight[lightNo].castShadow;
		}
		/// <summary>
		/// 環境光を設定する
		/// </summary>
		/// <param name="ambient"></param>
		void SetAmbient(const Vector3 ambient)
		{
			m_light.ambientLight = ambient;
		}

	private:
		Light m_light;
	};
}