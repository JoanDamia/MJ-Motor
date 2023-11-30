#pragma once
#include "Globals.h"
#include "Application.h"
#include "GameObjects.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl2.h"
#include "ImGui/imgui_impl_opengl3.h"

#include <iostream>
#include <string>
#include <vector>


class GameObjects;

class Components
{
public:
	enum class TYPE
	{
		NONE,
		MESH,
		TEXTURE,
	};

	Components(GameObjects* gameObject, TYPE type);

	virtual ~Components();
	virtual void OnGui();

	TYPE GetType() { return type; }

protected:
	TYPE type = TYPE::NONE;
	GameObjects* go = nullptr;
};