#pragma once

struct Status
{
	int MaxHp;                 //�ő�q�b�g�|�C���g
	int Hp;                    //�q�b�g�|�C���g(�̗�)
	int Atk;                   //�U����
	float Speed;               //�ړ����x
	int GetExp;                //�L����-���l�������o���l�A�����̓G-���L�����ɗ^����o���l�̗�
	std::string name;          //���O


	static std::unordered_map<std::string, Status> StatusDataMAP;
	static const std::string csvFilePath;
	static bool IsLoadedCSVFile;

	void Init(const std::string& name);

private:
	void LoadCSV(const std::string& name);
};

