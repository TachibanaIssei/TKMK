#pragma once

class KnightPlayer;
class Neutral_Enemy;

class Map:public IGameObject
{
public:
	Map();
	~Map();
	bool Start();
	void Update();
	void Render(RenderContext& rc);


};

