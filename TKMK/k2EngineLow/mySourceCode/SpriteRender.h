#pragma once

namespace nsK2EngineLow {
	struct SpriteCB
	{
		bool grayScale = false;	//�O���[�X�P�[�������邩�ǂ���
	};

	/// <summary>
	/// �X�v���C�g�����_�[
	/// </summary>
	class SpriteRender : public Noncopyable
	{
	public:
		/// <summary>
		/// ������
		/// </summary>
		/// <param name="filePath">�t�@�C���p�X</param>
		/// <param name="w">�摜�̉���</param>
		/// <param name="h">�摜�̏c��</param>
		/// <param name="alphaBlendMode">�f�t�H���g�͔���������</param>
		void Init(const char* filePath, const float w, const float h, AlphaBlendMode alphaBlendMode = AlphaBlendMode_Trans);

		/// <summary>
		/// ���W��ݒ肷��(Z��0.0f�ɂ���)
		/// </summary>
		/// <param name="position">���W</param>
		void SetPosition(const Vector3& position)
		{
			m_position = position;
		}
		/// <summary>
		/// ���W��ݒ肷��(Z��0.0f�ɂ���)
		/// </summary>
		/// <param name="x">X���W</param>
		/// <param name="y">Y���W</param>
		/// <param name="z">Z���W</param>
		void SetPosition(const float x, const float y, const float z = 0.0f)
		{
			SetPosition({ x, y, z });
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
		/// �傫����ݒ肷��(Z��1.0f�ɂ���)
		/// </summary>
		/// <param name="scale"></param>
		void SetScale(const Vector3& scale)
		{
			m_scale = scale;
		}
		/// <summary>
		/// �傫����ݒ肷��(Z��1.0f�ɂ���)
		/// </summary>
		/// <param name="x">X��</param>
		/// <param name="y">Y��</param>
		/// <param name="z">Z��</param>
		void SetScale(const float x, const float y, const float z = 1.0f)
		{
			SetScale({ x, y, z });
		}
		/// <summary>
		/// �傫�����擾����
		/// </summary>
		/// <returns>�傫��</returns>
		const Vector3& GetScale() const
		{
			return m_scale;
		}

		/// <summary>
		/// ��]��ݒ�
		/// </summary>
		/// <param name="rotation">��]</param>
		void SetRotation(const Quaternion& rotation)
		{
			m_rotation = rotation;
		}
		/// <summary>
		/// ��]���擾����
		/// </summary>
		/// <returns>��]</returns>
		const Quaternion& GetRotation() const
		{
			return m_rotation;
		}
		
		/// <summary>
		/// �s�{�b�g��ݒ肷��
		/// </summary>
		/// <param name="pivot"></param>
		void SetPivot(const Vector2& pivot)
		{
			m_pivot = pivot;
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
		/// ��Z�J���[��ݒ肷��
		/// </summary>
		/// <param name="mulColor">��Z�J���[</param>
		void SetMulColor(const Vector4& mulColor)
		{
			m_sprite.SetMulColor(mulColor);
		}
		/// <summary>
		/// ��Z�J���[���擾����
		/// </summary>
		/// <returns>��Z�J���[</returns>
		const Vector4& GetMulColor() const
		{
			return m_sprite.GetMulColor();
		}

		/// <summary>
		/// �X�V����
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
		/// �`�揈��
		/// </summary>
		/// <param name="rc">�����_�[�R���e�L�X�g</param>
		void Draw(RenderContext& rc, const bool drawTiming = false, const int viewportNo = -1);

		/// <summary>
		/// �������O�ʂɕ`�悷��
		/// </summary>
		/// <param name="rc"></param>
		void DrawFront(RenderContext& rc);

		/// <summary>
		/// RenderingEngine�ŕ`�悷��Ƃ��ɌĂяo��
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g</param>
		void OnRenderSprite(RenderContext& rc)
		{
			m_sprite.Draw(rc);
		}
		
		/// <summary>
		/// �������ς݁H
		/// </summary>
		/// <returns>�������ς݂Ȃ�true</returns>
		const bool IsInit()
		{
			if (m_isInit)	return true;
			else			return false;
		}

		/// <summary>
		/// �O���[�X�P�[����ݒ肷��
		/// </summary>
		/// <param name="flag">true�̏ꍇ�O���[�X�P�[�����s��</param>
		void SetGrayScale(const bool flag)
		{
			m_spriteCB.grayScale = flag;
		}
		/// <summary>
		/// �O���[�X�P�[����������
		/// </summary>
		/// <returns>true�̏ꍇ�O���[�X�P�[����</returns>
		const bool IsGrayScale()
		{
			return m_spriteCB.grayScale;
		}

	private:
		SpriteCB	m_spriteCB;
		Sprite		m_sprite;								//Sprite�N���X

		Vector2		m_pivot = Sprite::DEFAULT_PIVOT;		//�s�{�b�g
		Vector3		m_position = Vector3::Zero;				//���W
		Vector3		m_scale = Vector3::One;					//�傫��
		Quaternion	m_rotation = Quaternion::Identity;		//��]
		bool		m_isInit = false;
	};
}
