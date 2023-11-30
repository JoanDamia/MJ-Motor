#pragma once
#include "Globals.h"
#include "Application.h"
#include "GameObjects.h"
#include "Components.h"
#include "FBXLoader.h"
#include "C_Mesh.h"
#include "C_Textures"

#include <iostream>
#include <string>
#include <vector>


std::map<uint, GameObjects*> GameObjects::gameObjectList;

GameObjects::GameObjects(GameObjects* parent, std::string name)
{
	gameObjectList[id_count] = this;

	this->parent = parent;
	this->name = name;

	id = id_count;
	id_count++;
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

	switch (type)
	{
	case Components::TYPE::MESH:
		new_component = new C_Mesh(this);
		break;
	case Components::TYPE::TEXTURE:
		new_component = new C_Textures(this);
		break;
	default:
		LOG("component type error");
		break;
	}

	components.push_back(new_component);

	return new_component;
}

void GameObjects::Update()
{

}



