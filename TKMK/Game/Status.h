#pragma once

struct Status
{
public:
	static std::unordered_map<std::string, Status> StatusDataMAP;
	static const std::string csvFilePath;
	static bool IsLoadedCSVFile;

	void Init(const std::string& name);

	void SetMaxHp(const int maxhp)
	{
		m_maxHp = maxhp;
	}
	void SetHp(const int hp)
	{
		m_hp = hp;
	}
	void SetAttackPower(const int power)
	{
		m_attackPower = power;
	}
	void SetSpeed(const float speed)
	{
		m_speed = speed;
	}

	/// <summary>
	/// �ő�HP���擾����
	/// </summary>
	/// <returns></returns>
	const int GetMaxHp() const
	{
		return m_maxHp;
	}
	/// <summary>
	/// HP���擾����
	/// </summary>
	/// <returns></returns>
	const int GetHp() const
	{
		return m_hp;
	}
	/// <summary>
	/// �U���͂��擾����
	/// </summary>
	/// <returns></returns>
	const int GetAttackPower() const
	{
		return m_attackPower;
	}
	/// <summary>
	/// �ړ����x���擾����
	/// </summary>
	/// <returns></returns>
	const float GetSpeed() const
	{
		return m_speed;
	}

private:
	void LoadCSV(const std::string& name);

private:
	int m_maxHp = 0;                 //�ő�q�b�g�|�C���g
	int m_hp = 0;                    //�q�b�g�|�C���g(�̗�)
	int m_attackPower = 0;           //�U����
	float m_speed = 0.0f;            //�ړ����x
	std::string name{};				 //���O
};

