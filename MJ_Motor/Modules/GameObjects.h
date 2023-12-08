#pragma once
#include "Globals.h"
#include "Application.h"
#include "Components.h"
#include "C_Transform.h"

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
	C_Transform* transform = nullptr;

	//Methods
	GameObjects(GameObjects* parent, std::string name);
	~GameObjects();

	void Update();

	//Game Objects Parameters
	static std::map<uint, GameObjects*> gameObjectList;

	//Game Object Methods
	bool AddChild(GameObjects* child);
	vector <GameObjects*> GetChildren();
	GameObjects* GetChild(int n);

	//Components Methods
	Components* CreateComponent(Components::TYPE);
	vector <Components*> GetComponents();
	Components* GetSingleComponent(Components::TYPE);
	Components* GetComponentByNum(int i);

private:

	vector <GameObjects*> children;
	vector<Components*> components;
};