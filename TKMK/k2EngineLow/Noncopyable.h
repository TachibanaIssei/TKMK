#pragma once
/// <summary>
/// コピー禁止クラス
/// </summary>
namespace nsK2EngineLow {
	struct Noncopyable {
		Noncopyable() = default;
		Noncopyable(const Noncopyable&) = delete;
		Noncopyable& operator=(const Noncopyable&) = delete;
	};
}

