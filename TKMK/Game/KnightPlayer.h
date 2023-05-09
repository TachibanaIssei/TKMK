#pragma once
#include "KnightBase.h"

#include "Status.h"

class Game;
class Neutral_Enemy;
class KnightUlt;
class GameUI;
class GameCamera;

class KnightPlayer:public KnightBase
{
public:
	KnightPlayer();
	~KnightPlayer();
	bool Start();
	void Update();

	void Attack();

	void Avoidance();
	void Render(RenderContext& rc);
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

	inline Vector3 GetPosition() { return m_position; }

	void AvoidanceSprite();

	/// <summary>
	/// �K�E�Z�̓����蔻�萶������
	/// </summary>
	void MakeUltSkill();

	/// <summary>
	/// �X�L���𔭓������Ƃ��ɔ͈͓��ň�ԋ߂��G���˂炤����
	/// </summary>
	//void SkillTarget()
	//{
	//	m_neutral_Enemys = FindGOs<Neutral_Enemy>("Neutral_Enemy");

	//	Vector3 nearPos = Vector3::Zero;
	//	//��ԋ߂�����
	//	float Near = nearPos.Length();
	//	for (auto enemy : m_neutral_Enemys)
	//	{
	//		Vector3 toEnemy = enemy->GetPosition() - m_position;
	//		//�G�l�~�[�Ƃ̋������v�Z����
	//		float newNear = toEnemy.Length();
	//		//�v�Z������������ԋ߂�������菬�����Ȃ�㏑��
	//		if (Near > newNear) {
	//			Near = newNear;
	//		}
	//	}
	//	if (Near < 300) {

	//	}
	//}

private:
	void CoolTimeProcess();
	/// <summary>
	/// UI���O���[�X�P�[���ɂ��鏈��
	/// </summary>
	void GrayScaleUI();

private:
	Game* m_game=nullptr;
	//GameCamera* gameCamera = nullptr;
	GameUI* m_gameUI = nullptr;
	
	//CollisionObject* collisionObject;                     //�R���W����
	Vector3 AnimEndPos = Vector3::Zero;
	Vector3 OldPos = Vector3::Zero;
	Vector3 UltPos = Vector3::Zero;

	Vector2 Avoidance_FlamePos = Vector2::Zero;
	Vector2 Avoidance_BarPos = Vector2::Zero;

	EffectEmitter* EffectKnightSkill;
	bool m_swordEffectFlag = false;

	float UltimateSkillTimer = 0;

	//bool AtkCollistionFlag = false;
	// 
	

	bool UltimateSkillFlag = false;
	FontRender Skillfont;
	SpriteRender m_Avoidance_flameRender;
	SpriteRender m_Avoidance_barRender;
	SpriteRender m_AtkUpIcon_Render;

	//bool m_atkUpSpriteFlag = false;

	//�X�L�����g�������̈ړ����x
	float SkillSpeed = 270.0f;
	//������g�������̈ړ����x
	float AvoidanceSpeed = 170.0f;

	int oldLv;

	int dddd = 20;

	//std::vector<Neutral_Enemy*> m_neutral_Enemys;
	Neutral_Enemy* m_Neutral_Enemy = nullptr; //�����̓G
};

