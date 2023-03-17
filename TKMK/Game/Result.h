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

	//‚Æ‚è‚ ‚¦‚¸‡ˆÊ‚ğ‚Â‚¯‚Ä‚·‚½‚ß‚Ì•Ï”
	int Rank = 1;

};

