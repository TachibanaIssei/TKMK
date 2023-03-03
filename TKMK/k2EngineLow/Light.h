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
		Vector3 ptPosition;		//�ʒu
		Vector3 ptColor;		//�J���[
		float	ptRange;		//�e���͈�
	};

	struct Light
	{
		DirectionalLight directionalLight;	//�f�B���N�V�������C�g�̔z��
		float pad0;
		Vector3 cameraEyePos;	//�J�������W
		float pad1;
		Vector3 ambientLight;	//����
	};

	class SceneLight {
	public:
		void Init();
		Light& GetSceneLight()
		{
			return m_light;
		}
		void SetDirectionLight(int lightNo, Vector3 direction, Vector3 color)
		{
			m_light.directionalLight.direction = direction;
			m_light.directionalLight.color = color;
		}
		void SetAmbient(Vector3 ambient)
		{
			m_light.ambientLight = ambient;
		}
	private:
		Light m_light;
	};
}