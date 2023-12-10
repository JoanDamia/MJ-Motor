#pragma once
#include "Globals.h"
#include "Application.h"
#include "GameObjects.h"
#include "Components.h"
#include "C_Transform.h"
#include "C_Mesh.h"

#include <iostream>
#include <string>
#include <vector>

C_Transform::C_Transform(GameObjects* gameObject) : Components(gameObject, TYPE::TRANSFORM)
{

	transform.globalPosition.SetIdentity();
	transform.localPosition.SetIdentity();
	transform.globalTransPosition.SetIdentity();

	transform.localPosition.Decompose(transform.position, transform.qRotation, transform.scale);
	transform.qRotation.Normalize();

	transform.eulRotation = transform.qRotation.ToEulerXYZ();

	transform.globalTransPosition = transform.globalPosition.Transposed();

}

C_Transform::~C_Transform()
{

}

void C_Transform::SetTransform(float3 position, Quat rotation, float3 scale)
{
	
	transform.position = position;
	transform.rotation = rotation.Normalized();
	transform.scale = scale;
	transform.eulRotation = transform.qRotation.ToEulerXYZ() * RADTODEG;
	transform.localPosition = float4x4::FromTRS(transform.position, transform.qRotation, transform.scale);

	if (this->go->parent != nullptr)
	{
		if (this->go->parent->transform != nullptr)
		{
			transform.globalPosition = go->parent->transform->transform.globalPosition * transform.localPosition;
		}
	}

	transform.globalTransPosition = transform.globalPosition.Transposed();
}

float* C_Transform::GetGlobalTransposed()
{
	return transform.globalTransPosition.ptr();
}

void C_Transform::Update()
{

	C_Mesh* Cmesh = dynamic_cast<C_Mesh*>(go->GetSingleComponent(Components::TYPE::MESH));

	if (Cmesh != nullptr && Cmesh->mesh->localAABB_init)
	{
		Cmesh->mesh->GenerateGlobalBoundingBox();
	}

	transform.qRotation = Quat::FromEulerXYZ(transform.eulRotation.x * DEGTORAD, transform.eulRotation.y * DEGTORAD, transform.eulRotation.z * DEGTORAD);
	transform.qRotation.Normalize();
	transform.localPosition = float4x4::FromTRS(transform.position, transform.qRotation, transform.scale);


	if (this->go->id > 0)
	{
		if (this->go->parent != nullptr)
		{
			if (this->go->parent->transform != nullptr)
			{
				// We apply the posicion formula
				this->transform.globalPosition = this->go->parent->transform->transform.globalPosition * this->transform.localPosition;
				this->transform.globalTransPosition = this->transform.globalPosition.Transposed();
			}
		}
	}

	//Center camera to origin
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
	{
		if (App->editor->gameobject_selected)
		{
			App->camera->LookAt(App->editor->gameobject_selected->transform->go->transform->transform.position + App->editor->gameobject_selected->parent->transform->go->transform->transform.position);
		}
		else
		{
			App->camera->LookAt(float3(0, 0, 0));
		}
	}
}

float4x4 C_Transform::GetGlobalMatrix()
{
	return transform.globalPosition;
}


void C_Transform::OnGui()
{
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("Position: ");
		ImGui::SameLine();
		ImGui::DragFloat3("##Pos", &transform.position[0], 0.1f);

		ImGui::Text("Rotation: ");
		ImGui::SameLine();
		ImGui::DragFloat3("##Rot", &transform.eulRotation[0], 0.1f);

		ImGui::Text("Scale:");
		ImGui::SameLine();
		ImGui::DragFloat3("##Sca", &transform.scale[0], 0.1f);

		ImGui::Text("\n");

		if (ImGui::Button("Reset Position"))
		{
			this->transform.position[0] = (0.0f);
			this->transform.position[1] = (0.0f);
			this->transform.position[2] = (0.0f);
		}
		ImGui::SameLine();

		if (ImGui::Button("Reset Rotation"))
		{
			this->transform.eulRotation[0] = (0.0f);
			this->transform.eulRotation[1] = (0.0f);
			this->transform.eulRotation[2] = (0.0f);
		}
		ImGui::SameLine();

		if (ImGui::Button("Reset Scale"))
		{
			this->transform.scale[0] = (1.0f);
			this->transform.scale[1] = (1.0f);
			this->transform.scale[2] = (1.0f);
		}

		ImGui::Text("\n");
	}
}