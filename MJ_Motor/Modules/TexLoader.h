#pragma once
#include "Globals.h"
#include <string>
#include <vector>

class TexLoader
{
public:

	//Methods
	GLuint LoadTexture(char const* const thefilename);
	void CleanUp();

};
