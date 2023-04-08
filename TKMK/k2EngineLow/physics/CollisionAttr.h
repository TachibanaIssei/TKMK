/*!
 * @brief	コリジョン属性
 */
#pragma once

namespace nsK2EngineLow {
	/*!
	 * @brief	コリジョン属性の大分類。
	 */
	enum EnCollisionAttr {
		enCollisionAttr_Ground,
		enCollisionAttr_Character,
		enCollisionAttr_User,		
		enCollisionAttr_Wall,      //以下にユーザー定義のコリジョン属性を設定する。
		enCollisionAttr_SlipThroughWall	// すり抜け可能な壁
	};
}