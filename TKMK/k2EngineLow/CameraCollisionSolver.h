#pragma once
namespace nsK2EngineLow {
	/// <summary>
	/// カメラのコリジョンソルバー
	/// 
	/// カメラを背景にめり込ませたくない場合などに使用する
	/// </summary>
	class CameraCollisionSolver
	{
	public:
		/// <summary>
		/// コンストラクタ。
		/// </summary>
		CameraCollisionSolver();
		/// <summary>
		/// デストラクタ。
		/// </summary>
		~CameraCollisionSolver();
		/// <summary>
		/// 初期化。
		/// </summary>
		/// <param name="radius">カメラのバウンディングスフィアの半径。</param>
		void Init(float radius);
		/// <summary>
		/// コリジョン解決の実行。
		/// </summary>
		/// <param name="result">コリジョン解決を行う視点の座標が格納される。</param>
		/// <param name="position">コリジョン解決を行う前の視点の座標。</param>
		/// <param name="target">カメラの注視点。</param>
		/// <returns>コリジョン解決を行った場合はtrueが返ってくる。</returns>
		bool Execute(Vector3& result, const Vector3& position, const Vector3& target);
	private:
		SphereCollider		m_collider;			//コライダー。
		float				m_radius = 0.0f;	//半径。
		bool				m_isInited = false;	//初期化済みのフラグ。trueになってたら初期化済み。
	};
}