#pragma once
#include "Globals.h"
#include "Application.h"
#include "GameObjects.h"
#include "FBXLoader.h"

#include <iostream>
#include <string>
#include <vector>



class C_Textures : public Components
{
public:
	C_Textures(GameObjects* gameObject);
	~C_Textures();

	void SetTexture(const char* texName);

	void OnGui();


	const char* name;
};