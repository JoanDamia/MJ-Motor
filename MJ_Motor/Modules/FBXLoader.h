#pragma once
#include "Globals.h"
#include <string>
#include <vector>

#include "Glew/include/glew.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "MathGeoLib/include/MathGeoLib.h"

#include <Assimp/include/cimport.h>
#include <Assimp/include/scene.h>
#include "Assimp/include/postprocess.h"
#pragma comment (lib, "Assimp/libx86/assimp.lib")

//It has 5 vertex features for x,y,z,u,v
#define VERTEX_FEATURES 5


using namespace std;

class GameObjects;

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

	uint ID;

	AABB globalAABB;
	OBB globalOBB;
	AABB localAABB;
	bool localAABB_init = false;

	const char* bakerHouseTexPath = "Assets/BakerHouse/Baker_house.png";
	const char* building01TexPath = "building 01_c.tga";

	//Methods
	void RenderOneMesh(const GLfloat* globalTransform, uint texID);
	void GenerateGlobalBoundingBox();
	void GenerateLocalBoundingBox();
	void RenderAABB();
};

class FBXLoader
{
public:
	//Parameters
	static vector <MeshStorer*> meshesVector;
	float* transGlobalPos;

	//Methods
	void Debug();
	static void FileLoader(const char* file_path);
	static void RenderAll();
	static void CleanUp();

	static void GenerateMeshBuffer(MeshStorer* ourMesh);

	static void GetNodeInfo(const aiScene* rootScene, aiNode* rootNode, GameObjects* go);
};
