#pragma once

class Game;
class KnightBase;
class KnightPlayer;

class GameCamera:public IGameObject
{
public:
	GameCamera();
	~GameCamera();
	bool Start();
	void Update();
	void SetSGame(Game* Cgame)
	{
		m_game = Cgame;
	}
	Game* GetSGame()
	{
		return m_game;
	}

	void SetKnight(KnightPlayer* knightplayer)
	{
		m_knightplayer = knightplayer;
	}

	KnightPlayer* GetKnight()
	{
		return m_knightplayer;
	}


	Game* m_game = nullptr;
	KnightBase* m_knightbase = nullptr;
	KnightPlayer* m_knightplayer = nullptr;
	Vector3 m_toCameraPos = Vector3::Zero;
	Vector3 m_moveSpeed = Vector3::Zero;
	Quaternion m_rotation = Quaternion::Identity;
};

