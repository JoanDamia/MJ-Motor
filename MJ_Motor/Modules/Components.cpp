#pragma once
#include "Globals.h"
#include "Application.h"
#include "GameObjects.h"
#include "Components.h"

#include <iostream>
#include <string>
#include <vector>

Components::Components(GameObjects* game_object, TYPE type) : go(game_object), type(type)
{
}

Components::~Components()
{
}

void Components::OnGui()
{

}