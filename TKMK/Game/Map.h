#pragma once

class KnightPlayer;
class Neutral_Enemy;
class Game;

class Map:public IGameObject
{
public:
	Map();
	~Map();
	bool Start();
	void Update();

	void FindEnemys() {
		m_neutral_Enemys = FindGOs<Neutral_Enemy>("Neutral_Enemy");
	}

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

	static const int enemys = 10;

	SpriteRender m_Map;                  //�}�b�v�摜
	SpriteRender m_MapFrame;             //�}�b�v�̊O�g�̉摜
	SpriteRender m_MapPlayer;            //�}�b�v�ŉf���v���C���[�̉摜
	SpriteRender m_MapEnemy[enemys];                //�}�b�v�ŉf���G�̉摜
	Quaternion rot;

	std::vector<Neutral_Enemy*> m_neutral_Enemys;

	Neutral_Enemy* m_Neutral_Enemy = nullptr; //�����̓G
	KnightPlayer* m_knightPlayer=nullptr;        //���m�v���C���[
	Game* m_game = nullptr;


};

