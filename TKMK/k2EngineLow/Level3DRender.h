/*!
*@brief	レベル
*/

#pragma once
#include"level3D/TklFile.h"

namespace nsK2EngineLow {
	/// <summary>
	/// オブジェクト名
	/// </summary>
	struct LevelObjectData {
		Vector3 position;		//座標
		Quaternion rotation;	//回転
		Vector3 scale;			//拡大率
		const wchar_t* name;	//名前
		int number = 0;

		/// <summary>
		/// 引数で渡したオブジェクト名のオブジェクトか調べる
		/// </summary>
		/// <param name="objName">調べる名前</param>
		/// <returns>名前が同じ場合trueを返す</returns>
		bool EqualObjectName(const wchar_t* objName)
		{
			return wcscmp(objName, name) == 0;
		}

		/// <summary>
		/// 名前が前方一致か調べる
		/// </summary>
		/// <param name="n">調べる名前</param>
		/// <returns>一致していたらtrueを返す</returns>
		bool ForwardMatchName(const wchar_t* n)
		{
			auto len = wcslen(n);
			auto namelen = wcslen(name);
			if (len > namelen) {
				//名前が長い。不一致
				return false;
			}
			return wcsncmp(n, name, len) == 0;
		}
	};

	class Level3DRender
	{
	public:
		~Level3DRender();

		/// <summary>
		/// レベルを初期化
		/// </summary>
		/// <param name="filePath">tklファイルのファイルパス</param>
		/// <param name="hookFunc"></param>
		void Init(const char* filePath, std::function<bool(LevelObjectData& objData)> hookFunc);
	
	private:
		//tklファイルの行列を変換する
		void MatrixTklToLevel();

	private:
		using BonePtr = std::unique_ptr<Bone>;
		std::vector<BonePtr> m_bonelist;
		TklFile m_tklFile;
	};
}