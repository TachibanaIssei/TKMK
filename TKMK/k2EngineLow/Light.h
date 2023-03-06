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
		Vector3 color;		//���C�g�̃J���[
	};

	/// <summary>
	/// �|�C���g���C�g�\����
	/// </summary>
	struct PointLight
	{
		Vector3 position;		//�ʒu
		int isUse = false;		//�g�p���t���O
		Vector3 color;			//�J���[
		float pad1;
		Vector3	range;			//x�ɉe���͈�,y�ɉe�����ɗݏ悷��p�����[�^
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
		Vector3 color;		//�F
		int isUse = false;	//�g�p���t���O
		Vector3 range;		//x�ɉe���͈�,y�ɉe�����ɗݏ悷��p�����[�^
		float pad2;
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
		float pad0;
		PointLight pointLight;	//�|�C���g���C�g
		float pad1;
		SpotLight spotLight;	//�X�|�b�g���C�g
		float pad2;
		HemisphereLight hemisphereLight;
		float pad3;
		Vector3 cameraEyePos;	//�J�������W
		float pad4;
		Vector3 ambientLight;	//����
	};

	class SceneLight {
	public:
		void Init();
		Light& GetSceneLight()
		{
			return m_light;
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
		void SetDirectionLight(int lightNo, Vector3 direction, Vector3 color)
		{
			m_light.directionalLight.direction = direction;
			m_light.directionalLight.color = color;
		}
		/// <summary>
		/// �f�B���N�V�������C�g�̌��̕������擾����
		/// </summary>
		/// <returns>���̕���</returns>
		Vector3& GetDirLigDirection()
		{
			return m_light.directionalLight.direction;
		}

		////////////////////////////////////////////////////////////////////////////////////////
		///�����̊֐�
		////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// ������ݒ肷��
		/// </summary>
		/// <param name="ambient">�����̋���</param>
		void SetAmbient(Vector3 ambient)
		{
			m_light.ambientLight = ambient;
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
		void SetPointLight(Vector3 pos, Vector3 color,Vector3 range)
		{
			m_light.pointLight.position = pos;
			m_light.pointLight.color = color;
			m_light.pointLight.range = range;
			m_light.pointLight.isUse = true;
		}
		/// <summary>
		/// �|�C���g���C�g�̍��W��ݒ肷��
		/// </summary>
		/// <param name="pos"></param>
		void SetPointLightPosition(Vector3 pos)
		{
			m_light.pointLight.position = pos;
		}

		////////////////////////////////////////////////////////////////////////////////////////
		///�X�|�b�g���C�g�̊֐�
		////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// �X�|�b�g���C�g��ݒ肷��
		/// </summary>
		/// <param name="pos"></param>
		/// <param name="color"></param>
		/// <param name="range"></param>
		/// <param name="direction"></param>
		/// <param name="angle"></param>
		void SetSpotLight(Vector3 pos, Vector3 color, Vector3 range, Vector3 direction, Vector3 angle)
		{
			m_light.spotLight.position = pos;
			m_light.spotLight.color = color;
			m_light.spotLight.range = range;
			m_light.spotLight.direction = direction;
			m_light.spotLight.angle = angle;
			m_light.spotLight.isUse = true;
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
		/// �X�|�b�g���C�g�̌��̕������擾
		/// </summary>
		/// <returns>���̕���</returns>
		Vector3& GetSpotLightDirection()
		{
			return m_light.spotLight.direction;
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

	private:
		Light m_light;
	};
}