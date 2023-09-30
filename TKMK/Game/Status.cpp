#include "stdafx.h"
#include "Status.h"
#include <fstream>
#include <sstream>

const std::string Status::csvFilePath = "Status.csv";
std::unordered_map<std::string, Status> Status::StatusDataMAP = {};
bool Status::IsLoadedCSVFile = false;

void Status::Init(const std::string& name)
{
	if (IsLoadedCSVFile == false) {

		LoadCSV(name);
	}
	auto itr = StatusDataMAP.find(name);
	//���O���Ȃ�
	if (itr == StatusDataMAP.end())
	{
		return;
	}
	
	*this = itr->second;

	return;
}


void Status::LoadCSV(const std::string& name) {

	std::string str_buf;
	std::string buf;

	// �ǂݍ���csv�t�@�C�����J��(std::ifstream�̃R���X�g���N�^�ŊJ��)
	std::ifstream csvFile(csvFilePath);

	//�ŏ���1�s�͔�΂��B
	getline(csvFile, str_buf);
	// getline�֐���1�s���ǂݍ���(�ǂݍ��񂾓��e��str_buf�Ɋi�[)
	while (getline(csvFile, str_buf))
	{
		Status statusData;
		// �u,�v��؂育�ƂɃf�[�^��ǂݍ��ނ��߂�istringstream�^�ɂ���
		std::istringstream i_stream(str_buf);

		//���O��ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.name = buf;
		//HP��ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_maxHp = stoi(buf);
		//Attack��ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_attackPower = stoi(buf);
		//Speed��ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_speed = stof(buf);

		statusData.m_hp = statusData.m_maxHp;

		StatusDataMAP[statusData.name] = statusData;

	}

	IsLoadedCSVFile = true;
	return;

}
