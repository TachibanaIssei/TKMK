#pragma once
class ModelRender
{
public:
	ModelRender();
	~ModelRender();

	/// <summary>
	/// �������@�ʏ�͂��̊֐��ŏ���������
	/// </summary>
	/// <param name="filePath">�t�@�C���p�X</param>
	/// <param name="animationClips">�A�j���[�V�����N���b�v</param>
	/// <param name="numAnimationClips">�A�j���[�V�����N���b�v�̐�</param>
	/// <param name="enModelUpAxis">���f���̏����</param>
	/// <param name="isShadowReciever">�V���h�E���V�[�o�[�t���O��true�Ȃ�e��������</param>
	void Init(
		const char* filePath,
		AnimationClip* animationClips = nullptr,
		int numAnimationClips = 0,
		EnModelUpAxis enModelUpAxis = enModelUpAxisZ,
		bool isShadowReciever = true
	);

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();

	/// <summary>
	/// �A�j���[�V�������Đ�����
	/// </summary>
	/// <param name="animNo">�A�j���[�V�����N���b�v�̔ԍ�</param>
	/// <param name="interpolateTime">�⊮����(�P��:�b)</param>
	void PlayAnimation(int animNo, float interpolateTime = 0.0f)
	{
		m_animation.Play(animNo, interpolateTime);
	}

	/// <summary>
	/// �A�j���[�V�����̍Đ��������肷��
	/// </summary>
	/// <returns>�Đ�����������true</returns>
	bool IsPlayingAnimation() const
	{
		return m_animation.IsPlaying();
	}

private:
	/// <summary>
	/// �A�j���[�V�����ςݒ��_�o�b�t�@�̌v�Z������������
	/// </summary>
	/// <param name="tkmFilePath">�t�@�C���p�X</param>
	/// <param name="enModelUpAxis">���f���̏㎲</param>
	void InitComputeAnimationVertexBuffer(
		const char* tkmFilePath,
		EnModelUpAxis enModelUpAxis
	);

	//todo RenderingEngine�ɂ��Č��Ă���
	void InitModelOnRenderGBuffer(

	);

private:
	Animation m_animation;	//�A�j���[�V����
};

