#pragma once
#include "Globals.h"
#include "Application.h"
#include "GameObjects.h"
#include "Components.h"
#include "C_Textures.h"

#include <iostream>
#include <string>
#include <vector>

C_Textures::C_Textures(GameObjects* gameObject) : Components(gameObjects, TYPE::TEXTURE)
{

}

C_Textures::~C_Textures()
{

}

void C_Textures::SetTexture(const char* texName)
{
	name = texName;
}


void C_Textures::OnGui()
{

	if (ImGui::CollapsingHeader("Texture"))
	{
		ImGui::Text("Texture path:");
		ImGui::SameLine();
		ImGui::Text(name);

	}

}