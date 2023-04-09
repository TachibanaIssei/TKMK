#pragma once
namespace nsK2EngineLow {
	const int MAX_DIRECTIONAL_LIGHT = 4;	//ディレクションライトの最大数
	const int MAX_POINT_LIGHT = 10;			//ポイントライトの最大数
	const int MAX_SPOT_LIGHT = 10;

	/// <summary>
	/// ディレクションライト構造体
	/// </summary>
	struct DirectionalLight
	{
		Vector3 direction;	//ライトの方向
		float	pad0;
		Vector4 color;		//ライトのカラー
	};

	/// <summary>
	/// ポイントライト構造体
	/// </summary>
	struct PointLight
	{
		Vector3 position;		//位置
		int		isUse = false;		//使用中フラグ
		Vector4 color;			//カラー
		Vector3	attn;			//xに影響範囲,yに影響率に累乗するパラメータ
		float	pad1;
	};

	/// <summary>
	/// スポットライト構造体
	/// </summary>
	struct SpotLight 
	{
		Vector3 position;	//位置
		float	pad0;
		Vector3 angle;		//xに照射角度,yに影響に累乗するパラメータ
		float	pad1;
		Vector4 color;		//色
		Vector3 attn;		//xに影響範囲,yに影響率に累乗するパラメータ
		int		isUse = false;	//使用中フラグ
		Vector3 direction;	//照射方向
		float	pad2;
	};

	/// <summary>
	/// 半球ライト
	/// </summary>
	struct HemisphereLight
	{
		Vector3 groundColor;	//地面色
		int		isUse = false;	//使用フラグ
		Vector3 skyColor;		//天球色
		float	pad0;
		Vector3 groundNormal;	//地面法線
	};

	/// <summary>
	/// ライト構造体
	/// </summary>
	struct Light
	{
		DirectionalLight	directionalLight[MAX_DIRECTIONAL_LIGHT];	//ディレクションライトの配列
		PointLight			pointLight[MAX_POINT_LIGHT];				//ポイントライト
		SpotLight			spotLight[MAX_SPOT_LIGHT];					//スポットライト
		HemisphereLight		hemisphereLight;
		float				pad1;
		Vector3				cameraEyePos;								//カメラ座標
		float				pad2;
		Vector4				ambientLight;								//環境光
		Matrix				mLVP;										//ライトビュープロジェクション行列
	};

	class SceneLight {
	public:
		void Init();
		Light& GetSceneLight()
		{
			return m_light;
		}

		/// <summary>
		/// ライトビュープロジェクション行列を設定する
		/// </summary>
		/// <param name="LVP">ライトビュープロジェクション行列</param>
		void SetmLVP(Matrix LVP)
		{
			m_light.mLVP = LVP;
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
		void SetDirectionLight(int lightNo, Vector3 direction, Vector4 color)
		{
			SetDirLightDirection(direction,lightNo);
			SetDirLightColor(color,lightNo);
		}
		/// <summary>
		/// ディレクションライトの光の方向を設定する
		/// </summary>
		/// <param name="direction">方向</param>
		/// <param name="lightNo">ライトの番号</param>
		void SetDirLightDirection(Vector3 direction, int lightNo = 0)
		{
			m_light.directionalLight[lightNo].direction = direction;
		}
		/// <summary>
		/// ディレクションライトの光の色を設定する
		/// </summary>
		/// <param name="color">色</param>
		/// <param name="lightNo">ライトの番号</param>
		void SetDirLightColor(Vector4 color, int lightNo = 0)
		{
			m_light.directionalLight[lightNo].color = color;
		}
		/// <summary>
		/// ディレクションライトの光の方向を取得する
		/// </summary>
		/// <param name="lightNo">ライトの番号</param>
		/// <returns>光の方向</returns>
		const Vector3& GetDirLigDirection(int lightNo = 0) const
		{
			return m_light.directionalLight[lightNo].direction;
		}
		/// <summary>
		/// ディレクションライトの光の色を取得する
		/// </summary>
		/// <param name="lightNo">ライトの番号</param>
		/// <returns>色</returns>
		const Vector4& GetDirLigColor(int lightNo = 0) const
		{
			return m_light.directionalLight[lightNo].color;
		}

	////////////////////////////////////////////////////////////////////////////////////////
	///環境光の関数
	////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// 環境光を設定する
		/// </summary>
		/// <param name="ambient">環境光の強さ</param>
		void SetAmbient(Vector4 ambient)
		{
			m_light.ambientLight = ambient;
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
			m_light.cameraEyePos = eyePos;
		}

	////////////////////////////////////////////////////////////////////////////////////////
	///ポイントライトの関数
	////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// ポイントライトを設定する
		/// </summary>
		/// <param name="lightNo">ライトの番号</param>
		/// <param name="pos">ライトの位置</param>
		/// <param name="color">ライトの色</param>
		/// <param name="range">xにライトの影響範囲,yに影響範囲に累乗するパラメータ</param>
		void SetPointLight(int lightNo, Vector3 pos, Vector4 color,Vector3 attn)
		{
			SetPointLightPosition(pos,lightNo);
			SetPointLightColor(color, lightNo);
			SetPointLightAttn(attn, lightNo);
			UsePointLight(lightNo);
		}
		/// <summary>
		/// ポイントライトの座標を設定する
		/// </summary>
		/// <param name="pos">座標</param>
		/// <param name="lightNo">ライトの番号</param>
		void SetPointLightPosition(Vector3 pos, int lightNo = 0)
		{
			m_light.pointLight[lightNo].position = pos;
		}
		/// <summary>
		/// ポイントライトの色を設定する
		/// </summary>
		/// <param name="color">色</param>
		/// <param name="lightNo">ライトの番号</param>
		void SetPointLightColor(Vector4 color, int lightNo = 0)
		{
			m_light.pointLight[lightNo].color = color;
		}
		/// <summary>
		/// 影響範囲と累乗するパラメータを設定
		/// </summary>
		/// <param name="attn">Xに影響範囲,Yに累乗するパラメータ</param>
		/// <param name="lightNo">ライトの番号</param>
		void SetPointLightAttn(Vector3 attn, int lightNo = 0)
		{
			m_light.pointLight[lightNo].attn = attn;
		}
		/// <summary>
		/// ポイントライトを使用する
		/// </summary>
		/// <param name="lightNo">ライトの番号</param>
		void UsePointLight(int lightNo = 0)
		{
			m_light.pointLight[lightNo].isUse = true;
		}
		/// <summary>
		/// ポイントライトを使用しない
		/// </summary>
		/// <param name="lightNo">ライトの番号</param>
		void UnUsePointLight(int lightNo = 0)
		{
			m_light.pointLight[lightNo].isUse = false;
		}

		/// <summary>
		/// ポイントライトの位置を取得する
		/// </summary>
		/// <param name="lightNo">ライトの番号</param>
		/// <returns>座標</returns>
		const Vector3& GetPointLightPosition(int lightNo = 0) const
		{
			return m_light.pointLight[lightNo].position;
		}
		/// <summary>
		/// ポイントライトの光の色を取得
		/// </summary>
		/// <param name="lightNo">ライトの番号</param>
		/// <returns>色</returns>
		const Vector4& GetPointLightColor(int lightNo = 0) const
		{
			return m_light.pointLight[lightNo].color;
		}
		/// <summary>
		/// スポットライトの影響範囲と累乗するパラメータを取得
		/// </summary>
		/// <param name="lightNo">ライトの番号</param>
		/// <returns>Xに影響範囲,Yに累乗するパラメータ</returns>
		const Vector3& GetPointLightAttn(int lightNo = 0) const
		{
			return m_light.pointLight[lightNo].attn;
		}
		/// <summary>
		/// ポイントライトは使用中?
		/// </summary>
		/// <param name="lightNo">ライトの番号</param>
		/// <returns>使用中ならtrue</returns>
		const int PointLightIsUse(int lightNo = 0) const
		{
			return m_light.pointLight[lightNo].isUse;
		}

	////////////////////////////////////////////////////////////////////////////////////////
	///スポットライトの関数
	////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// スポットライトを設定する
		/// </summary>
		/// <param name="pos">位置</param>
		/// <param name="color">色</param>
		/// <param name="range">Xに影響範囲,Yに累乗するパラメータ</param>
		/// <param name="direction">方向</param>
		/// <param name="angle">角度</param>
		void SetSpotLight(int lightNo, Vector3 pos, Vector4 color, Vector3 attn, Vector3 direction, Vector3 angle)
		{
			SetSpotLightPosition(pos,lightNo);
			SetSpotLightColor(color, lightNo);
			SetSpotLightAttn(attn, lightNo);
			SetSpotLightDirection(direction, lightNo);
			SetSpotLightAngle(angle, lightNo);
			UseSpotLight(lightNo);
		}
		/// <summary>
		/// スポットライトの位置を設定
		/// </summary>
		/// <param name="pos">座標</param>
		void SetSpotLightPosition(Vector3 pos, int lightNo = 0)
		{
			m_light.spotLight[lightNo].position = pos;
		}
		/// <summary>
		/// スポットライトのライト色の設定
		/// </summary>
		/// <param name="color">色</param>
		void SetSpotLightColor(Vector4 color, int lightNo = 0)
		{
			m_light.spotLight[lightNo].color = color;
		}
		/// <summary>
		/// 影響範囲と累乗するパラメータを設定
		/// </summary>
		/// <param name="attn">Xに影響範囲,Yに累乗するパラメータ</param>
		void SetSpotLightAttn(Vector3 attn, int lightNo = 0)
		{
			m_light.spotLight[lightNo].attn = attn;
		}
		/// <summary>
		/// スポットライトのライトの方向を設定
		/// </summary>
		/// <param name="direction">方向</param>
		void SetSpotLightDirection(Vector3 direction, int lightNo = 0)
		{
			m_light.spotLight[lightNo].direction = direction;
		}
		/// <summary>
		/// スポットライトのライトの角度を設定
		/// </summary>
		/// <param name="angle">角度</param>
		void SetSpotLightAngle(Vector3 angle, int lightNo = 0)
		{
			m_light.spotLight[lightNo].angle = angle;
		}
		/// <summary>
		/// スポットライトを使用する
		/// </summary>
		void UseSpotLight(int lightNo = 0)
		{
			m_light.spotLight[lightNo].isUse = true;
		}
		/// <summary>
		/// スポットライトを使用しない
		/// </summary>
		void UnUseSpotLight(int lightNo = 0)
		{
			m_light.spotLight[lightNo].isUse = false;
		}

		/// <summary>
		/// スポットライトの位置を取得
		/// </summary>
		/// <returns>座標</returns>
		const Vector3& GetSpotLightPosition(int lightNo = 0) const
		{
			return m_light.spotLight[lightNo].position;
		}
		/// <summary>
		/// スポットライトの光の色を取得
		/// </summary>
		/// <returns>色</returns>
		const Vector4& GetSpotLightColor(int lightNo = 0) const
		{
			return m_light.spotLight[lightNo].color;
		}
		/// <summary>
		/// スポットライトの影響範囲と累乗するパラメータを取得
		/// </summary>
		/// <returns>Xに影響範囲,Yに累乗するパラメータ</returns>
		const Vector3& GetSpotLightAttn(int lightNo = 0)const
		{
			return m_light.spotLight[lightNo].attn;
		}
		/// <summary>
		/// スポットライトの光の方向を取得
		/// </summary>
		/// <returns>光の方向</returns>
		const Vector3& GetSpotLightDirection(int lightNo = 0) const
		{
			return m_light.spotLight[lightNo].direction;
		}
		/// <summary>
		/// スポットライトの角度を取得する
		/// </summary>
		/// <returns>角度</returns>
		const Vector3& GetSpotLightAngle(int lightNo = 0) const
		{
			return m_light.spotLight[lightNo].angle;
		}
		/// <summary>
		/// スポットライトは使用中？
		/// </summary>
		/// <returns>使用中の場合true</returns>
		const int SpotLightIsUse(int lightNo = 0) const
		{
			return m_light.spotLight[lightNo].isUse;
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

		/// <summary>
		/// 半球ライトの地面色を取得する
		/// </summary>
		/// <returns>地面色</returns>
		const Vector3& GetHemiLightGroundColor() const
		{
			return m_light.hemisphereLight.groundColor;
		}
		/// <summary>
		/// 半球ライトの天球色を取得する
		/// </summary>
		/// <returns></returns>
		const Vector3& GetHemiLightSkyColor() const
		{
			return m_light.hemisphereLight.skyColor;
		}
		/// <summary>
		/// 半球ライトの地面の法線を取得する
		/// </summary>
		/// <returns></returns>
		const Vector3& GetHemiLightGroundNormal() const
		{
			return m_light.hemisphereLight.groundNormal;
		}
		/// <summary>
		/// 半球ライトは使用中?
		/// </summary>
		/// <returns>使用中のときtrue</returns>
		const int HemiLightIsUse() const
		{
			return m_light.hemisphereLight.isUse;
		}

	private:
		void InitDirectionLight();
		void InitPointLight();
		void InitSpotLight();

	private:
		Light m_light;
	};
}