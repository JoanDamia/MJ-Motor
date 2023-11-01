#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#pragma comment (lib, "Assimp/libx86/assimp.lib")

#include "FBXLoader.h"
#include "Application.h"

vector <MeshStorer*>FBXLoader::meshesVector; 


void FBXLoader::Debug()
{
	// Stream log messages to Debug window
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);
}

void FBXLoader::FileLoader(const char* file_path)
{
	const aiScene* scene = aiImportFile(file_path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene -> HasMeshes())
	{
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			MeshStorer* ourMesh = new MeshStorer();

			// Copy vertices
			ourMesh->num_vertex = scene->mMeshes[i]->mNumVertices;
			ourMesh->vertex = new float[ourMesh->num_vertex * 3];
			memcpy(ourMesh->vertex, scene->mMeshes[i]->mVertices, sizeof(float) * ourMesh->num_vertex * 3);
			LOG("New mesh with %d vertex", ourMesh->num_vertex);
			//App->editor->console_log.AddLog(__FILE__, __LINE__, "New mesh with %d vertex", ourMesh->num_vertex);

			// Copy faces
			if (scene->mMeshes[i]->HasFaces())
			{
				ourMesh->num_index = scene->mMeshes[i]->mNumFaces * 3;
				ourMesh->index = new uint[ourMesh->num_index]; // assume each face is a triangle
				for (uint j = 0; j < scene->mMeshes[i]->mNumFaces; ++j)
				{
					if (scene->mMeshes[i]->mFaces[j].mNumIndices != 3) 
					{
						LOG("WARNING, geometry face with != 3 indices!");
						//App->editor->console_log.AddLog(__FILE__, __LINE__, "WARNING, geometry face with != 3 indices!");
					}
					else
					{
						memcpy(&ourMesh->index[j * 3], scene->mMeshes[i]->mFaces[j].mIndices, 3 * sizeof(uint));
						LOG("New mesh with %d index", ourMesh->num_index);
						//App->editor->console_log.AddLog(__FILE__, __LINE__, "New mesh with %d index", ourMesh->num_index);

					}
				}
				//Generate Buffer
				MeshStorer::GenerateMeshBuffer(ourMesh);

				//Store Mesh
				meshesVector.push_back(ourMesh);
			}

		}

		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		aiReleaseImport(scene);

		//ourMesh->RenderOneMesh();
	}
	else
		LOG("Error loading scene % s", file_path);
		//App->editor->console_log.AddLog(__FILE__, __LINE__, "Error loading scene % s", file_path);
}

void FBXLoader::RenderAll() 
{
	for (int i = 0; i < meshesVector.size(); i++) 
	{
		meshesVector[i]->RenderOneMesh();
	}
}

void MeshStorer::RenderOneMesh()
{
	/*
	glBegin(GL_TRIANGLES);

	
	for (int i = 0; i < num_index; i++)
	{
		glVertex3f(vertex[index[i] * 3], vertex[index[i] * 3 + 1], vertex[index[i] * 3 + 2]);
	}

	glEnd();
	*/


	// enable vertex arrays
	glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);
	glEnableClientState(GL_VERTEX_ARRAY);


	// before draw, specify vertex and index arrays with their offsets
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glDrawElements(GL_TRIANGLES,            // primitive type
		num_index,                      // # of indices
		GL_UNSIGNED_INT,         // data type
		(void*)0);               // ptr to indices

	glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
	
}


void MeshStorer::GenerateMeshBuffer(MeshStorer* ourMesh)
{
	//Mesh Buffer
	//Mesh Buffer Generator
	glGenBuffers(1, (GLuint*)&(ourMesh->id_vertex));
	glGenBuffers(1, (GLuint*)&(ourMesh->id_index));
	glBindBuffer(GL_ARRAY_BUFFER, ourMesh->id_vertex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ourMesh->id_index);

	//Mesh Buffer Data
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * ourMesh->num_vertex * 3, ourMesh->vertex, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * ourMesh->num_index, ourMesh->index, GL_STATIC_DRAW);

	//Close Mesh Buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}


void FBXLoader::CleanUp()
{
	// detach log stream
	aiDetachAllLogStreams();

	meshesVector.clear();
	
}
