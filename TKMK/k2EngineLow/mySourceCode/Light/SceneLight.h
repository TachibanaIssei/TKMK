#pragma once
namespace nsK2EngineLow {
	/// <summary>
	/// �f�B���N�V�������C�g�\����
	/// </summary>
	struct DirectionalLight
	{
		Vector3 direction;	//���C�g�̕���
		int castShadow;		//�e���L���X�g����H
		Vector4 color;		//���C�g�̃J���[
	};

	/// <summary>
	/// �J�������̍\����
	/// </summary>
	struct EyeInfo
	{
		Matrix mViewProjInv[MAX_VIEWPORT];	//�r���[�v���W�F�N�V�����s��̋t�s��
		Vector4 eyePos[MAX_VIEWPORT];		//�J�����̈ʒu
		int drawCameraNumber;				//�`�悵�Ă���J�����̔ԍ�
	};

	struct Light
	{
		std::array<DirectionalLight, MAX_DIRECTIONAL_LIGHT> directionalLight;	//�f�B���N�V���i�����C�g�̔z��
		Vector3 ambientLight;			//����
		float pad0;						//�p�f�B���O0
		EyeInfo eyeInfomation;			//�J�����̏��
	};

	class SceneLight {
	public:
		/// <summary>
		/// ������
		/// </summary>
		void Init();
		/// <summary>
		/// �V�[�����C�g���擾
		/// </summary>
		/// <returns></returns>
		Light& GetSceneLight()
		{
			return m_light;
		}
		/// <summary>
		/// �f�B���N�V�������C�g��ݒ肷��
		/// </summary>
		/// <param name="lightNo">���C�g�ԍ�</param>
		/// <param name="direction">���C�g�̕���</param>
		/// <param name="color">���C�g�̐F</param>
		void SetDirectionLight(const int lightNo, const Vector3 direction, const Vector4 color)
		{
			m_light.directionalLight[lightNo].direction = direction;
			m_light.directionalLight[lightNo].color = color;
		}
		/// <summary>
		/// �f�B���N�V�������C�g�̃L���X�g�V���h�E�t���O��ݒ�
		/// </summary>
		/// <param name="lightNo">���C�g�ԍ�</param>
		/// <param name="flag"></param>
		void SetDirectionLightCastShadow(const int lightNo, const bool flag)
		{
			m_light.directionalLight[lightNo].castShadow = flag;
		}
		/// <summary>
		/// �L���X�g�V���h�E�t���O���擾
		/// </summary>
		/// <param name="lightNo">���C�g�ԍ�</param>
		/// <returns>true��������e�𗎂Ƃ�</returns>
		const bool IsCastShadow(const int lightNo) const
		{
			return m_light.directionalLight[lightNo].castShadow;
		}
		/// <summary>
		/// ������ݒ肷��
		/// </summary>
		/// <param name="ambient"></param>
		void SetAmbient(const Vector3 ambient)
		{
			m_light.ambientLight = ambient;
		}
		/// <summary>
		/// �`�撆�̃J�����̔ԍ�
		/// </summary>
		/// <param name="number"></param>
		void SetDrawCameraNumber(const int number)
		{
			m_light.eyeInfomation.drawCameraNumber = number;
		}

	private:
		Light m_light;
	};
}