#pragma once

class Game;
class KnightBase;

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
	void SetKnight(KnightBase* knightbase)
	{
		m_knightbase = knightbase;
	}
	KnightBase* GetKnight()
	{
		return m_knightbase;
	}


	Game* m_game = nullptr;
	KnightBase* m_knightbase = nullptr;
	Vector3 m_toCameraPos = Vector3::Zero;
	Vector3 m_moveSpeed = Vector3::Zero;
	Quaternion m_rotation = Quaternion::Identity;
};

