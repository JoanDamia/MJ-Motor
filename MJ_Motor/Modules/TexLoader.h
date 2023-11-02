#pragma once
#include "Globals.h"
#include <string>
#include <vector>

class TexLoader
{
public:

	//Methods
	static GLuint LoadTexture(char const* thefilename);
	void CleanUp();

};
