#pragma once

class BackGround : public IGameObject
{
public:
	BackGround();
	~BackGround();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void SetPosition(Vector3 position)
	{
		m_position = position;
	}
	void SetScale(Vector3 scale)
	{
		m_scale = scale;
	}
	void SetRotation(Quaternion rotation)
	{
		m_rotation = rotation;
	}

	Vector3 GetPosition() {
		return m_position;
	}

private:
	ModelRender m_modelRenderGround;
	ModelRender m_modelRenderObject;
	ModelRender m_modelRenderWall;
	Vector3 m_position = Vector3::Zero;
	Vector3 m_scale = Vector3::One;
	Quaternion m_rotation = Quaternion::Identity;
	PhysicsStaticObject m_physicsStaticObjectGround;
	PhysicsStaticObject m_physicsStaticObjectTower;
	PhysicsStaticObject m_physicsStaticObjectWall;

};

