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
	
	/// <summary>
	/// ワールド座標をマップ用の座標に変換する
	/// </summary>
	/// <param name="worldCenterPosition">真ん中の座標</param>
	/// <param name="worldPosition">変換したい座標</param>
	/// <param name="mapPosition">変換した値が入る</param>
	/// <returns></returns>
	const bool WorldPositionConvertToMapPosition(Vector3 worldCenterPosition, Vector3 worldPosition, Vector3& mapPosition);
private:
	void PlayerMap();
	void EnemyMap();
	SpriteRender m_Map;                  //マップ画像
	SpriteRender m_MapFrame;             //マップの外枠の画像
	SpriteRender m_MapPlayer;            //マップで映すプレイヤーの画像
	SpriteRender m_MapEnemy;                //マップで映す敵の画像
	Quaternion rot;


	Neutral_Enemy* m_Neutral_Enemy = nullptr; //中立の敵
	KnightPlayer* m_knightPlayer=nullptr;        //剣士プレイヤー
};

