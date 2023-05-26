#include "stdafx.h"
#include "Map.h"
#include "KnightPlayer.h"
#include "Neutral_Enemy.h"
#include "Player.h"
#include "Game.h"
#include "GameCamera.h"

namespace
{
	const Vector3 MAP_CENTER_POSITION = Vector3(-780.0f, 360.0f, 0.0f);

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
	m_Map.Init("Assets/sprite/minimap.DDS", 350, 350);
	//���W�ݒ�
	m_Map.SetPosition(MAP_CENTER_POSITION);
	////�}�b�v�̊O�g�̓ǂݍ���
	//m_MapFrame.Init("Assets/sprite/minimap_window.DDS", 300, 300);
	////���W�ݒ�
	//m_MapFrame.SetPosition(MAP_CENTER_POSITION);

	//�}�b�v�Ńv���C���[�̕\���摜�ǂݍ���
	m_MapPlayer.Init("Assets/sprite/minimap_player.DDS", 35, 35);
	//���W�ݒ�
	m_MapPlayer.SetPosition(MAP_CENTER_POSITION);

	m_PlayerCamera.Init("Assets/sprite/CameraAngle.DDS",50, 50);
	m_PlayerCamera.SetPosition(MAP_CENTER_POSITION);
	m_PlayerCamera.SetMulColor({ 1.0f, 1.0f, 1.0f, 0.5f });
	m_PlayerCamera.SetPivot({ 0.5f,1.0f });

	m_game = FindGO<Game>("game");

	m_camera = FindGO<GameCamera>("gamecamera");
	
	player = FindGO<Player>("player");
	
	return true;
}
void Map::Update()
{
	PlayerMap();
	CameraMap();
	EnemyMap();
	m_Map.Update();
	//m_MapFrame.Update();
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

void Map::CameraMap()
{
	//�J�����̍��W
	Vector3 CameraPosition = m_camera->GetSpringCamera()->GetRealPosition();
	Vector3 playerPosition = player->GetCharPosition();
	//�J�����̉�]
	Quaternion CameraRot = Quaternion::Identity;
	CameraRot.SetRotationYFromDirectionXZ(CameraPosition - playerPosition);
	//�J�����̑O����
	Vector3 CameraIcon = Vector3::AxisZ;
	//�N�H�[�^�j�I�����x�N�g���ɕς���
	CameraRot.Apply(CameraIcon);
	//�x���W�ɂy�̒l������
	CameraIcon.y = CameraIcon.z;
	//�y���O�ɂ���
	CameraIcon.z = 0;
	//�^�ォ�獡�v�Z�����x�N�g���Ɋ��Z����
	CameraRot.SetRotation(Vector3::AxisY, CameraIcon);
	//�摜���w�肷��
	m_PlayerCamera.SetRotation(CameraRot);

	Vector3 mapPosition;
	CameraPosition = player->GetCharPosition();
	Vector3 diff = player->GetPlayerActor()->GetForward();
	diff *= -50.0f;
	CameraPosition += diff;

	if (WorldPositionConvertToMapPosition(Vector3::Zero, CameraPosition, mapPosition))
	{
		m_PlayerCamera.SetPosition(mapPosition + MAP_CENTER_POSITION);
	}

	m_PlayerCamera.Update();
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
	
}

void Map::Render(RenderContext& rc) 
{
	if (m_game->GetStopFlag() == true) {
		return;
	}

	m_Map.Draw(rc);
	//m_MapFrame.Draw(rc);
	

	for (auto enemy : m_game->GetNeutral_Enemys())
	{
		enemy->EnemyMap(rc);
		m_MapPlayer.Draw(rc);
		m_PlayerCamera.Draw(rc);
	}
}