#pragma once
#include "Globals.h"
#include "Application.h"
#include "GameObjects.h"
#include "Components.h"
#include "FBXLoader.h"
#include "C_Mesh.h"

#include <iostream>
#include <string>
#include <vector>


std::map<uint, GameObjects*> GameObjects::gameObject_list;

GameObjects::GameObjects(GameObjects* parent, std::string name)
{
	gameObject_list[id_count] = this;

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



