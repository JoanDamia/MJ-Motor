#pragma once
#include "Globals.h"
#include "Application.h"
#include "GameObjects.h"
#include "FBXLoader.h"

#include <iostream>
#include <string>
#include <vector>


class C_Mesh : public Components
{
public:

	//Parameters 
	std::string name;
	MeshStorer* mesh;

	//Methods
	C_Mesh(GameObjects* gameObjects);
	~C_Mesh();

	void SetMesh(MeshStorer* mesh, std::string meshName);
	void OnGui();
	void Update();

	MeshStorer* GetMesh()
	{
		return mesh;
	}
};
