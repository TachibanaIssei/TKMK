#pragma once
namespace nsK2EngineLow {
	/// <summary>
	/// フォントレンダー
	/// </summary>
	class FontRender
	{
	public:
		static const int MAX_TEXT_SIZE = 256;
		~FontRender()
		{

		}

		/// <summary>
		/// 表示する文字を設定する
		/// </summary>
		/// <param name="text">表示する文字</param>
		void SetText(const wchar_t* text)
		{
			swprintf_s(m_text, text);
		}
		/// <summary>
		/// 表示する文字を取得する
		/// </summary>
		/// <returns>表示する文字</returns>
		const wchar_t* GetText() const
		{
			return m_text;
		}

		/// <summary>
		/// 座標を設定する(Zは0.0で)
		/// </summary>
		/// <param name="position">座標</param>
		void SetPosition(const Vector3& position)
		{
			m_position = position;
		}
		/// <summary>
		/// 座標を設定する(Zは0.0で)
		/// </summary>
		/// <param name="x">X軸</param>
		/// <param name="y">Y軸</param>
		/// <param name="z">Z軸</param>
		void SetPosition(float x, float y, float z = 0.0f)
		{
			SetPosition({ x,y,z });
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
		/// 大きさを設定する
		/// </summary>
		/// <param name="scale">大きさ</param>
		void SetScale(const float scale)
		{
			m_scale = scale;
		}
		/// <summary>
		/// 大きさを取得する
		/// </summary>
		/// <returns>大きさ</returns>
		const float GetScale() const
		{
			return m_scale;
		}

		/// <summary>
		/// 回転を設定する
		/// </summary>
		/// <param name="rotation">回転</param>
		void SetRotation(const float rotation)
		{
			m_rotation = Math::DegToRad(rotation);
		}
		/// <summary>
		/// 回転を取得する
		/// </summary>
		/// <returns>回転</returns>
		const float GetRotation() const
		{
			return m_rotation;
		}

		/// <summary>
		/// ピボットを設定する。
		/// x = 0.5, y = 0.5で画像の中心。
		/// x = 0.0, y = 0.0で画像の左下。
		/// x = 1.0, y = 1.0で画像の右上。
		/// </summary>
		/// <param name="pivot">ピボット</param>
		void SetPivot(const Vector2& pivot)
		{
			m_pivot = pivot;
		}
		/// <summary>
		/// ピボットを設定する。
		/// x = 0.5, y = 0.5で画像の中心。
		/// x = 0.0, y = 0.0で画像の左下。
		/// x = 1.0, y = 1.0で画像の右上。
		/// </summary>
		/// <param name="x">X軸のピボット</param>
		/// <param name="y">Y軸のピボット</param>
		void SetPivot(float x, float y)
		{
			SetPivot({ x,y });
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
		/// 色を設定する
		/// </summary>
		/// <param name="color">色</param>
		void SetColor(const Vector4 color)
		{
			m_color = color;
		}
		/// <summary>
		/// 色を設定する
		/// </summary>
		/// <param name="r">赤</param>
		/// <param name="g">緑</param>
		/// <param name="b">青</param>
		/// <param name="a">アルファ</param>
		void SetColor(float r, float g, float b, float a)
		{
			SetColor({ r,g,b,a });
		}
		/// <summary>
		/// 色を取得する
		/// </summary>
		/// <returns>色</returns>
		const Vector4 GetColor()const
		{
			return m_color;
		}

		/// <summary>
		/// 影のパラメーターを設定する。文字に輪郭をつけることができます。
		/// </summary>
		/// <param name="isDrawShadow">影を描画する？</param>
		/// <param name="shadowOffset">影を描画するときのピクセルのオフセット量</param>
		/// <param name="shadowColor">影の色</param>
		void SetShadowParam(bool isDrawShadow, float shadowOffset, const Vector4& shadowColor)
		{
			m_font.SetShadowParam(isDrawShadow, shadowOffset, shadowColor);
		}

		/// <summary>
		/// 描画処理
		/// </summary>
		/// <param name="rc">レンダーコンテキスト</param>
		void Draw(RenderContext& rc);

	private:
		Vector3		m_position = Vector3::Zero;			//座標
		float		m_scale = 1.0f;						//大きさ
		Vector4		m_color = g_vec4White;				//文字の色。デフォルトは白
		float		m_rotation = 0.0f;					//回転
		Vector2		m_pivot = Sprite::DEFAULT_PIVOT;	//ピボット
		wchar_t		m_text[MAX_TEXT_SIZE];				//文字
		Font		m_font;								//フォント
	};
}