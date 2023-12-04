#pragma once
#include "Globals.h"
#include "Application.h"
#include "GameObjects.h"
#include "Components.h"

#include <iostream>
#include <string>
#include <vector>

Components::Components(GameObjects* gameObject, TYPE type) : go(gameObject), type(type)
{
}

Components::~Components()
{
}

void Components::OnGui()
{

}

void Components::Update()
{

}