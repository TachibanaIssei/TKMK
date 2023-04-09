#pragma once
namespace nsK2EngineLow {
	const int MAX_DIRECTIONAL_LIGHT = 4;	//�f�B���N�V�������C�g�̍ő吔
	const int MAX_POINT_LIGHT = 10;			//�|�C���g���C�g�̍ő吔
	const int MAX_SPOT_LIGHT = 10;

	/// <summary>
	/// �f�B���N�V�������C�g�\����
	/// </summary>
	struct DirectionalLight
	{
		Vector3 direction;	//���C�g�̕���
		float	pad0;
		Vector4 color;		//���C�g�̃J���[
	};

	/// <summary>
	/// �|�C���g���C�g�\����
	/// </summary>
	struct PointLight
	{
		Vector3 position;		//�ʒu
		int		isUse = false;		//�g�p���t���O
		Vector4 color;			//�J���[
		Vector3	attn;			//x�ɉe���͈�,y�ɉe�����ɗݏ悷��p�����[�^
		float	pad1;
	};

	/// <summary>
	/// �X�|�b�g���C�g�\����
	/// </summary>
	struct SpotLight 
	{
		Vector3 position;	//�ʒu
		float	pad0;
		Vector3 angle;		//x�ɏƎˊp�x,y�ɉe���ɗݏ悷��p�����[�^
		float	pad1;
		Vector4 color;		//�F
		Vector3 attn;		//x�ɉe���͈�,y�ɉe�����ɗݏ悷��p�����[�^
		int		isUse = false;	//�g�p���t���O
		Vector3 direction;	//�Ǝ˕���
		float	pad2;
	};

	/// <summary>
	/// �������C�g
	/// </summary>
	struct HemisphereLight
	{
		Vector3 groundColor;	//�n�ʐF
		int		isUse = false;	//�g�p�t���O
		Vector3 skyColor;		//�V���F
		float	pad0;
		Vector3 groundNormal;	//�n�ʖ@��
	};

	/// <summary>
	/// ���C�g�\����
	/// </summary>
	struct Light
	{
		DirectionalLight	directionalLight[MAX_DIRECTIONAL_LIGHT];	//�f�B���N�V�������C�g�̔z��
		PointLight			pointLight[MAX_POINT_LIGHT];				//�|�C���g���C�g
		SpotLight			spotLight[MAX_SPOT_LIGHT];					//�X�|�b�g���C�g
		HemisphereLight		hemisphereLight;
		float				pad1;
		Vector3				cameraEyePos;								//�J�������W
		float				pad2;
		Vector4				ambientLight;								//����
		Matrix				mLVP;										//���C�g�r���[�v���W�F�N�V�����s��
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
			SetDirLightDirection(direction,lightNo);
			SetDirLightColor(color,lightNo);
		}
		/// <summary>
		/// �f�B���N�V�������C�g�̌��̕�����ݒ肷��
		/// </summary>
		/// <param name="direction">����</param>
		/// <param name="lightNo">���C�g�̔ԍ�</param>
		void SetDirLightDirection(Vector3 direction, int lightNo = 0)
		{
			m_light.directionalLight[lightNo].direction = direction;
		}
		/// <summary>
		/// �f�B���N�V�������C�g�̌��̐F��ݒ肷��
		/// </summary>
		/// <param name="color">�F</param>
		/// <param name="lightNo">���C�g�̔ԍ�</param>
		void SetDirLightColor(Vector4 color, int lightNo = 0)
		{
			m_light.directionalLight[lightNo].color = color;
		}
		/// <summary>
		/// �f�B���N�V�������C�g�̌��̕������擾����
		/// </summary>
		/// <param name="lightNo">���C�g�̔ԍ�</param>
		/// <returns>���̕���</returns>
		const Vector3& GetDirLigDirection(int lightNo = 0) const
		{
			return m_light.directionalLight[lightNo].direction;
		}
		/// <summary>
		/// �f�B���N�V�������C�g�̌��̐F���擾����
		/// </summary>
		/// <param name="lightNo">���C�g�̔ԍ�</param>
		/// <returns>�F</returns>
		const Vector4& GetDirLigColor(int lightNo = 0) const
		{
			return m_light.directionalLight[lightNo].color;
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
		/// <param name="lightNo">���C�g�̔ԍ�</param>
		/// <param name="pos">���C�g�̈ʒu</param>
		/// <param name="color">���C�g�̐F</param>
		/// <param name="range">x�Ƀ��C�g�̉e���͈�,y�ɉe���͈͂ɗݏ悷��p�����[�^</param>
		void SetPointLight(int lightNo, Vector3 pos, Vector4 color,Vector3 attn)
		{
			SetPointLightPosition(pos,lightNo);
			SetPointLightColor(color, lightNo);
			SetPointLightAttn(attn, lightNo);
			UsePointLight(lightNo);
		}
		/// <summary>
		/// �|�C���g���C�g�̍��W��ݒ肷��
		/// </summary>
		/// <param name="pos">���W</param>
		/// <param name="lightNo">���C�g�̔ԍ�</param>
		void SetPointLightPosition(Vector3 pos, int lightNo = 0)
		{
			m_light.pointLight[lightNo].position = pos;
		}
		/// <summary>
		/// �|�C���g���C�g�̐F��ݒ肷��
		/// </summary>
		/// <param name="color">�F</param>
		/// <param name="lightNo">���C�g�̔ԍ�</param>
		void SetPointLightColor(Vector4 color, int lightNo = 0)
		{
			m_light.pointLight[lightNo].color = color;
		}
		/// <summary>
		/// �e���͈͂Ɨݏ悷��p�����[�^��ݒ�
		/// </summary>
		/// <param name="attn">X�ɉe���͈�,Y�ɗݏ悷��p�����[�^</param>
		/// <param name="lightNo">���C�g�̔ԍ�</param>
		void SetPointLightAttn(Vector3 attn, int lightNo = 0)
		{
			m_light.pointLight[lightNo].attn = attn;
		}
		/// <summary>
		/// �|�C���g���C�g���g�p����
		/// </summary>
		/// <param name="lightNo">���C�g�̔ԍ�</param>
		void UsePointLight(int lightNo = 0)
		{
			m_light.pointLight[lightNo].isUse = true;
		}
		/// <summary>
		/// �|�C���g���C�g���g�p���Ȃ�
		/// </summary>
		/// <param name="lightNo">���C�g�̔ԍ�</param>
		void UnUsePointLight(int lightNo = 0)
		{
			m_light.pointLight[lightNo].isUse = false;
		}

		/// <summary>
		/// �|�C���g���C�g�̈ʒu���擾����
		/// </summary>
		/// <param name="lightNo">���C�g�̔ԍ�</param>
		/// <returns>���W</returns>
		const Vector3& GetPointLightPosition(int lightNo = 0) const
		{
			return m_light.pointLight[lightNo].position;
		}
		/// <summary>
		/// �|�C���g���C�g�̌��̐F���擾
		/// </summary>
		/// <param name="lightNo">���C�g�̔ԍ�</param>
		/// <returns>�F</returns>
		const Vector4& GetPointLightColor(int lightNo = 0) const
		{
			return m_light.pointLight[lightNo].color;
		}
		/// <summary>
		/// �X�|�b�g���C�g�̉e���͈͂Ɨݏ悷��p�����[�^���擾
		/// </summary>
		/// <param name="lightNo">���C�g�̔ԍ�</param>
		/// <returns>X�ɉe���͈�,Y�ɗݏ悷��p�����[�^</returns>
		const Vector3& GetPointLightAttn(int lightNo = 0) const
		{
			return m_light.pointLight[lightNo].attn;
		}
		/// <summary>
		/// �|�C���g���C�g�͎g�p��?
		/// </summary>
		/// <param name="lightNo">���C�g�̔ԍ�</param>
		/// <returns>�g�p���Ȃ�true</returns>
		const int PointLightIsUse(int lightNo = 0) const
		{
			return m_light.pointLight[lightNo].isUse;
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
		/// �X�|�b�g���C�g�̈ʒu��ݒ�
		/// </summary>
		/// <param name="pos">���W</param>
		void SetSpotLightPosition(Vector3 pos, int lightNo = 0)
		{
			m_light.spotLight[lightNo].position = pos;
		}
		/// <summary>
		/// �X�|�b�g���C�g�̃��C�g�F�̐ݒ�
		/// </summary>
		/// <param name="color">�F</param>
		void SetSpotLightColor(Vector4 color, int lightNo = 0)
		{
			m_light.spotLight[lightNo].color = color;
		}
		/// <summary>
		/// �e���͈͂Ɨݏ悷��p�����[�^��ݒ�
		/// </summary>
		/// <param name="attn">X�ɉe���͈�,Y�ɗݏ悷��p�����[�^</param>
		void SetSpotLightAttn(Vector3 attn, int lightNo = 0)
		{
			m_light.spotLight[lightNo].attn = attn;
		}
		/// <summary>
		/// �X�|�b�g���C�g�̃��C�g�̕�����ݒ�
		/// </summary>
		/// <param name="direction">����</param>
		void SetSpotLightDirection(Vector3 direction, int lightNo = 0)
		{
			m_light.spotLight[lightNo].direction = direction;
		}
		/// <summary>
		/// �X�|�b�g���C�g�̃��C�g�̊p�x��ݒ�
		/// </summary>
		/// <param name="angle">�p�x</param>
		void SetSpotLightAngle(Vector3 angle, int lightNo = 0)
		{
			m_light.spotLight[lightNo].angle = angle;
		}
		/// <summary>
		/// �X�|�b�g���C�g���g�p����
		/// </summary>
		void UseSpotLight(int lightNo = 0)
		{
			m_light.spotLight[lightNo].isUse = true;
		}
		/// <summary>
		/// �X�|�b�g���C�g���g�p���Ȃ�
		/// </summary>
		void UnUseSpotLight(int lightNo = 0)
		{
			m_light.spotLight[lightNo].isUse = false;
		}

		/// <summary>
		/// �X�|�b�g���C�g�̈ʒu���擾
		/// </summary>
		/// <returns>���W</returns>
		const Vector3& GetSpotLightPosition(int lightNo = 0) const
		{
			return m_light.spotLight[lightNo].position;
		}
		/// <summary>
		/// �X�|�b�g���C�g�̌��̐F���擾
		/// </summary>
		/// <returns>�F</returns>
		const Vector4& GetSpotLightColor(int lightNo = 0) const
		{
			return m_light.spotLight[lightNo].color;
		}
		/// <summary>
		/// �X�|�b�g���C�g�̉e���͈͂Ɨݏ悷��p�����[�^���擾
		/// </summary>
		/// <returns>X�ɉe���͈�,Y�ɗݏ悷��p�����[�^</returns>
		const Vector3& GetSpotLightAttn(int lightNo = 0)const
		{
			return m_light.spotLight[lightNo].attn;
		}
		/// <summary>
		/// �X�|�b�g���C�g�̌��̕������擾
		/// </summary>
		/// <returns>���̕���</returns>
		const Vector3& GetSpotLightDirection(int lightNo = 0) const
		{
			return m_light.spotLight[lightNo].direction;
		}
		/// <summary>
		/// �X�|�b�g���C�g�̊p�x���擾����
		/// </summary>
		/// <returns>�p�x</returns>
		const Vector3& GetSpotLightAngle(int lightNo = 0) const
		{
			return m_light.spotLight[lightNo].angle;
		}
		/// <summary>
		/// �X�|�b�g���C�g�͎g�p���H
		/// </summary>
		/// <returns>�g�p���̏ꍇtrue</returns>
		const int SpotLightIsUse(int lightNo = 0) const
		{
			return m_light.spotLight[lightNo].isUse;
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
		void InitDirectionLight();
		void InitPointLight();
		void InitSpotLight();

	private:
		Light m_light;
	};
}