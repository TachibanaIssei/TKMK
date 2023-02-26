#pragma once
namespace nsK2EngineLow {
	/// <summary>
	/// �t�H���g�����_�[
	/// </summary>
	class FontRender
	{
	public:
		static const int MAX_TEXT_SIZE = 256;
		~FontRender()
		{

		}

		/// <summary>
		/// �\�����镶����ݒ肷��
		/// </summary>
		/// <param name="text">�\�����镶��</param>
		void SetText(const wchar_t* text)
		{
			swprintf_s(m_text, text);
		}
		/// <summary>
		/// �\�����镶�����擾����
		/// </summary>
		/// <returns>�\�����镶��</returns>
		const wchar_t* GetText() const
		{
			return m_text;
		}

		/// <summary>
		/// ���W��ݒ肷��(Z��0.0��)
		/// </summary>
		/// <param name="position">���W</param>
		void SetPosition(const Vector3& position)
		{
			m_position = position;
		}
		/// <summary>
		/// ���W��ݒ肷��(Z��0.0��)
		/// </summary>
		/// <param name="x">X��</param>
		/// <param name="y">Y��</param>
		/// <param name="z">Z��</param>
		void SetPosition(float x, float y, float z = 0.0f)
		{
			SetPosition({ x,y,z });
		}
		/// <summary>
		/// ���W���擾����
		/// </summary>
		/// <returns>���W</returns>
		const Vector3& GetPosition() const
		{
			return m_position;
		}

		/// <summary>
		/// �傫����ݒ肷��
		/// </summary>
		/// <param name="scale">�傫��</param>
		void SetScale(const float scale)
		{
			m_scale = scale;
		}
		/// <summary>
		/// �傫�����擾����
		/// </summary>
		/// <returns>�傫��</returns>
		const float GetScale() const
		{
			return m_scale;
		}

		/// <summary>
		/// ��]��ݒ肷��
		/// </summary>
		/// <param name="rotation">��]</param>
		void SetRotation(const float rotation)
		{
			m_rotation = Math::DegToRad(rotation);
		}
		/// <summary>
		/// ��]���擾����
		/// </summary>
		/// <returns>��]</returns>
		const float GetRotation() const
		{
			return m_rotation;
		}

		/// <summary>
		/// �s�{�b�g��ݒ肷��B
		/// x = 0.5, y = 0.5�ŉ摜�̒��S�B
		/// x = 0.0, y = 0.0�ŉ摜�̍����B
		/// x = 1.0, y = 1.0�ŉ摜�̉E��B
		/// </summary>
		/// <param name="pivot">�s�{�b�g</param>
		void SetPivot(const Vector2& pivot)
		{
			m_pivot = pivot;
		}
		/// <summary>
		/// �s�{�b�g��ݒ肷��B
		/// x = 0.5, y = 0.5�ŉ摜�̒��S�B
		/// x = 0.0, y = 0.0�ŉ摜�̍����B
		/// x = 1.0, y = 1.0�ŉ摜�̉E��B
		/// </summary>
		/// <param name="x">X���̃s�{�b�g</param>
		/// <param name="y">Y���̃s�{�b�g</param>
		void SetPivot(float x, float y)
		{
			SetPivot({ x,y });
		}
		/// <summary>
		/// �s�{�b�g���擾����
		/// </summary>
		/// <returns>�s�{�b�g</returns>
		const Vector2& GetPivot() const
		{
			return m_pivot;
		}

		/// <summary>
		/// �F��ݒ肷��
		/// </summary>
		/// <param name="color">�F</param>
		void SetColor(const Vector4 color)
		{
			m_color = color;
		}
		/// <summary>
		/// �F��ݒ肷��
		/// </summary>
		/// <param name="r">��</param>
		/// <param name="g">��</param>
		/// <param name="b">��</param>
		/// <param name="a">�A���t�@</param>
		void SetColor(float r, float g, float b, float a)
		{
			SetColor({ r,g,b,a });
		}
		/// <summary>
		/// �F���擾����
		/// </summary>
		/// <returns>�F</returns>
		const Vector4 GetColor()const
		{
			return m_color;
		}

		/// <summary>
		/// �e�̃p�����[�^�[��ݒ肷��B�����ɗ֊s�����邱�Ƃ��ł��܂��B
		/// </summary>
		/// <param name="isDrawShadow">�e��`�悷��H</param>
		/// <param name="shadowOffset">�e��`�悷��Ƃ��̃s�N�Z���̃I�t�Z�b�g��</param>
		/// <param name="shadowColor">�e�̐F</param>
		void SetShadowParam(bool isDrawShadow, float shadowOffset, const Vector4& shadowColor)
		{
			m_font.SetShadowParam(isDrawShadow, shadowOffset, shadowColor);
		}

		/// <summary>
		/// �`�揈��
		/// </summary>
		/// <param name="rc">�����_�[�R���e�L�X�g</param>
		void Draw(RenderContext& rc);

	private:
		Vector3		m_position = Vector3::Zero;			//���W
		float		m_scale = 1.0f;						//�傫��
		Vector4		m_color = g_vec4White;				//�����̐F�B�f�t�H���g�͔�
		float		m_rotation = 0.0f;					//��]
		Vector2		m_pivot = Sprite::DEFAULT_PIVOT;	//�s�{�b�g
		wchar_t		m_text[MAX_TEXT_SIZE];				//����
		Font		m_font;								//�t�H���g
	};
}