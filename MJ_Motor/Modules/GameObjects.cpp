#pragma once
#include "Globals.h"
#include "Application.h"
#include "GameObjects.h"
#include "Components.h"
#include "FBXLoader.h"

#include <iostream>
#include <string>
#include <vector>


GameObjects::GameObjects(GameObjects* parent, std::string name)
{
	App->scene_intro->gameObjects[App->fbxRender->id_count] = this;
	App->meshRender->id_count++;

	id = App->fbxRender->id_count;
	this->name = name;
}

GameObjects::~GameObjects()
{
	for (size_t i = 0; i < components.size(); i++)
	{
		if (components[i] != nullptr)
		{
			delete components[i];
			components[i] = nullptr;
		}
	}

	components.clear();
}

Components* GameObjects::CreateComponent(Components::TYPE type)
{
	Components* new_component;

	return new_component;
}

void GameObjects::Update()
{

}



