#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#pragma comment (lib, "Assimp/libx86/assimp.lib")

#include "FBXLoader.h"
#include "Application.h"
#include "TexLoader.h"

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
		for (int i = 0; i < scene->mRootNode->mNumChildren; i++)
		{
			MeshStorer* ourMesh = new MeshStorer();

			aiMesh* ai_mesh = scene->mMeshes[scene->mRootNode->mChildren[i]->mMeshes[0]];

			// Copy vertices
			ourMesh->num_vertex = ai_mesh->mNumVertices;
			ourMesh->vertex = new float[ourMesh->num_vertex * VERTEX_FEATURES];
			LOG("New mesh with %d vertex", ourMesh->num_vertex);
					
			for (int v = 0; v < ourMesh->num_vertex; v++) {

				ourMesh->vertex[v * VERTEX_FEATURES] = ai_mesh->mVertices[v].x;
				ourMesh->vertex[v * VERTEX_FEATURES + 1] = ai_mesh->mVertices[v].y;
				ourMesh->vertex[v * VERTEX_FEATURES + 2] = ai_mesh->mVertices[v].z;
			
				if (ai_mesh->HasTextureCoords(0))
				{
					ourMesh->vertex[v * VERTEX_FEATURES + 3] = ai_mesh->mTextureCoords[0][v].x;
					ourMesh->vertex[v * VERTEX_FEATURES + 4] = ai_mesh->mTextureCoords[0][v].y;
				}
			}
		
			// Copy faces
			if (ai_mesh->HasFaces())
			{
				ourMesh->num_index = ai_mesh->mNumFaces * 3;
				ourMesh->index = new uint[ourMesh->num_index]; // assume each face is a triangle
				for (uint j = 0; j < ai_mesh->mNumFaces; ++j)
				{
					if (ai_mesh->mFaces[j].mNumIndices != 3)
					{
						LOG("WARNING, geometry face with != 3 indices!");
					}
					else
					{
						memcpy(&ourMesh->index[j * 3], ai_mesh->mFaces[j].mIndices, 3 * sizeof(uint));
						LOG("New mesh with %d index", ourMesh->num_index);

					}
				}
			}

			//Load Texture
			ourMesh->id_texture = TexLoader::LoadTexture(ourMesh->bakerHouseTexPath);

			//Generate Buffer
			FBXLoader::GenerateMeshBuffer(ourMesh);

			//Store Mesh
			meshesVector.push_back(ourMesh);

		}

		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		aiReleaseImport(scene);

		//ourMesh->RenderOneMesh();
	}
	else
		LOG("Error loading scene % s", file_path);
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
	//Enable vertex arrays
	glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);

	glBindTexture(GL_TEXTURE_2D, id_texture);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	//Before draw, specify vertex and index arrays with their offsets
	glVertexPointer(3, GL_FLOAT, sizeof(float) * VERTEX_FEATURES, NULL);
	glTexCoordPointer(2, GL_FLOAT, sizeof(float) * VERTEX_FEATURES, (void*)(3 * sizeof(float)));
	glNormalPointer(GL_FLOAT, sizeof(float) * VERTEX_FEATURES, NULL);

	glDrawElements(GL_TRIANGLES,            // primitive type
		num_index,                      // # of indices
		GL_UNSIGNED_INT,         // data type
		(void*)0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
	glDisable(GL_TEXTURE_COORD_ARRAY);
}


void FBXLoader::GenerateMeshBuffer(MeshStorer* ourMesh)
{
	//Mesh Buffer
	//Mesh Buffer Generator
	glGenBuffers(1, (GLuint*)&(ourMesh->id_vertex));
	glGenBuffers(1, (GLuint*)&(ourMesh->id_index));
	glBindBuffer(GL_ARRAY_BUFFER, ourMesh->id_vertex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ourMesh->id_index);

	//Mesh Buffer Data
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * ourMesh->num_vertex * VERTEX_FEATURES, ourMesh->vertex, GL_STATIC_DRAW);
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
