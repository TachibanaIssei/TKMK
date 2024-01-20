#include "k2EngineLowPreCompile.h"
#include "CollisionObject.h"

nsK2EngineLow::CollisionObject::CollisionObject()
{
}

nsK2EngineLow::CollisionObject::~CollisionObject()
{
	if (g_collisionObjectManager) {
		g_collisionObjectManager->RemoveCollisionObject(this);
	}
}

bool nsK2EngineLow::CollisionObject::Start()
{
	g_collisionObjectManager->AddCollisionObject(this);
	return true;
}

void nsK2EngineLow::CollisionObject::Update()
{
	if (m_isEnableAutoDelete == false)
	{
		return;
	}


	if (m_timer > m_timeLimit + 0000.1f)
	{
		DeleteGO(this);
	}
	m_timer += g_gameTime->GetFrameDeltaTime();
}

nsK2EngineLow::CollisionObjectManager::CollisionObjectManager()
{
}

nsK2EngineLow::CollisionObjectManager::~CollisionObjectManager()
{
	m_collisionObjectVector.clear();
}
