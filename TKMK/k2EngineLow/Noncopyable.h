#pragma once
/// <summary>
/// �R�s�[�֎~�N���X
/// </summary>
namespace nsK2EngineLow {
	struct Noncopyable {
		Noncopyable() = default;
		Noncopyable(const Noncopyable&) = delete;
		Noncopyable& operator=(const Noncopyable&) = delete;
	};
}

