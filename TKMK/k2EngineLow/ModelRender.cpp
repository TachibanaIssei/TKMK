#include "k2EngineLowPreCompile.h"
#include "ModelRender.h"

void nsK2EngineLow::ModelRender::Init(const char* tkmFilepath, AnimationClip* animationClips, int numAnimationClips, EnModelUpAxis enModelUpAxis)
{
	//tkm�t�@�C���p�X��ݒ�
	m_modelInitData.m_tkmFilePath = tkmFilepath;
	//�V�F�[�_�[�t�@�C���p�X��ݒ�
	m_modelInitData.m_fxFilePath = "Assets/shader/model.fx";
	//���f���̏������ݒ�
	m_modelInitData.m_modelUpAxis = enModelUpAxis;

	// �X�P���g�����������B
	InitSkeleton(tkmFilepath);
	// �A�j���[�V�������������B
	InitAnimation(animationClips, numAnimationClips, enModelUpAxis);

	if (m_animationClips != nullptr)
	{
		//�X�P���g�����w�肷��
		m_modelInitData.m_skeleton = &m_skeleton;
	}

	MakeDirectionData();

	m_modelInitData.m_expandConstantBuffer = &m_directionLight;
	m_modelInitData.m_expandConstantBufferSize = sizeof(m_directionLight);

	//���f���N���X�̏�����
	m_model.Init(m_modelInitData);
}

void nsK2EngineLow::ModelRender::Update()
{
	//���[���h�s��̍X�V(���W�A��]�A�傫��)
	m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);

	//�X�P���g��������������Ă�����
	if (m_skeleton.IsInited())
	{
		//�X�P���g�����X�V����
		m_skeleton.Update(m_model.GetWorldMatrix());
	}

	//�A�j���[�V������i�߂�
	m_animation.Progress(g_gameTime->GetFrameDeltaTime() * m_animationSpeed);
}

void nsK2EngineLow::ModelRender::MakeDirectionData()
{
	//���C�g�̕���
	m_directionLight.lightDirection = Vector3{ 0.0f,-1.0f,-1.0f };
	//���K������
	m_directionLight.lightDirection.Normalize();
	//���C�g�̐F
	m_directionLight.ligColor = Vector3{ 0.5f,0.5f,0.5 };
}

void nsK2EngineLow::ModelRender::InitSkeleton(const char* filePath)
{
	//�X�P���g���̃f�[�^��ǂݍ��݁B
	std::string skeletonFilePath = filePath;
	int pos = (int)skeletonFilePath.find(".tkm");
	skeletonFilePath.replace(pos, 4, ".tks");
	m_skeleton.Init(skeletonFilePath.c_str());
}

void nsK2EngineLow::ModelRender::InitAnimation(AnimationClip* animationClips, int numAnimationClips, EnModelUpAxis enModelUpAxis)
{
	m_animationClips = animationClips;
	m_numAnimationClips = numAnimationClips;

	if (m_animationClips != nullptr) {
		//�A�j���[�V�����̏�����
		m_animation.Init(
			m_skeleton,
			m_animationClips,
			numAnimationClips
		);
	}
}
