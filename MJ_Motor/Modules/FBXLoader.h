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

	~MeshStorer() {
		delete[num_vertex]vertex;
		delete[num_index]index;
	}

	//Parameters
	uint id_index = 0; // index in VRAM
	uint num_index = 0;
	uint* index = nullptr;
	uint id_vertex = 0; // unique vertex in VRAM
	uint num_vertex = 0;
	float* vertex = nullptr;
	uint id_texture = 0;
	const char* texture_path = "C:/Users/Miguel/Documents/GitHub/MJ-Motor/MJ_Motor/Assets/Textures/Lenna_(test_image).png";

	//Methods
	void RenderOneMesh();
	static void GenerateMeshBuffer(MeshStorer* ourMesh);

};

class FBXLoader
{
public:

	//Methods
	void Debug();
	static void FileLoader(const char* file_path, MeshStorer* ourMesh);
	static void RenderAll();
	static void CleanUp();

	static vector <MeshStorer*> meshesVector;
};
