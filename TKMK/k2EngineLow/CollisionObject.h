#pragma once
#include "physics/PhysicsGhostObject.h"

namespace nsK2EngineLow {
	/// <summary>
	/// 他のコリジョンと当たり判定を行うクラス
	/// </summary>
	class CollisionObject:public IGameObject
	{
	public:
		CollisionObject();
		~CollisionObject();
		bool Start();
		void Update();

		/// ボックス形状のゴーストオブジェクトを作成。
		/// </summary>
		/// <param name="pos">座標。</param>
		/// <param name="rot">回転。</param>
		/// <param name="size">サイズ。</param>
		void CreateBox(Vector3 pos, Quaternion rot, Vector3 size)
		{
			m_physicsGhostObject.CreateBox(pos, rot, size);
		}

		/// <summary>
		/// カプセル形状のゴーストオブジェクトを作成。
		/// </summary>
		/// <param name="pos">座標。</param>
		/// <param name="rot">回転。</param>
		/// <param name="radius">カプセルの半径。</param>
		/// <param name="height">カプセルの高さ。</param>
		void CreateCapsule(Vector3 pos, Quaternion rot, float radius, float height)
		{
			m_physicsGhostObject.CreateCapsule(pos, rot, radius, height);
		}

		/// <summary>
		/// 球形状のゴーストオブジェクトを作成。
		/// </summary>
		/// <param name="pos">座標。</param>
		/// <param name="rot">回転。</param>
		/// <param name="radius">球形の半径。</param>
		void CreateSphere(Vector3 pos, Quaternion rot, float radius)
		{
			m_physicsGhostObject.CreateSphere(pos, rot, radius);
		}

		/// <summary>
		/// メッシュ形状のゴーストオブジェクトを作成。
		/// </summary>
		/// <param name="pos">座標。</param>
		/// <param name="rot">回転。</param>
		/// <param name="model">モデル。</param>
		/// <param name="worldMatrix">ワールド行列。</param>
		void CreateMesh(Vector3 pos, Quaternion rot, const Model& model, const Matrix& worldMatrix)
		{
			m_physicsGhostObject.CreateMesh(pos, rot, model, worldMatrix);
		}

		/// <summary>
		/// 設定した時間(秒)が経過すると自動で削除されます。0.0fを設定したら1フレーム経過で削除されます。
		/// </summary>
		/// <param name="timeLimit">削除したい時間(秒)。</param>
		void SetTimeLimit(const float timeLimit)
		{
			m_timeLimit = timeLimit;
		}

		/// <summary>
		/// 名前を設定。
		/// </summary>
		/// <param name="name">名前。</param>
		void SetName(const char* name)
		{
			m_name = name;
		}

		/// <summary>
		/// 名前を取得。
		/// </summary>
		/// <returns></returns>
		const char* GetName() const
		{
			return m_name;
		}

		/// <summary>
		/// 制作者の名前を設定。
		/// </summary>
		/// <param name="name">名前。</param>
		void SetCreatorName(const char* name)
		{
			m_creatorName = name;
		}

		/// <summary>
		/// 制作者の名前を取得。
		/// </summary>
		/// <returns></returns>
		const char* GetCreatorName() const
		{
			return m_creatorName;
		}

		/// <summary>
		/// 座標を設定。
		/// </summary>
		/// <param name="position"></param>
		void SetPosition(const Vector3& position)
		{
			m_physicsGhostObject.SetPosition(position);
		}
		/// <summary>
		/// 座標を取得
		/// </summary>
		/// <returns></returns>
		const Vector3& GetPosition()
		{
			return m_physicsGhostObject.GetPosition();
		}
		/// <summary>
		/// 回転を設定。
		/// </summary>
		/// <param name="rotation"></param>
		void SetRotation(const Quaternion& rotation)
		{
			m_physicsGhostObject.SetRotation(rotation);
		}

		/// <summary>
		/// 行列を設定。
		/// </summary>
		/// <param name="rotation">行列。</param>
		void SetWorldMatrix(const Matrix& matrix);

		/// <summary>
		/// 自動で削除するかどうかを設定する。
		/// </summary>
		/// <param name="isFlag">trueなら自動で削除される。falseなら自動で削除されないので、DeleteGOをする必要がある。</param>
		void SetIsEnableAutoDelete(bool isFlag)
		{
			m_isEnableAutoDelete = isFlag;
		}

		/// <summary>
		/// コリジョンオブジェクト同士の当たり判定。
		/// </summary>
		/// <param name="collisionObject">衝突判定したいコリジョンオブジェクト。</param>
		/// <returns>衝突したらtrue。</returns>
		const bool IsHit(CollisionObject* collisionObject) const;

		/// <summary>
		/// コリジョンオブジェクトとキャラコンの当たり判定。
		/// </summary>
		/// <param name="collisionObject">衝突判定したいキャラコン。</param>
		/// <returns>衝突したらtrue。</returns>
		const bool IsHit(CharacterController& characterController) const;

		/// <summary>
		/// ゴーストオブジェクトを取得。
		/// </summary>
		/// <returns>ゴーストオブジェクト。</returns>
		PhysicsGhostObject& GetGhostObject()
		{
			return m_physicsGhostObject;
		}

		/// <summary>
		/// btコリジョンオブジェクトを取得。
		/// </summary>
		/// <returns>btコリジョンオブジェクト。</returns>
		btCollisionObject& GetbtCollisionObject()
		{
			return m_physicsGhostObject.GetbtCollisionObject();
		}

		/// <summary>
		/// 当たり判定が有効かどうかを設定する。
		/// </summary>
		/// <param name="isEnable">有効にしたいならtrue。</param>
		void SetIsEnable(bool isEnable)
		{
			m_isEnable = isEnable;
		}

		/// <summary>
		/// 当たり判定が有効かどうかを取得する。
		/// </summary>
		/// <returns>当たり判定が有効ならtrue。</returns>
		const bool IsEnable() const
		{
			return m_isEnable;
		}

	private:
		PhysicsGhostObject			m_physicsGhostObject;				//ゴーストオブジェクト
		const char* m_name = nullptr;									//名前。
		const char* m_creatorName = nullptr;							//制作者の名前。
		float						m_timer = 0.0f;						//タイマー
		float						m_timeLimit = 0.0f;					//削除されるまでの時間
		bool						m_isEnableAutoDelete = true;		//自動で削除されるならtrue
		bool						m_isEnable = true;					//trueなら当たり判定有効
	};


	/// <summary>
	/// コリジョンオブジェクトマネージャー
	/// </summary>
	class CollisionObjectManager : public Noncopyable
	{
	public:
		CollisionObjectManager();
		~CollisionObjectManager();

		/// <summary>
		/// コリジョンオブジェクトを追加
		/// </summary>
		/// <param name="collisionObject"></param>
		void AddCollisionObject(CollisionObject* collisionObject)
		{
			m_collisionObjectVector.push_back(collisionObject);
		}

		/// <summary>
		/// 名前が完全一致するコリジョンオブジェクトを検索する。こちらは1つだけ。
		/// </summary>
		/// <param name="name">名前。</param>
		/// <returns>コリジョンオブジェクト。見つからなかった場合はnullptr。</returns>
		const CollisionObject* FindCollisionObject(const char* name);

		/// <summary>
		/// 名前が前方一致するコリジョンオブジェクトを検索する。こちらは1つだけ。
		/// </summary>
		/// <param name="name">名前。</param>
		/// <returns>コリジョンオブジェクト。見つからなかった場合はnullptr。</returns>
		const CollisionObject* FindMatchForwardNameCollisionObject(const char* name) const;

		/// <summary>
		/// 名前が完全一致するコリジョンオブジェクトを検索する。こちらは複数。
		/// </summary>
		/// <param name="name">名前。</param>
		/// <returns>コリジョンオブジェクトの配列。</returns>
		const std::vector<CollisionObject*>& FindCollisionObjects(const char* name);

		/// <summary>
		/// 名前が前方一致するコリジョンオブジェクトを検索する。こちらは複数。
		/// </summary>
		/// <param name="name">名前。</param>
		/// <returns>コリジョンオブジェクトの配列。</returns>
		const std::vector<CollisionObject*>& FindMatchForwardNameCollisionObjects(const char* name);

		/// <summary>
		/// 配列からコリジョンオブジェクトを削除。
		/// </summary>
		/// <param name="deleteCollisionObject">削除したいコリジョンオブジェクト。</param>
		void RemoveCollisionObject(const CollisionObject* deleteCollisionObject);

	private:
		std::vector<CollisionObject*>		m_collisionObjectVector;						//コリジョンオブジェクトのコンテナ
		std::vector<CollisionObject*>		m_findsCollisionObjectVector;					//名前が前方一致のコリジョンオブジェクトのコンテナ
		std::vector<CollisionObject*>		m_findMatchForwardNameCollisionObjectVector;	//名前が完全一致のコリジョンオブジェクトのコンテナ
	};

}
