#pragma once
class Lamp:public IGameObject
{
public:
	Lamp();
	~Lamp();

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
	
	Vector3 m_position = Vector3::Zero;
	Vector3 m_scale = Vector3::One;
	Quaternion m_rotation = Quaternion::Identity;
	
};

