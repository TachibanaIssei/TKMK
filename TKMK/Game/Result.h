#pragma once

class Tittle;

class Result:public IGameObject
{
public:
	Result();
	~Result();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	Quaternion m_sRotation;
	SpriteRender m_spriteRender;
	Tittle* tittle=nullptr;
	SoundSource* m_bgm = nullptr;

	//とりあえず順位をつけて試すための変数
	int Rank = 1;

};

