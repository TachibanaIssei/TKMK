#pragma once
class CharUltFlag:public IGameObject
{
public:
	CharUltFlag();
	~CharUltFlag();

	//�N���K�E�Z��ł�����
	enum EnWhoUltimateSkill
	{
		enNobodyState,
		enWizardState,
		enZombieState,
		enMonsterState
	};
	EnWhoUltimateSkill m_whoUltimatSkill= enNobodyState;

	/// <summary>
	/// �K�E�Z��ł����L�����̃X�e�[�g�ɂ���B�ł�����̓X�e�[�g��enNobodyState�ɖ߂�
	/// </summary>
	/// <param name="Who">�ł����L�����̃X�e�[�g</param>
	void WhoUlt(EnWhoUltimateSkill Who)
	{
		m_whoUltimatSkill = Who;
	}



};

