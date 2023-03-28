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

private:
	ModelRender m_modelRender;
	ModelRender m_stagecollision;
	Vector3 m_position = Vector3::Zero;
	Vector3 m_scale = Vector3::One;
	Quaternion m_rotation = Quaternion::Identity;
	PhysicsStaticObject m_physicsStaticObject;

};

