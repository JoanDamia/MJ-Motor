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

void C_Mesh::SetMesh(MeshStorer* mesh, std::string meshName)
{
	this->mesh = mesh;
	name = meshName;
}

void C_Mesh::OnGui()
{
	if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("Mesh Name:");
		ImGui::SameLine();
		ImGui::Text(name.c_str());
		ImGui::Text("\n");
	}
}

void C_Mesh::Update()
{
	mesh->RenderOneMesh(go->transform->GetGlobalTransposed(), mesh->id_texture);
}