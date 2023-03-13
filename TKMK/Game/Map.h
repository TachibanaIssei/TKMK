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
	/// ���[���h���W���}�b�v�p�̍��W�ɕϊ�����
	/// </summary>
	/// <param name="worldCenterPosition">�^�񒆂̍��W</param>
	/// <param name="worldPosition">�ϊ����������W</param>
	/// <param name="mapPosition">�ϊ������l������</param>
	/// <returns></returns>
	const bool WorldPositionConvertToMapPosition(Vector3 worldCenterPosition, Vector3 worldPosition, Vector3& mapPosition);
private:
	void PlayerMap();
	void EnemyMap();
	SpriteRender m_Map;                  //�}�b�v�摜
	SpriteRender m_MapFrame;             //�}�b�v�̊O�g�̉摜
	SpriteRender m_MapPlayer;            //�}�b�v�ŉf���v���C���[�̉摜
	SpriteRender m_MapEnemy;                //�}�b�v�ŉf���G�̉摜
	Quaternion rot;


	Neutral_Enemy* m_Neutral_Enemy = nullptr; //�����̓G
	KnightPlayer* m_knightPlayer=nullptr;        //���m�v���C���[
};

