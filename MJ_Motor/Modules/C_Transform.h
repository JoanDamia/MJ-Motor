#pragma once
#include "Globals.h"
#include "Application.h"
#include "GameObjects.h"
#include "Libraries/MathGeoLib/include/MathGeoLib.h"

#include <iostream>
#include <string>
#include <vector>

struct Transform
{
	Quat qRotation;
	Quat rotation;

	float4x4 localPosition;
	float4x4 globalTransPosition;
	float4x4 globalPosition;

	float3 eulRotation;
	float3 scale;
	float3 position;
};


class C_Transform : public Components
{
public:
	//Parameters
	Transform transform;

	//Methods
	C_Transform(GameObjects* gameObject);
	~C_Transform();

	float4x4 GetGlobalMatrix();
	float* GetGlobalTransposed();

	void SetTransform(float3 position, Quat rotation, float3 scale);
	void Update() override;
	void OnGui();
};
