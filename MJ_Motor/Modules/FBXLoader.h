#pragma once
#include "Globals.h"
#include <string>
#include <vector>

using namespace std;

struct MeshStorer
{
	MeshStorer() {}

	~MeshStorer() {}

	uint id_index = 0; // index in VRAM
	uint num_index = 0;
	uint* index = nullptr;
	uint id_vertex = 0; // unique vertex in VRAM
	uint num_vertex = 0;
	float* vertex = nullptr;

};

class FBXLoader
{
public:
	//Parameters
	const char file_path;

	//Methods
	void Debug();
	static void FileLoader(const char* file_path, MeshStorer* ourMesh);
	void CleanUp();

	static int meshArray[];
};
