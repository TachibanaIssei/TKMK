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

	Game* game = nullptr;
	KnightBase* knightbase = nullptr;
	Vector3 m_toCameraPos = Vector3::Zero;
	Vector3 m_moveSpeed = Vector3::Zero;
	Quaternion m_rotation = Quaternion::Identity;
};

