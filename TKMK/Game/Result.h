#pragma once

class Tittle;

class Result:public IGameObject
{
public:
	Result();
	~Result();
	void Update();
	bool Start();
	void Render(RenderContext& rc);
	void Rank();

	Quaternion m_sRotation;
	SpriteRender m_spriteRender;
	Tittle* tittle=nullptr;

	//����
	enum EnRankPoint {
		enRankPoint_1st = 1,	//�P��
		enRankPoint_2st,	//�Q��
		enRankPoint_3st,	//�R��
		enRankPoint_4st,	//�S��
		enRankPoint_Num,
	};

private:
	int charPoints[2];
	int titleScene = 1;

	FontRender moji;
	FontRender jiji;
	EnRankPoint m_rankPoint;

};

