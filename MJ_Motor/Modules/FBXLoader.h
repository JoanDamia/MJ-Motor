#pragma once
#include "Globals.h"
#include <string>
#include <vector>

#include "Glew/include/glew.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

using namespace std;

struct MeshStorer
{
	MeshStorer() {}

	~MeshStorer() {}

	//Parameters
	uint id_index = 0; // index in VRAM
	uint num_index = 0;
	uint* index = nullptr;
	uint id_vertex = 0; // unique vertex in VRAM
	uint num_vertex = 0;
	float* vertex = nullptr;

	//Methods
	void RenderOneMesh();

};

class FBXLoader
{
public:

	//Methods
	void Debug();
	static void FileLoader(const char* file_path, MeshStorer* ourMesh);
	void CleanUp();

	static int meshArray[];
};