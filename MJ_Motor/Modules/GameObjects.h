#pragma once
#include "Globals.h"
#include "Application.h"
#include "Components.h"

#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;


class GameObjects
{
public:

	//Parameters
	uint id = 0;
	uint id_count = 0;

	string name;
	bool activate;
	GameObjects* parent = nullptr;
	//C_Transform* transform = nullptr;

	static std::map<uint, GameObjects*> gameObjectList;

	//Methods
	GameObjects(GameObjects* parent, std::string name);
	~GameObjects();

	void Update();

	//Game Objects Methods
	uint CreateGameObject(GameObjects* parent, std::string name);
	bool AddChild(GameObjects* child);
	vector <GameObjects*> GetChildren();
	GameObjects* GetChild(int n);

	//Components Methods
	Components* CreateComponent(Components::TYPE);
	vector <Components*> GetComponents();
	Components* GetSingleComponent(Components::TYPE);

private:

	vector <GameObjects*> children;
	vector<Components*> components;
};