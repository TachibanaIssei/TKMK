#pragma once

namespace nsK2EngineLow {
	/// <summary>
	/// スプライトレンダー
	/// </summary>
	class SpriteRender
	{
	public:
		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="filePath">ファイルパス</param>
		/// <param name="w">画像の横幅</param>
		/// <param name="h">画像の縦幅</param>
		/// <param name="alphaBlendMode">デフォルトは半透明合成</param>
		void Init(const char* filePath, const float w, const float h, AlphaBlendMode alphaBlendMode = AlphaBlendMode_Trans);

		/// <summary>
		/// 座標を設定する(Zは0.0fにする)
		/// </summary>
		/// <param name="position">座標</param>
		void SetPosition(const Vector3& position)
		{
			m_position = position;
		}
		/// <summary>
		/// 座標を設定する(Zは0.0fにする)
		/// </summary>
		/// <param name="x">X座標</param>
		/// <param name="y">Y座標</param>
		/// <param name="z">Z座標</param>
		void SetPosition(float x, float y, float z = 0.0f)
		{
			SetPosition({ x, y, z });
		}
		/// <summary>
		/// 座標を取得する
		/// </summary>
		/// <returns>座標</returns>
		const Vector3& GetPosition() const
		{
			return m_position;
		}
		
		/// <summary>
		/// 大きさを設定する(Zは1.0fにする)
		/// </summary>
		/// <param name="scale"></param>
		void SetScale(const Vector3& scale)
		{
			m_scale = scale;
		}
		/// <summary>
		/// 大きさを設定する(Zは1.0fにする)
		/// </summary>
		/// <param name="x">X軸</param>
		/// <param name="y">Y軸</param>
		/// <param name="z">Z軸</param>
		void SetScale(float x, float y, float z = 1.0f)
		{
			SetScale({ x, y, z });
		}
		/// <summary>
		/// 大きさを取得する
		/// </summary>
		/// <returns>大きさ</returns>
		const Vector3& GetScale() const
		{
			return m_scale;
		}

		/// <summary>
		/// 回転を設定
		/// </summary>
		/// <param name="rotation">回転</param>
		void SetRotation(const Quaternion& rotation)
		{
			m_rotation = rotation;
		}
		/// <summary>
		/// 回転を取得する
		/// </summary>
		/// <returns>回転</returns>
		const Quaternion& GetRotation() const
		{
			return m_rotation;
		}
		
		/// <summary>
		/// ピボットを設定する
		/// </summary>
		/// <param name="pivot"></param>
		void SetPivot(const Vector2& pivot)
		{
			m_pivot = pivot;
		}
		/// <summary>
		/// ピボットを取得する
		/// </summary>
		/// <returns>ピボット</returns>
		const Vector2& GetPivot() const
		{
			return m_pivot;
		}

		/// <summary>
		/// 乗算カラーを設定する
		/// </summary>
		/// <param name="mulColor">乗算カラー</param>
		void SetMulColor(const Vector4& mulColor)
		{
			m_sprite.SetMulColor(mulColor);
		}
		/// <summary>
		/// 乗算カラーを取得する
		/// </summary>
		/// <returns>乗算カラー</returns>
		const Vector4& GetMulColor() const
		{
			return m_sprite.GetMulColor();
		}

		/// <summary>
		/// 更新処理
		/// </summary>
		void Update()
		{
			m_sprite.Update(
				m_position,
				m_rotation,
				m_scale,
				m_pivot
			);
		}

		/// <summary>
		/// 描画処理
		/// </summary>
		/// <param name="rc">レンダーコンテキスト</param>
		void Draw(RenderContext& rc, bool drawTiming = false);

		/// <summary>
		/// RenderingEngineで描画するときに呼び出す
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		void OnRenderSprite(RenderContext& rc)
		{
			m_sprite.Draw(rc);
		}
		
		bool IsInit()
		{
			if (m_isInit)	return true;
			else			return false;
		}

	private:
		Sprite m_sprite;								//Spriteクラス

		Vector2 m_pivot = Sprite::DEFAULT_PIVOT;		//ピボット
		Vector3 m_position = Vector3::Zero;				//座標
		Vector3	m_scale = Vector3::One;					//大きさ
		Quaternion m_rotation = Quaternion::Identity;	//回転
		bool m_isInit = false;
	};
}

