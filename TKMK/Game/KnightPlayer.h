#pragma once

#include "KnightBase.h"

class Game;

class KnightPlayer:public KnightBase
{
public:
	KnightPlayer();
	~KnightPlayer();

	//bool Start();
	void Update();
	inline Vector3 GetPosition() { return m_position; }

	void SetSGame(Game* Cgame)
	{
		m_game = Cgame;
	}
	Game* GetSGame()
	{
		return m_game;
	}
	void Render(RenderContext& rc);

private:
	Game* m_game;

	//bool FirstAtkState = false;


	//Vector3 m_position = Vector3::Zero;
	////�����X�e�[�^�X �ő�HP�AHP�A�U���́A�X�s�[�h
	//Status status = { 150,150,35,150.0f };
	////���x���A�b�v���ɑ�������X�e�[�^�X
	//LvUpStatus LvUpStatus = { 30,10,30.0f };
};

