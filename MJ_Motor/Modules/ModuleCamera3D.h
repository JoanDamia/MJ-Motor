#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl2.h"
#include "ImGui/imgui_impl_opengl3.h"

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Look(const float3 &Position, const float3&Reference, bool RotateAroundReference = false);
	void LookAt(const float3&Spot);
	void Move(const float3&Movement);
	float* GetViewMatrix();

	bool firstPersonView = false;

	ImVec2 Normalize(float x, float y, float w, float h, ImVec2 point);

private:

	void CalculateViewMatrix();

public:
	
	//You won't need this after using Frustum
	float3 X, Y, Z, Position, Reference;

	Frustum frustum;

private:

	mat4x4 ViewMatrix;
	//Frustum mMainCamera; Some help here :)
};