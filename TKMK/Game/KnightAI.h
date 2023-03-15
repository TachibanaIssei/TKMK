#pragma once
#include "KnightBase.h"
#include "Status.h"
class Game;
class KnightPlayer;
class KnightAI:public KnightBase
{
public:
	KnightAI();
	~KnightAI();
	void Update();
	void Attack();
	void Rotation();
	void SearchPlayer();
	void Render(RenderContext& rc);
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	inline Vector3 GetPosition() { return m_position; }

	void AvoidanceSprite();
	void SetGame(Game* game)
	{
		m_game = game;
	}
	Game* GetGame()
	{
		return m_game;
	}
	

private:
	Game* m_game;//�Q�[��
	
	Quaternion				m_rotation;                    //��]
	Vector3					m_position ;                    //�|�W�V����
	Vector3					m_forward;                      //�O����
	KnightPlayer* m_KnightPlayer = nullptr;                    //���m�v���C���[
	bool					m_isSearchPlayer = false;          //�T�[�`�v���C���[
	FontRender				m_fontRender;                       //����
	SphereCollider			m_sphereCollider;							//�R���C�_�[�B
	RigidBody				m_rigidBody;						//���́B	
};

