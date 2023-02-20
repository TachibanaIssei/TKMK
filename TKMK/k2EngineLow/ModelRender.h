#pragma once

namespace nsK2EngineLow {

	class ModelRender
	{
	public:
		void Init(
			const char* tkmFilepath
		)
		{
			m_modelInitData.m_tkmFilePath = tkmFilepath;
			m_modelInitData.m_fxFilePath = "Assets/shader/model.fx";
			m_modelInitData.m_modelUpAxis = enModelUpAxisZ;
			m_model.Init(m_modelInitData);
		};

		void SetPosition(Vector3 position)
		{
			m_position = position;
		};

		void SetRotation(Quaternion rotation)
		{
			m_rotation = rotation;
		}

		void SetScale(Vector3 scale)
		{
			m_scale = scale;
		}

		void Update()
		{
			m_model.UpdateWorldMatrix(m_position,m_rotation, m_scale);
		};

		void Draw(RenderContext& rc)
		{
			m_model.Draw(rc);
		};

	private:
		Vector3 m_position = Vector3(0.0f,0.0f,0.0f);
		Vector3 m_scale = Vector3::One;
		Quaternion m_rotation = Quaternion::Identity;
		Model m_model;
		ModelInitData m_modelInitData;
	};

}