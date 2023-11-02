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
	GLuint id_texture;

	const char* bakerHouseTexPath = "Assets/BakerHouse/BakerHouse.png";

	//Methods
	void RenderOneMesh();
	static void GenerateMeshBuffer(MeshStorer* ourMesh);

};

class FBXLoader
{
public:

	//Methods
	void Debug();
	static void FileLoader(const char* file_path);
	static void RenderAll();
	static void CleanUp();

	static vector <MeshStorer*> meshesVector;
};
