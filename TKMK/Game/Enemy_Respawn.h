#pragma once
class Enemy_Respawn
{
public:
	//一定時間ごとに呼び出してエネミーの数がmaxではなかったらエネミーの数を増やす
	Enemy_Respawn();
	virtual ~Enemy_Respawn();

	void Respawn();

};

