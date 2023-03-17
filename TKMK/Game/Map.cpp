#include "stdafx.h"
#include "Map.h"
#include "KnightPlayer.h"
#include "Neutral_Enemy.h"


namespace
{
	Vector3 MAP_CENTER_POSITION = Vector3(-800.0f, 380.0f, 0.0f);

	float MAP_RADIUS = 160.0f;
	float LIMITED_RANGE_IMAGE = 600.0f;
}
Map::Map()
{

}

Map::~Map()
{

}

bool Map::Start()
{
	//マップの読み込み
	m_Map.Init("Assets/sprite/minimap.DDS", 302, 302);
	//座標設定
	m_Map.SetPosition(MAP_CENTER_POSITION);
	//マップの外枠の読み込み
	m_MapFrame.Init("Assets/sprite/minimap_window.DDS", 300, 300);
	//座標設定
	m_MapFrame.SetPosition(MAP_CENTER_POSITION);

	//マップでプレイヤーの表示画像読み込み
	m_MapPlayer.Init("Assets/sprite/minimap_player.DDS", 35, 35);
	//座標設定
	m_MapPlayer.SetPosition(MAP_CENTER_POSITION);
	//マップでエネミーの表示画像読み込み
	m_MapEnemy.Init("Assets/sprite/minimap_enemy.DDS", 35, 35);
	m_MapEnemy.SetPosition(MAP_CENTER_POSITION);

	m_knightPlayer = FindGO<KnightPlayer>("m_knightplayer");
	m_Neutral_Enemy = FindGO<Neutral_Enemy>("Neutral_Enemy");
	return true;
}
void Map::Update()
{
	PlayerMap();
	EnemyMap();
	m_Map.Update();
	m_MapFrame.Update();
}

/// <summary>
	/// ワールド座標をマップ用の座標に変換する
	/// </summary>
	/// <param name="worldCenterPosition">真ん中の座標</param>
	/// <param name="worldPosition">変換したい座標</param>
	/// <param name="mapPosition">変換した値が入る</param>
	/// <returns></returns>
const bool Map::WorldPositionConvertToMapPosition(Vector3 worldCenterPosition, Vector3 worldPosition, Vector3& mapPosition)
{
	//Y座標はマップの座標とは関係ないので、0にする。
	worldCenterPosition.y = 0.0f;
	worldPosition.y = 0.0f;
	Vector3 diff = worldPosition - worldCenterPosition;


	diff /= 8.0f;
	mapPosition = { diff.x,diff.z,0.0f };

	return true;
}


void Map::PlayerMap()
{
	Vector3 playerPosition = m_knightPlayer->GetPosition();

	//プレイヤーの回転を取得する
	Quaternion playerIconRot = m_knightPlayer->GetRot();
	//プレイヤーの前方向
	Vector3 playerIcon = Vector3::AxisZ;
	//クォータニオンをベクトルに変える
	playerIconRot.Apply(playerIcon);
	//Ｙ座標にＺの値を入れる
	playerIcon.y = playerIcon.z;
	//Ｚを０にする
	playerIcon.z = 0;
	//真上から今計算したベクトルに換算する
	playerIconRot.SetRotation(Vector3::AxisY, playerIcon);
	//画像を指定する
	m_MapPlayer.SetRotation(playerIconRot);

	Vector3 mapPosition;

	if (WorldPositionConvertToMapPosition(Vector3::Zero, playerPosition, mapPosition))
	{
		m_MapPlayer.SetPosition(mapPosition + MAP_CENTER_POSITION);
		
	}

	m_MapPlayer.Update();
}

void Map::EnemyMap()
{
	Vector3 enemyPosition = m_Neutral_Enemy->GetPosition();
	Vector3 mapPosition;

	if (WorldPositionConvertToMapPosition(Vector3::Zero, enemyPosition, mapPosition))
	{
		m_MapEnemy.SetPosition(mapPosition + MAP_CENTER_POSITION);

	}
	m_MapEnemy.Update();
}
void Map::Render(RenderContext& rc) 
{

	
	m_Map.Draw(rc);
	m_MapFrame.Draw(rc);
	m_MapPlayer.Draw(rc);
	m_MapEnemy.Draw(rc);
}