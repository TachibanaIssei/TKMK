#pragma once
namespace nsK2EngineLow {
	/// <summary>
	/// �f�B���N�V�������C�g�\����
	/// </summary>
	struct DirectionalLight
	{
		Vector3 lightDirection;	//���C�g�̕���
		Vector3 ligColor;		//���C�g�̃J���[
		Vector3 CameraEyePos;	//�J�������W
	};

	/// <summary>
	/// �|�C���g���C�g�\����
	/// </summary>
	struct PointLight
	{
		Vector3 ptPosition;		//�ʒu
		Vector3 ptColor;		//�J���[
		float	ptRange;		//�e���͈�
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
		
		void SetPointLight(Vector3 pos, Vector3 color = { 0.5f,0.5f,0.5 }, float range = 100.0f)
		{
			//���C�g�̈ʒu
			m_pointLight.ptPosition = pos;
			//���C�g�̉e���͈�
			m_pointLight.ptRange = range;
			//���C�g�̐F
			m_pointLight.ptColor = color;
			
		}

	private:
		DirectionalLight			m_directionLight;		//�f�B���N�V�������C�g
		PointLight					m_pointLight;			//�|�C���g���C�g
	};
}