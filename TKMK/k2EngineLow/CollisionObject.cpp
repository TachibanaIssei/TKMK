#include "k2EngineLowPreCompile.h"
#include "CollisionObject.h"

namespace
{
	const float ADD_TIME_LIMIT = 0.0001f;	//時間経過で消去する際に制限時間に加算する値(m_timeLimitが0のときがあるため)
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
			//名前一致！
			if (strcmp(collisionObject->GetName(), name) == 0)
			{
				//当たり判定が有効なら。
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
				//名前が長い。不一致。
				continue;
			}
			if (strncmp(name, collisionObject->GetName(), len) == 0)
			{
				//当たり判定が有効なら。
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
			//名前一致！
			if (strcmp(collisionObject->GetName(), name) == 0)
			{
				//当たり判定が有効なら。
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
				//名前が長い。不一致。
				continue;
			}
			if (strncmp(name, collisionObject->GetName(), len) == 0)
			{
				//当たり判定が有効なら。
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
			// 条件一致した要素を削除する
			if (*it == deleteCollisionObject) {
				// 削除された要素の次を指すイテレータが返される。
				it = m_collisionObjectVector.erase(it);
			}
			// 要素削除をしない場合に、イテレータを進める
			else {
				++it;
			}
		}
	}

}