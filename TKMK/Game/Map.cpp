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
	//�}�b�v�̓ǂݍ���
	m_Map.Init("Assets/sprite/minimap.DDS", 302, 302);
	//���W�ݒ�
	m_Map.SetPosition(MAP_CENTER_POSITION);
	//�}�b�v�̊O�g�̓ǂݍ���
	m_MapFrame.Init("Assets/sprite/minimap_window.DDS", 300, 300);
	//���W�ݒ�
	m_MapFrame.SetPosition(MAP_CENTER_POSITION);

	//�}�b�v�Ńv���C���[�̕\���摜�ǂݍ���
	m_MapPlayer.Init("Assets/sprite/minimap_player.DDS", 35, 35);
	//���W�ݒ�
	m_MapPlayer.SetPosition(MAP_CENTER_POSITION);

	for (int amount = 0; amount < enemys; amount++) {
		//�}�b�v�ŃG�l�~�[�̕\���摜�ǂݍ���
		m_MapEnemy[amount].Init("Assets/sprite/minimap_enemy.DDS", 35, 35);
		m_MapEnemy[amount].SetPosition(MAP_CENTER_POSITION);
	}
	
	m_game = FindGO<Game>("game");
	
	player = FindGO<Player>("player");
	//m_Neutral_Enemy = FindGO<Neutral_Enemy>("Neutral_Enemy");

	//m_game->
	m_neutral_Enemys = FindGOs<Neutral_Enemy>("Neutral_Enemy");

	

	////�z��̃T�C�Y�𒲂ׂ�for���ŉ�
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
	/// ���[���h���W���}�b�v�p�̍��W�ɕϊ�����
	/// </summary>
	/// <param name="worldCenterPosition">�^�񒆂̍��W</param>
	/// <param name="worldPosition">�ϊ����������W</param>
	/// <param name="mapPosition">�ϊ������l������</param>
	/// <returns></returns>
const bool Map::WorldPositionConvertToMapPosition(Vector3 worldCenterPosition, Vector3 worldPosition, Vector3& mapPosition)
{
	//Y���W�̓}�b�v�̍��W�Ƃ͊֌W�Ȃ��̂ŁA0�ɂ���B
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

	//�v���C���[�̉�]���擾����
	//Quaternion playerIconRot = m_knightPlayer->GetRot();
	Quaternion playerIconRot = player->CharSetRot();
	//�v���C���[�̑O����
	Vector3 playerIcon = Vector3::AxisZ;
	//�N�H�[�^�j�I�����x�N�g���ɕς���
	playerIconRot.Apply(playerIcon);
	//�x���W�ɂy�̒l������
	playerIcon.y = playerIcon.z;
	//�y���O�ɂ���
	playerIcon.z = 0;
	//�^�ォ�獡�v�Z�����x�N�g���Ɋ��Z����
	playerIconRot.SetRotation(Vector3::AxisY, playerIcon);
	//�摜���w�肷��
	m_MapPlayer.SetRotation(playerIconRot);

	Vector3 mapPosition;

	if (WorldPositionConvertToMapPosition(Vector3::Zero, playerPosition, mapPosition))
	{
		m_MapPlayer.SetPosition(mapPosition + MAP_CENTER_POSITION);
		
	}

	m_MapPlayer.Update();
}

//�G�l�~�[�̃}�b�v�̈ړ�����
void Map::EnemyMap()
{
	if (m_game->GetNeutral_EnemyContaier() == 0)
	{
		m_neutral_Enemys.clear();
		return;
	}
	//��̃G���[
	// ���x�����
	//m_neutral_Enemys = FindGOs<Neutral_Enemy>("Neutral_Enemy");
	//enemy�̐������J��Ԃ�
	int i = 0;
	for (auto enemy : m_neutral_Enemys)
	{
		//�G�l�~�[�̍��W���擾
		Vector3 enemyPosition = enemy->GetPosition();
		Vector3 mapPosition;
		//���[���h���W���}�b�v���W�ɕϊ�����
		if (WorldPositionConvertToMapPosition(Vector3::Zero, enemyPosition, mapPosition))
		{
			//���W��ݒ�
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