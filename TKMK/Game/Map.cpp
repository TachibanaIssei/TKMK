#include "stdafx.h"
#include "Map.h"
#include "KnightPlayer.h"
#include "Neutral_Enemy.h"
#include "Player.h"
#include "Game.h"


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

	for (int amount = 0; amount < enemys; amount++) {
		//マップでエネミーの表示画像読み込み
		m_MapEnemy[amount].Init("Assets/sprite/minimap_enemy.DDS", 35, 35);
		m_MapEnemy[amount].SetPosition(MAP_CENTER_POSITION);
	}
	
	m_game = FindGO<Game>("game");
	
	player = FindGO<Player>("player");
	//m_Neutral_Enemy = FindGO<Neutral_Enemy>("Neutral_Enemy");

	//m_game->
	m_neutral_Enemys = FindGOs<Neutral_Enemy>("Neutral_Enemy");

	

	////配列のサイズを調べてfor文で回す
	//for (auto seutral_Enemy : seutral_Enemys)
	//{
	//	DeleteGO(seutral_Enemy);
	//}
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
	//Vector3 playerPosition = m_knightPlayer->GetPosition();
	Vector3 playerPosition = player->GetCharPosition();

	//プレイヤーの回転を取得する
	//Quaternion playerIconRot = m_knightPlayer->GetRot();
	Quaternion playerIconRot = player->CharSetRot();
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

//エネミーのマップの移動処理
void Map::EnemyMap()
{
	if (m_game->GetNeutral_EnemyContaier() == 0)
	{
		m_neutral_Enemys.clear();
		return;
	}
	//謎のエラー
	// レベル上限
	//m_neutral_Enemys = FindGOs<Neutral_Enemy>("Neutral_Enemy");
	//enemyの数だけ繰り返す
	int i = 0;
	for (auto enemy : m_neutral_Enemys)
	{
		//エネミーの座標を取得
		Vector3 enemyPosition = enemy->GetPosition();
		Vector3 mapPosition;
		//ワールド座標をマップ座標に変換する
		if (WorldPositionConvertToMapPosition(Vector3::Zero, enemyPosition, mapPosition))
		{
			//座標を設定
			m_MapEnemy[i].SetPosition(mapPosition + MAP_CENTER_POSITION);
			m_MapEnemy[i].Update();
			i++;
		}
	}


	/*Vector3 enemyPosition = m_Neutral_Enemy->GetPosition();
	Vector3 mapPosition;

	if (WorldPositionConvertToMapPosition(Vector3::Zero, enemyPosition, mapPosition))
	{
		m_MapEnemy.SetPosition(mapPosition + MAP_CENTER_POSITION);

	}*/
	//m_MapEnemy.Update();
}
void Map::Render(RenderContext& rc) 
{

	
	m_Map.Draw(rc);
	m_MapFrame.Draw(rc);
	m_MapPlayer.Draw(rc);
	for(int amount=0;amount< m_neutral_Enemys.size();amount++) m_MapEnemy[amount].Draw(rc);
}