#pragma once
namespace nsK2EngineLow {
	const int MAX_DIRECTIONAL_LIGHT = 4;	//�f�B���N�V�������C�g�̍ő吔

	/// <summary>
	/// �f�B���N�V�������C�g�\����
	/// </summary>
	struct DirectionalLight
	{
		Vector3 direction;	//���C�g�̕���
		float pad0;
		Vector4 color;		//���C�g�̃J���[
	};

	/// <summary>
	/// �|�C���g���C�g�\����
	/// </summary>
	struct PointLight
	{
		Vector3 position;		//�ʒu
		int isUse = false;		//�g�p���t���O
		Vector4 color;			//�J���[
		Vector3	attn;			//x�ɉe���͈�,y�ɉe�����ɗݏ悷��p�����[�^
	};

	/// <summary>
	/// �X�|�b�g���C�g�\����
	/// </summary>
	struct SpotLight 
	{
		Vector3 position;	//�ʒu
		float pad0;
		Vector3 angle;		//x�ɏƎˊp�x,y�ɉe���ɗݏ悷��p�����[�^
		float pad1;
		Vector4 color;		//�F
		Vector3 attn;		//x�ɉe���͈�,y�ɉe�����ɗݏ悷��p�����[�^
		int isUse = false;	//�g�p���t���O
		Vector3 direction;	//�Ǝ˕���
	};

	/// <summary>
	/// �������C�g
	/// </summary>
	struct HemisphereLight
	{
		Vector3 groundColor;	//�n�ʐF
		int		isUse = false;	//�g�p�t���O
		Vector3 skyColor;		//�V���F
		float pad1;
		Vector3 groundNormal;	//�n�ʖ@��
	};

	/// <summary>
	/// ���C�g�\����
	/// </summary>
	struct Light
	{
		DirectionalLight directionalLight;	//�f�B���N�V�������C�g�̔z��
		PointLight pointLight;	//�|�C���g���C�g
		float pad1;
		SpotLight spotLight;	//�X�|�b�g���C�g
		float pad2;
		HemisphereLight hemisphereLight;
		float pad3;
		Vector3 cameraEyePos;	//�J�������W
		float pad4;
		Vector4 ambientLight;	//����
		Matrix mLVP;			//���C�g�r���[�v���W�F�N�V�����s��
	};

	class SceneLight {
	public:
		void Init();
		Light& GetSceneLight()
		{
			return m_light;
		}

		/// <summary>
		/// ���C�g�r���[�v���W�F�N�V�����s���ݒ肷��
		/// </summary>
		/// <param name="LVP">���C�g�r���[�v���W�F�N�V�����s��</param>
		void SetmLVP(Matrix LVP)
		{
			m_light.mLVP = LVP;
		}
	////////////////////////////////////////////////////////////////////////////////////////
	///�f�B���N�V�������C�g�̊֐�
	////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// �f�B���N�V������ݒ肷��
		/// </summary>
		/// <param name="lightNo">���C�g�ԍ�</param>
		/// <param name="direction">���C�g����</param>
		/// <param name="color">���C�g�̐F</param>
		void SetDirectionLight(int lightNo, Vector3 direction, Vector4 color)
		{
			SetDirLightDirection(direction);
			SetDirLightColor(color);
		}
		/// <summary>
		/// �f�B���N�V�������C�g�̌��̕�����ݒ肷��
		/// </summary>
		/// <param name="direction">����</param>
		void SetDirLightDirection(Vector3 direction)
		{
			m_light.directionalLight.direction = direction;
		}
		/// <summary>
		/// �f�B���N�V�������C�g�̌��̐F��ݒ肷��
		/// </summary>
		/// <param name="color">�F</param>
		void SetDirLightColor(Vector4 color)
		{
			m_light.directionalLight.color = color;
		}
		/// <summary>
		/// �f�B���N�V�������C�g�̌��̕������擾����
		/// </summary>
		/// <returns>���̕���</returns>
		const Vector3& GetDirLigDirection() const
		{
			return m_light.directionalLight.direction;
		}
		/// <summary>
		/// �f�B���N�V�������C�g�̌��̐F���擾����
		/// </summary>
		/// <returns>�F</returns>
		const Vector4& GetDirLigColor() const
		{
			return m_light.directionalLight.color;
		}

	////////////////////////////////////////////////////////////////////////////////////////
	///�����̊֐�
	////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// ������ݒ肷��
		/// </summary>
		/// <param name="ambient">�����̋���</param>
		void SetAmbient(Vector4 ambient)
		{
			m_light.ambientLight = ambient;
		}

	////////////////////////////////////////////////////////////////////////////////////////
	///�J�����̈ʒu�̊֐�
	////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// �J�����̈ʒu��ݒ肷��
		/// </summary>
		/// <param name="eyePos"></param>
		void SetEyePos(Vector3 eyePos)
		{
			m_light.cameraEyePos = eyePos;
		}

	////////////////////////////////////////////////////////////////////////////////////////
	///�|�C���g���C�g�̊֐�
	////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// �|�C���g���C�g��ݒ肷��
		/// </summary>
		/// <param name="pos">���C�g�̈ʒu</param>
		/// <param name="color">���C�g�̐F</param>
		/// <param name="range">x�Ƀ��C�g�̉e���͈�,y�ɉe���͈͂ɗݏ悷��p�����[�^</param>
		void SetPointLight(Vector3 pos, Vector4 color,Vector3 attn)
		{
			SetPointLightPosition(pos);
			SetPointLightColor(color);
			SetPointLightAttn(attn);
			UsePointLight();
		}
		/// <summary>
		/// �|�C���g���C�g�̍��W��ݒ肷��
		/// </summary>
		/// <param name="pos"></param>
		void SetPointLightPosition(Vector3 pos)
		{
			m_light.pointLight.position = pos;
		}
		/// <summary>
		/// �|�C���g���C�g�̐F��ݒ肷��
		/// </summary>
		/// <param name="color">�F</param>
		void SetPointLightColor(Vector4 color)
		{
			m_light.pointLight.color = color;
		}
		/// <summary>
		/// �e���͈͂Ɨݏ悷��p�����[�^��ݒ�
		/// </summary>
		/// <param name="attn">X�ɉe���͈�,Y�ɗݏ悷��p�����[�^</param>
		void SetPointLightAttn(Vector3 attn)
		{
			m_light.pointLight.attn = attn;
		}
		/// <summary>
		/// �|�C���g���C�g���g�p����
		/// </summary>
		void UsePointLight()
		{
			m_light.pointLight.isUse = true;
		}
		/// <summary>
		/// �|�C���g���C�g���g�p���Ȃ�
		/// </summary>
		void UnUsePointLight()
		{
			m_light.pointLight.isUse = false;
		}

		/// <summary>
		/// �|�C���g���C�g�̈ʒu���擾����
		/// </summary>
		/// <returns>���W</returns>
		const Vector3& GetPointLightPosition() const
		{
			return m_light.pointLight.position;
		}
		/// <summary>
		/// �|�C���g���C�g�̌��̐F���擾
		/// </summary>
		/// <returns>�F</returns>
		const Vector4& GetPointLightColor() const
		{
			return m_light.pointLight.color;
		}
		/// <summary>
		/// �X�|�b�g���C�g�̉e���͈͂Ɨݏ悷��p�����[�^���擾
		/// </summary>
		/// <returns>X�ɉe���͈�,Y�ɗݏ悷��p�����[�^</returns>
		const Vector3& GetPointLightAttn() const
		{
			return m_light.pointLight.attn;
		}
		/// <summary>
		/// �|�C���g���C�g�͎g�p��?
		/// </summary>
		/// <returns>�g�p���Ȃ�true</returns>
		const int PointLightIsUse() const
		{
			return m_light.pointLight.isUse;
		}

	////////////////////////////////////////////////////////////////////////////////////////
	///�X�|�b�g���C�g�̊֐�
	////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// �X�|�b�g���C�g��ݒ肷��
		/// </summary>
		/// <param name="pos">�ʒu</param>
		/// <param name="color">�F</param>
		/// <param name="range">X�ɉe���͈�,Y�ɗݏ悷��p�����[�^</param>
		/// <param name="direction">����</param>
		/// <param name="angle">�p�x</param>
		void SetSpotLight(Vector3 pos, Vector4 color, Vector3 attn, Vector3 direction, Vector3 angle)
		{
			SetSpotLightPosition(pos);
			SetSpotLightColor(color);
			SetSpotLightAttn(attn);
			SetSpotLightDirection(direction);
			SetSpotLightAngle(angle);
			UseSpotLight();
		}
		/// <summary>
		/// �X�|�b�g���C�g�̈ʒu��ݒ�
		/// </summary>
		/// <param name="pos">���W</param>
		void SetSpotLightPosition(Vector3 pos)
		{
			m_light.spotLight.position = pos;
		}
		/// <summary>
		/// �X�|�b�g���C�g�̃��C�g�F�̐ݒ�
		/// </summary>
		/// <param name="color">�F</param>
		void SetSpotLightColor(Vector4 color)
		{
			m_light.spotLight.color = color;
		}
		/// <summary>
		/// �e���͈͂Ɨݏ悷��p�����[�^��ݒ�
		/// </summary>
		/// <param name="attn">X�ɉe���͈�,Y�ɗݏ悷��p�����[�^</param>
		void SetSpotLightAttn(Vector3 attn)
		{
			m_light.spotLight.attn = attn;
		}
		/// <summary>
		/// �X�|�b�g���C�g�̃��C�g�̕�����ݒ�
		/// </summary>
		/// <param name="direction">����</param>
		void SetSpotLightDirection(Vector3 direction)
		{
			m_light.spotLight.direction = direction;
		}
		/// <summary>
		/// �X�|�b�g���C�g�̃��C�g�̊p�x��ݒ�
		/// </summary>
		/// <param name="angle">�p�x</param>
		void SetSpotLightAngle(Vector3 angle)
		{
			m_light.spotLight.angle = angle;
		}
		/// <summary>
		/// �X�|�b�g���C�g���g�p����
		/// </summary>
		void UseSpotLight()
		{
			m_light.spotLight.isUse = true;
		}
		/// <summary>
		/// �X�|�b�g���C�g���g�p���Ȃ�
		/// </summary>
		void UnUseSpotLight()
		{
			m_light.spotLight.isUse = false;
		}

		/// <summary>
		/// �X�|�b�g���C�g�̈ʒu���擾
		/// </summary>
		/// <returns>���W</returns>
		const Vector3& GetSpotLightPosition() const
		{
			return m_light.spotLight.position;
		}
		/// <summary>
		/// �X�|�b�g���C�g�̌��̐F���擾
		/// </summary>
		/// <returns>�F</returns>
		const Vector4& GetSpotLightColor() const
		{
			return m_light.spotLight.color;
		}
		/// <summary>
		/// �X�|�b�g���C�g�̉e���͈͂Ɨݏ悷��p�����[�^���擾
		/// </summary>
		/// <returns>X�ɉe���͈�,Y�ɗݏ悷��p�����[�^</returns>
		const Vector3& GetSpotLightAttn()const
		{
			return m_light.spotLight.attn;
		}
		/// <summary>
		/// �X�|�b�g���C�g�̌��̕������擾
		/// </summary>
		/// <returns>���̕���</returns>
		const Vector3& GetSpotLightDirection() const
		{
			return m_light.spotLight.direction;
		}
		/// <summary>
		/// �X�|�b�g���C�g�̊p�x���擾����
		/// </summary>
		/// <returns>�p�x</returns>
		const Vector3& GetSpotLightAngle() const
		{
			return m_light.spotLight.angle;
		}
		/// <summary>
		/// �X�|�b�g���C�g�͎g�p���H
		/// </summary>
		/// <returns>�g�p���̏ꍇtrue</returns>
		const int SpotLightIsUse() const
		{
			return m_light.spotLight.isUse;
		}

	////////////////////////////////////////////////////////////////////////////////////////
	///�������C�g�̊֐�
	////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// �������C�g��ݒ肷��
		/// </summary>
		/// <param name="groundColor">�n�ʐF</param>
		/// <param name="skyColor">�V���F</param>
		/// <param name="groundNormal">�n�ʂ̖@��</param>
		void SetHemiLight(Vector3 groundColor, Vector3 skyColor, Vector3 groundNormal)
		{
			SetHemiLightGroundColor(groundColor);
			SetHemiLightSkyColor(skyColor);
			SetHemiLightGroundNormal(groundNormal);
			UseHemiLight();
		}
		/// <summary>
		/// �������C�g�̒n�ʐF��ݒ�
		/// </summary>
		/// <param name="groundColor">�n�ʐF</param>
		void SetHemiLightGroundColor(Vector3 groundColor)
		{
			m_light.hemisphereLight.groundColor = groundColor;
		}
		/// <summary>
		/// �������C�g�̓V���F��ݒ�
		/// </summary>
		/// <param name="skyColor">�V���F</param>
		void SetHemiLightSkyColor(Vector3 skyColor)
		{
			m_light.hemisphereLight.skyColor = skyColor;
		}
		/// <summary>
		/// �������C�g�̒n�ʂ̖@��
		/// </summary>
		/// <param name="normal">�n�ʂ̖@��</param>
		void SetHemiLightGroundNormal(Vector3 normal)
		{
			m_light.hemisphereLight.groundNormal = normal;
		}
		/// <summary>
		/// �������C�g���g�p����
		/// </summary>
		void UseHemiLight()
		{
			m_light.hemisphereLight.isUse = true;
		}
		/// <summary>
		/// �������C�g���g�p���Ȃ�
		/// </summary>
		void UnUseHemiLight()
		{
			m_light.hemisphereLight.isUse = false;
		}

		/// <summary>
		/// �������C�g�̒n�ʐF���擾����
		/// </summary>
		/// <returns>�n�ʐF</returns>
		const Vector3& GetHemiLightGroundColor() const
		{
			return m_light.hemisphereLight.groundColor;
		}
		/// <summary>
		/// �������C�g�̓V���F���擾����
		/// </summary>
		/// <returns></returns>
		const Vector3& GetHemiLightSkyColor() const
		{
			return m_light.hemisphereLight.skyColor;
		}
		/// <summary>
		/// �������C�g�̒n�ʂ̖@�����擾����
		/// </summary>
		/// <returns></returns>
		const Vector3& GetHemiLightGroundNormal() const
		{
			return m_light.hemisphereLight.groundNormal;
		}
		/// <summary>
		/// �������C�g�͎g�p��?
		/// </summary>
		/// <returns>�g�p���̂Ƃ�true</returns>
		const int HemiLightIsUse() const
		{
			return m_light.hemisphereLight.isUse;
		}

	private:
		Light m_light;
	};
}