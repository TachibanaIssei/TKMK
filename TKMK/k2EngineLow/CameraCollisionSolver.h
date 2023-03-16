#pragma once
namespace nsK2EngineLow {
	/// <summary>
	/// �J�����̃R���W�����\���o�[
	/// 
	/// �J������w�i�ɂ߂荞�܂������Ȃ��ꍇ�ȂǂɎg�p����
	/// </summary>
	class CameraCollisionSolver
	{
	public:
		/// <summary>
		/// �R���X�g���N�^�B
		/// </summary>
		CameraCollisionSolver();
		/// <summary>
		/// �f�X�g���N�^�B
		/// </summary>
		~CameraCollisionSolver();
		/// <summary>
		/// �������B
		/// </summary>
		/// <param name="radius">�J�����̃o�E���f�B���O�X�t�B�A�̔��a�B</param>
		void Init(float radius);
		/// <summary>
		/// �R���W���������̎��s�B
		/// </summary>
		/// <param name="result">�R���W�����������s�����_�̍��W���i�[�����B</param>
		/// <param name="position">�R���W�����������s���O�̎��_�̍��W�B</param>
		/// <param name="target">�J�����̒����_�B</param>
		/// <returns>�R���W�����������s�����ꍇ��true���Ԃ��Ă���B</returns>
		bool Execute(Vector3& result, const Vector3& position, const Vector3& target);
	private:
		SphereCollider		m_collider;			//�R���C�_�[�B
		float				m_radius = 0.0f;	//���a�B
		bool				m_isInited = false;	//�������ς݂̃t���O�Btrue�ɂȂ��Ă��珉�����ς݁B
	};
}