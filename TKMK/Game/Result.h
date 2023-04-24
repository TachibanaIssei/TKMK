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

	//èáà 
	enum EnRankPoint {
		enRankPoint_1st = 1,	//ÇPà 
		enRankPoint_2st,	//ÇQà 
		enRankPoint_3st,	//ÇRà 
		enRankPoint_4st,	//ÇSà 
		enRankPoint_Num,
	};

private:
	int charPoints[2];
	int titleScene = 1;

	FontRender moji;
	FontRender jiji;
	EnRankPoint m_rankPoint;

};

