#pragma once

struct Status
{
	int MaxHp;                 //最大ヒットポイント
	int Hp;                    //ヒットポイント(体力)
	int Atk;                   //攻撃力
	float Speed;               //移動速度
	std::string name;          //名前


	static std::unordered_map<std::string, Status> StatusDataMAP;
	static const std::string csvFilePath;
	static bool IsLoadedCSVFile;

	void Init(const std::string& name);

private:
	void LoadCSV(const std::string& name);
};

