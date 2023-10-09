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
	/// 最大HPを取得する
	/// </summary>
	/// <returns></returns>
	const int GetMaxHp() const
	{
		return m_maxHp;
	}
	/// <summary>
	/// HPを取得する
	/// </summary>
	/// <returns></returns>
	const int GetHp() const
	{
		return m_hp;
	}
	/// <summary>
	/// 攻撃力を取得する
	/// </summary>
	/// <returns></returns>
	const int GetAttackPower() const
	{
		return m_attackPower;
	}
	/// <summary>
	/// 移動速度を取得する
	/// </summary>
	/// <returns></returns>
	const float GetSpeed() const
	{
		return m_speed;
	}

private:
	void LoadCSV(const std::string& name);

private:
	int m_maxHp = 0;                 //最大ヒットポイント
	int m_hp = 0;                    //ヒットポイント(体力)
	int m_attackPower = 0;           //攻撃力
	float m_speed = 0.0f;            //移動速度
	std::string name{};				 //名前
};

