#pragma once
#include "WizardBase.h"

#include "Status.h"

class Game;
class Neutral_Enemy;
class MagicBall;

class WizardPlayer:public WizardBase
{
public:
	WizardPlayer();
	~WizardPlayer();

	void Update();
	void Attack();
	void Avoidance();
	void Render(RenderContext& rc);

	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

	inline Vector3 GetPosition() { return m_position; }

	/// <summary>
	/// �}�W�b�N�{�[���̐���
	/// </summary>
	void MakeMagicBall();

	/// <summary>
	/// �A�^�b�N�̓����蔻��
	/// </summary>
	/// <param name="stickL">�U����������̃x�N�g��</param>
	/// 
	//void AttackCollision(Vector3& position/*,Vector3& stickL*/)
	//{
	//	//�R���W�����������Ă��Ȃ��Ȃ�
	//	if (AtkCollisionSetFlag == false)
	//	{
	//		AtkCollisionPos = position;
	//		AtkCollisionPos.y += 50.0f;
	//		//Vector3 stickL;
	//		AtkCollisionVec.x = g_pad[0]->GetLStickXF();
	//		AtkCollisionVec.y = g_pad[0]->GetLStickYF();
	//		//���K��
	//		AtkCollisionVec.Normalize();

	//		//Quaternion rot;
	//		////Y�����̉�]�N�H�[�^�j�I�����쐬
	//		//rot.SetRotationYFromDirectionXZ(AtkCollisionVec);
	//		////�x�N�g���ɃN�H�[�^�j�I����K��
	//		//rot.Apply(position);

	//		//�R���W�����I�u�W�F�N�g���쐬����B
	//		AtkCollision = NewGO<CollisionObject>(0);
	//		Vector3 collitionPosition = position;
	//		collitionPosition.y += 50.0f;
	//		//collisionPosition.y += 50.0f;
	//		//�{�b�N�X��̃R���W�������쐬����B
	//		AtkCollision->CreateBox(collitionPosition, //���W�B
	//			Quaternion::Identity/*(rot)*/, //��]�B
	//			Vector3(150.0f, 50.0f, 15.0f) //�傫���B
	//		);
	//		AtkCollision->SetIsEnableAutoDelete(false);
	//		AtkCollision->SetName("wizard_Attack");

	//		AtkCollisionSetFlag = true;
	//	}
	//	else
	//	{
	//		//�ړ����x�ݒ�
	//		AtkCollisionPos += AtkCollisionVec * 4.0f;
	//		//���W��ݒ�
	//		collisionObject->SetPosition(AtkCollisionPos);

	//	}
	//}


	//void AvoidanceSprite();





private:
	Game* m_game = nullptr;
	Neutral_Enemy* m_Neutral_Enemy = nullptr; //�����̓G
	//MagicBall* magicBall = nullptr;

	CollisionObject* AtkCollision;                   //�A�^�b�N�̃R���W����
	Vector3 AtkCollisionPos = Vector3::Zero;
	Vector3 AtkCollisionVec = Vector3::Zero;
	//�A�^�b�N�̃R���W�������쐬�������̔���
	bool AtkCollisionSetFlag = false;
	bool AttackFlag = false;
	float AttackTimer = 0.0f;


	//�X�L�����g�������̈ړ����x
	float SkillSpeed = 270.0f;
	//������g�������̈ړ����x
	float AvoidanceSpeed = 170.0f;

	bool m_spriteFlag = true;

	int exp = 5;

};

