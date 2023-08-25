#include "k2EngineLowPreCompile.h"
#include "CollisionObject.h"

namespace
{
	const float ADD_TIME_LIMIT = 0.0001f;	//���Ԍo�߂ŏ�������ۂɐ������Ԃɉ��Z����l(m_timeLimit��0�̂Ƃ������邽��)
}

namespace nsK2EngineLow
{
	CollisionObject::CollisionObject()
	{
	}

	CollisionObject::~CollisionObject()
	{
		if (g_collisionObjectManager) {
			g_collisionObjectManager->RemoveCollisionObject(this);
		}
	}

	bool CollisionObject::Start()
	{
		g_collisionObjectManager->AddCollisionObject(this);
		return true;
	}

	void CollisionObject::Update()
	{
		if (m_isEnableAutoDelete == false)
		{
			return;
		}


		if (m_timer > m_timeLimit + ADD_TIME_LIMIT)
		{
			DeleteGO(this);
		}
		m_timer += g_gameTime->GetFrameDeltaTime();
	}

	void CollisionObject::SetWorldMatrix(const Matrix& matrix)
	{
		Vector3 position;
		position.x = matrix.m[3][0];
		position.y = matrix.m[3][1];
		position.z = matrix.m[3][2];
		SetPosition(position);
		Quaternion rotation;
		rotation.SetRotation(matrix);
		SetRotation(rotation);
	}

	const bool CollisionObject::IsHit(CollisionObject* collisionObject) const
	{
		bool isCollision = false;
		PhysicsWorld::GetInstance()->ContactTest(&collisionObject->GetbtCollisionObject(), [&](const btCollisionObject& contactObject) {
			if (m_physicsGhostObject.IsSelf(contactObject) == true) {
				isCollision = true;
			}
		});
		return isCollision;
	}

	const bool CollisionObject::IsHit(CharacterController& characterController) const
	{
		bool isCollision = false;
		PhysicsWorld::GetInstance()->ContactTest(characterController, [&](const btCollisionObject& contactObject) {
			if (m_physicsGhostObject.IsSelf(contactObject) == true) {
				isCollision = true;
			}
			});
		return isCollision;
	}

	CollisionObjectManager::CollisionObjectManager()
	{
	}

	CollisionObjectManager::~CollisionObjectManager()
	{
		m_collisionObjectVector.clear();
	}

	const CollisionObject* CollisionObjectManager::FindCollisionObject(const char* name)
	{
		for (auto collisionObject : m_collisionObjectVector)
		{
			//���O��v�I
			if (strcmp(collisionObject->GetName(), name) == 0)
			{
				//�����蔻�肪�L���Ȃ�B
				if (collisionObject->IsEnable() == true)
				{
					return collisionObject;
				}
			}
		}
		return nullptr;
	}

	const CollisionObject* CollisionObjectManager::FindMatchForwardNameCollisionObject(const char* name) const
	{
		for (auto collisionObject : m_collisionObjectVector)
		{
			auto len = strlen(name);
			auto namelen = strlen(collisionObject->GetName());
			if (len > namelen) {
				//���O�������B�s��v�B
				continue;
			}
			if (strncmp(name, collisionObject->GetName(), len) == 0)
			{
				//�����蔻�肪�L���Ȃ�B
				if (collisionObject->IsEnable() == true)
				{
					return collisionObject;
				}
			}
		}
		return nullptr;
	}

	const std::vector<CollisionObject*>& CollisionObjectManager::FindCollisionObjects(const char* name)
	{
		m_findMatchForwardNameCollisionObjectVector.clear();
		for (auto collisionObject : m_collisionObjectVector)
		{
			//���O��v�I
			if (strcmp(collisionObject->GetName(), name) == 0)
			{
				//�����蔻�肪�L���Ȃ�B
				if (collisionObject->IsEnable() == true)
				{
					m_findMatchForwardNameCollisionObjectVector.push_back(collisionObject);
				}
			}
		}
		return m_findMatchForwardNameCollisionObjectVector;
	}

	const std::vector<CollisionObject*>& CollisionObjectManager::FindMatchForwardNameCollisionObjects(const char* name)
	{
		m_findsCollisionObjectVector.clear();
		for (auto collisionObject : m_collisionObjectVector)
		{
			auto len = strlen(name);
			auto namelen = strlen(collisionObject->GetName());
			if (len > namelen) {
				//���O�������B�s��v�B
				continue;
			}
			if (strncmp(name, collisionObject->GetName(), len) == 0)
			{
				//�����蔻�肪�L���Ȃ�B
				if (collisionObject->IsEnable() == true)
				{
					m_findsCollisionObjectVector.push_back(collisionObject);
				}
			}
		}
		return m_findsCollisionObjectVector;
	}

	void CollisionObjectManager::RemoveCollisionObject(const CollisionObject* deleteCollisionObject)
	{
		for (auto it = m_collisionObjectVector.begin(); it != m_collisionObjectVector.end();) {
			// ������v�����v�f���폜����
			if (*it == deleteCollisionObject) {
				// �폜���ꂽ�v�f�̎����w���C�e���[�^���Ԃ����B
				it = m_collisionObjectVector.erase(it);
			}
			// �v�f�폜�����Ȃ��ꍇ�ɁA�C�e���[�^��i�߂�
			else {
				++it;
			}
		}
	}

}