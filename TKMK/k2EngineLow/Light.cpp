#include "k2EngineLowPreCompile.h"
#include "Light.h"

void nsK2EngineLow::Light::Init()
{
	Vector3 dir = { 1.0f, -1.0f, -1.0f };
	dir.Normalize();
	SetDirectionLight(dir);
	SetPointLight(Vector3(0.0f, 10.0f, 0.0f));
}
