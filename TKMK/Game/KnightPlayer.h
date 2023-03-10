#pragma once
#include "KnightBase.h"
#include "Status.h"
class Game;

class KnightPlayer:public KnightBase
{
public:
	KnightPlayer();
	~KnightPlayer();
	void Update();

	void Attack();
	void Render(RenderContext& rc);
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

	inline Vector3 GetPosition() { return m_position; }

	void SetSGame(Game* Cgame)
	{
		m_game = Cgame;
	}
	Game* GetSGame()
	{
		return m_game;
	}
	/// <summary>
	/// �摜�\���t���O���擾����
	/// </summary>
	/// <returns>false�Ȃ�\�����Ȃ�</returns>
	const bool GetSpriteFlag() const
	{
		return m_spriteFlag;
	}
private:
	Game* m_game=nullptr;
	
	Vector3 AnimEndPos = Vector3::Zero;
	Vector3 OldPos = Vector3::Zero;
	Vector3 UltPos = Vector3::Zero;
	float UltimateSkillTimer = 0;

	enum AtkTimingState
	{
		FirstAtk_State,
		SecondAtk_State,
		SecondAtkStart_State,
		LastAtk_State,
		Num_State,

	};
	AtkTimingState m_AtkTmingState = Num_State;

	//bool AtkCollistionFlag = false;
	
	bool UltimateSkillFlag = false;

	Status m_Status;                           //�X�e�[�^�X
	bool m_spriteFlag = true;					//�N���A������false
	//Vector3 m_position = Vector3::Zero;
	////�����X�e�[�^�X �ő�HP�AHP�A�U���́A�X�s�[�h
	//Status status = { 150,150,35,150.0f };
	////���x���A�b�v���ɑ�������X�e�[�^�X
	//LvUpStatus LvUpStatus = { 30,10,30.0f };
};

