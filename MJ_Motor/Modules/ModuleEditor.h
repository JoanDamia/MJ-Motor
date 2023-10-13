#pragma once
#include "Module.h"
#include "Globals.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl2.h"
#include "ImGui/imgui_impl_opengl3.h"

class ModuleEditor : public Module
{
public:
	ModuleEditor(Application* app, bool start_enabled = true);
	~ModuleEditor();

	SDL_GLContext context;

	bool Start();
	bool Init();
	void DrawEditor();
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	bool showDemo;
	bool showCubeDirectMode = false;
	bool showCubeBufferColors = false;
};