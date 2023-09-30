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
	//名前がない
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

	// 読み込むcsvファイルを開く(std::ifstreamのコンストラクタで開く)
	std::ifstream csvFile(csvFilePath);

	//最初の1行は飛ばす。
	getline(csvFile, str_buf);
	// getline関数で1行ずつ読み込む(読み込んだ内容はstr_bufに格納)
	while (getline(csvFile, str_buf))
	{
		Status statusData;
		// 「,」区切りごとにデータを読み込むためにistringstream型にする
		std::istringstream i_stream(str_buf);

		//名前を読み込む。
		getline(i_stream, buf, ',');
		statusData.name = buf;
		//HPを読み込む。
		getline(i_stream, buf, ',');
		statusData.m_maxHp = stoi(buf);
		//Attackを読み込む。
		getline(i_stream, buf, ',');
		statusData.m_attackPower = stoi(buf);
		//Speedを読み込む。
		getline(i_stream, buf, ',');
		statusData.m_speed = stof(buf);

		statusData.m_hp = statusData.m_maxHp;

		StatusDataMAP[statusData.name] = statusData;

	}

	IsLoadedCSVFile = true;
	return;

}
