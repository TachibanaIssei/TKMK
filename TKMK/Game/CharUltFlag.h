#pragma once
class CharUltFlag:public IGameObject
{
public:
	CharUltFlag();
	~CharUltFlag();

	//誰が必殺技を打ったか
	enum EnWhoUltimateSkill
	{
		enNobodyState,
		enWizardState,
		enZombieState,
		enMonsterState
	};
	EnWhoUltimateSkill m_whoUltimatSkill= enNobodyState;

	/// <summary>
	/// 必殺技を打ったキャラのステートにする。打った後はステートをenNobodyStateに戻す
	/// </summary>
	/// <param name="Who">打ったキャラのステート</param>
	void WhoUlt(EnWhoUltimateSkill Who)
	{
		m_whoUltimatSkill = Who;
	}



};

