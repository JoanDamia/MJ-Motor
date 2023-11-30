#pragma once
#include "Globals.h"
#include "Application.h"
#include "GameObjects.h"
#include "Components.h"
#include "C_Mesh.h"

#include <iostream>
#include <string>
#include <vector>


C_Mesh::C_Mesh(GameObjects* gameObject) : Components(gameObject, TYPE::MESH)
{

}

C_Mesh::~C_Mesh()
{

}

void C_Mesh::SetMesh(MeshStorer* mesh, std::string mesh_name)
{
	this->mesh = mesh;
	name = mesh_name;
}

void C_Mesh::OnGui()
{
	if (ImGui::CollapsingHeader("Mesh"))
	{
		ImGui::Text("Mesh Name:");
		ImGui::SameLine();
		ImGui::Text(name.c_str());
	}
}