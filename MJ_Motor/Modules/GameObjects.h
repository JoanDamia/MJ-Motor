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

	static std::map<uint, GameObjects*> gameObjectList;

	//Methods
	GameObjects(GameObjects* parent, std::string name);
	~GameObjects();

	void Update();
	Components* CreateComponent(Components::TYPE);


private:

	vector <GameObjects*> children;
	vector<Components*> components;
};