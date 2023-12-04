#pragma once
#include "Globals.h"
#include "Application.h"
#include "GameObjects.h"
#include "Components.h"
#include "FBXLoader.h"
#include "C_Mesh.h"
#include "C_Textures.h"
#include "C_Transform.h"

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

	//transform = dynamic_cast<C_Transform*>(CreateComponent(Components::TYPE::TRANSFORM));

	if (parent != nullptr) parent->AddChild(this);
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

void GameObjects::Update()
{

}


// -----------------------------------------------------------------
uint GameObjects::CreateGameObject(GameObjects* parent, std::string name)
{
	GameObjects* gameObject = new GameObjects(parent, name);

	return gameObject->id;
}

bool GameObjects::AddChild(GameObjects* child)
{
	children.push_back(child);

	return true;
}

vector <GameObjects*> GameObjects::GetChildren()
{
	return children;
}

GameObjects* GameObjects::GetChild(int n)
{
	return children[n];
}


// -----------------------------------------------------------------
Components* GameObjects::CreateComponent(Components::TYPE type)
{
	Components* newComponent;

	switch (type)
	{
	case Components::TYPE::MESH:
		newComponent = new C_Mesh(this);
		break;
	case Components::TYPE::TEXTURE:
		newComponent = new C_Textures(this);
		break;
	case Components::TYPE::TRANSFORM:
		newComponent = new C_Transform(this);
		break;
	default:
		LOG("Component type error");
		App->editor->console_log.AddLog(__FILE__, __LINE__, "Component type error");
		break;
	}

	components.push_back(newComponent);

	return newComponent;
}

vector <Components*> GameObjects::GetComponents()
{
	return components;
}

Components* GameObjects::GetSingleComponent(Components::TYPE type)
{
	for (auto component : components)
	{
		if (component->GetType() == type)
		{
			return component;
		}
	}
	return NULL;
}


