#pragma once
#include "Globals.h"
#include <string>
#include <vector>

#include "Glew/include/glew.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

//It has 5 vertex features for x,y,z,u,v
#define VERTEX_FEATURES 5

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

	const char* bakerHouseTexPath = "Assets/BakerHouse/Baker_house.png";

	//Methods
	void RenderOneMesh();

};

class FBXLoader
{
public:

	//Methods
	void Debug();
	static void FileLoader(const char* file_path);
	static void RenderAll();
	static void CleanUp();

	static void GenerateMeshBuffer(MeshStorer* ourMesh);

	static vector <MeshStorer*> meshesVector;
};
