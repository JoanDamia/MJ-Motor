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
	string name;
	bool activate;
	GameObjects* parent = nullptr;

	//Methods
	GameObjects(GameObjects* parent, std::string name);
	~GameObjects();

	void Update();
	Components* CreateComponent(Components::TYPE);


private:

	vector <GameObjects*> children;
	vector<Components*> components;
};