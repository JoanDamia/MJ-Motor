#pragma once
#include "Globals.h"
#include <string>
#include <vector>
#include <map>

class TexLoader
{
public:
	//Parameters
	static std::map<std::string, uint> texture_names;

	//Methods
	static GLuint LoadTexture(char const* thefilename);
	void CleanUp();

};
