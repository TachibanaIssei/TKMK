#pragma once
namespace nsK2EngineLow {
	struct DirectionalLight
	{
		Vector3 lightDirection;	//���C�g�̕���
		float pad0;
		Vector3 ligColor;		//���C�g�̃J���[
		float pad1;
		Vector3 CameraEyePos;	//�J�������W
	};

	class Light
	{
	public:
		void Init();

		/// <summary>
		/// �f�B���N�V�������C�g���擾����
		/// </summary>
		/// <returns>�f�B���N�V�������C�g</returns>
		DirectionalLight& GetDirectionalLight()
		{
			return m_directionLight;
		}

		/// <summary>
		/// �f�B���N�V�������C�g��ݒ肷��
		/// </summary>
		/// <param name="dir">���C�g�̕���</param>
		/// <param name="color">���C�g�̐F</param>
		void SetDirectionLight(Vector3 dir, Vector3 color = { 0.5f,0.5f,0.5f })
		{
			//���C�g�̕���
			m_directionLight.lightDirection = dir;
			//���C�g�̐F
			m_directionLight.ligColor = color;
			//���_�̈ʒu��ݒ�
			m_directionLight.CameraEyePos = g_camera3D->GetPosition();
		}

	private:
		DirectionalLight			m_directionLight;										//�f�B���N�V�������C�g
	};
}