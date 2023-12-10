#include "FBXLoader.h"
#include "Application.h"
#include "TexLoader.h"
#include "GameObjects.h"

#include "C_Mesh.h"
#include "C_Textures.h"


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

		GameObjects* FbxGameObject = new GameObjects(App->renderer3D->gameObjectList[0], file_path);

		GetNodeInfo(scene, scene->mRootNode, FbxGameObject);

		for (int i = 0; i < scene->mRootNode->mNumChildren; i++)
		{
			MeshStorer* ourMesh = new MeshStorer();

			aiMesh* ai_mesh = scene->mMeshes[scene->mRootNode->mChildren[i]->mMeshes[0]];

			// Copy vertices
			ourMesh->num_vertex = ai_mesh->mNumVertices;
			ourMesh->vertex = new float[ourMesh->num_vertex * VERTEX_FEATURES];
			LOG("New mesh with %d vertex", ourMesh->num_vertex);
			App->editor->console_log.AddLog(__FILE__, __LINE__, "New mesh with %d vertex", ourMesh->num_vertex);
					
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
						App->editor->console_log.AddLog(__FILE__, __LINE__, "WARNING, geometry face with != 3 indices!");
					}
					else
					{
						memcpy(&ourMesh->index[j * 3], ai_mesh->mFaces[j].mIndices, 3 * sizeof(uint));
					}
				}
				LOG("New mesh with %d index", ourMesh->num_index);
				App->editor->console_log.AddLog(__FILE__, __LINE__, "New mesh with %d index", ourMesh->num_index);
			}

			//Default building textures
			ourMesh->building_textures[0] = "Assets/street/Building_V02_C.png";
			ourMesh->building_textures[2] = "Assets/street/Building_V01_C.png";
			ourMesh->building_textures[1] = "Assets/street/building05-_c.png";
			ourMesh->building_textures[3] = "Assets/street/building03_c.png";
			ourMesh->building_textures[4] = "Assets/street/building_025_c.png";
			ourMesh->building_textures[5] = "Assets/street/building_016_c.png";
			ourMesh->building_textures[6] = "Assets/street/building-06_-c.png";
			ourMesh->building_textures[7] = "Assets/street/building-06_-c-.png";
			ourMesh->building_textures[8] = "Assets/street/building-01_c.png";

			//Game Objects
			ourMesh->ID = App->renderer3D->CreateGameObject(FbxGameObject, scene->mRootNode->mChildren[i]->mName.C_Str());

			//Mesh Components
			dynamic_cast<C_Mesh*>(App->renderer3D->gameObjectList[ourMesh->ID]->CreateComponent(Components::TYPE::MESH))->SetMesh(ourMesh, scene->mRootNode->mChildren[i]->mName.C_Str());
			GetNodeInfo(scene, scene->mRootNode->mChildren[i], App->renderer3D->gameObjectList[ourMesh->ID]);

			//Texture Components
			if (App->renderer3D->gameObjectList[ourMesh->ID]->name == "City_building_041" || App->renderer3D->gameObjectList[ourMesh->ID]->name == "City_building_001" || App->renderer3D->gameObjectList[ourMesh->ID]->name == "City_building_017")
			{
				ourMesh->id_texture = TexLoader::LoadTexture(ourMesh->building_textures[0]);
				dynamic_cast<C_Textures*>(App->renderer3D->gameObjectList[ourMesh->ID]->CreateComponent(Components::TYPE::TEXTURE))->SetTexture(ourMesh->building_textures[0]);
			}
			else if (App->renderer3D->gameObjectList[ourMesh->ID]->name == "City_building_038" || App->renderer3D->gameObjectList[ourMesh->ID]->name == "City_building_037" || App->renderer3D->gameObjectList[ourMesh->ID]->name == "City_building_034" || App->renderer3D->gameObjectList[ourMesh->ID]->name == "City_building_026" || App->renderer3D->gameObjectList[ourMesh->ID]->name == "City_building_022")
			{
				ourMesh->id_texture = TexLoader::LoadTexture(ourMesh->building_textures[1]);
				dynamic_cast<C_Textures*>(App->renderer3D->gameObjectList[ourMesh->ID]->CreateComponent(Components::TYPE::TEXTURE))->SetTexture(ourMesh->building_textures[1]);
			}
			else if (App->renderer3D->gameObjectList[ourMesh->ID]->name == "City_building_040" || App->renderer3D->gameObjectList[ourMesh->ID]->name == "City_building_039" || App->renderer3D->gameObjectList[ourMesh->ID]->name == "City_building_035" || App->renderer3D->gameObjectList[ourMesh->ID]->name == "City_building_031" || App->renderer3D->gameObjectList[ourMesh->ID]->name == "City_building_030" || App->renderer3D->gameObjectList[ourMesh->ID]->name == "City_building_028")
			{
				ourMesh->id_texture = TexLoader::LoadTexture(ourMesh->building_textures[2]);
				dynamic_cast<C_Textures*>(App->renderer3D->gameObjectList[ourMesh->ID]->CreateComponent(Components::TYPE::TEXTURE))->SetTexture(ourMesh->building_textures[2]);
			}
			else if (App->renderer3D->gameObjectList[ourMesh->ID]->name == "City_building_032" || App->renderer3D->gameObjectList[ourMesh->ID]->name == "City_building_014" || App->renderer3D->gameObjectList[ourMesh->ID]->name == "City_building_004")
			{
				ourMesh->id_texture = TexLoader::LoadTexture(ourMesh->building_textures[3]);
				dynamic_cast<C_Textures*>(App->renderer3D->gameObjectList[ourMesh->ID]->CreateComponent(Components::TYPE::TEXTURE))->SetTexture(ourMesh->building_textures[3]);
			}
			else if (App->renderer3D->gameObjectList[ourMesh->ID]->name == "City_building_033" || App->renderer3D->gameObjectList[ourMesh->ID]->name == "City_building_013" || App->renderer3D->gameObjectList[ourMesh->ID]->name == "City_building_036")
			{
				ourMesh->id_texture = TexLoader::LoadTexture(ourMesh->building_textures[7]);
				dynamic_cast<C_Textures*>(App->renderer3D->gameObjectList[ourMesh->ID]->CreateComponent(Components::TYPE::TEXTURE))->SetTexture(ourMesh->building_textures[7]);
			}
			else if (App->renderer3D->gameObjectList[ourMesh->ID]->name == "City_building_024" || App->renderer3D->gameObjectList[ourMesh->ID]->name == "City_building_008" || App->renderer3D->gameObjectList[ourMesh->ID]->name == "City_building_036")
			{
				ourMesh->id_texture = TexLoader::LoadTexture(ourMesh->building_textures[4]);
				dynamic_cast<C_Textures*>(App->renderer3D->gameObjectList[ourMesh->ID]->CreateComponent(Components::TYPE::TEXTURE))->SetTexture(ourMesh->building_textures[4]);
			}
			else if (App->renderer3D->gameObjectList[ourMesh->ID]->name == "City_building_023" || App->renderer3D->gameObjectList[ourMesh->ID]->name == "City_building_011" || App->renderer3D->gameObjectList[ourMesh->ID]->name == "City_building_007")
			{
				ourMesh->id_texture = TexLoader::LoadTexture(ourMesh->building_textures[5]);
				dynamic_cast<C_Textures*>(App->renderer3D->gameObjectList[ourMesh->ID]->CreateComponent(Components::TYPE::TEXTURE))->SetTexture(ourMesh->building_textures[5]);
			}


			ourMesh->GenerateLocalBoundingBox();

			FBXLoader::SetUpMesh(ourMesh);  


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
	{
		LOG("Error loading scene % s", file_path);
		App->editor->console_log.AddLog(__FILE__, __LINE__, "Error loading scene % s", file_path);
	}
}

void FBXLoader::RenderAll() 
{
	for (int i = 0; i < meshesVector.size(); i++) 
	{
		//meshesVector[i]->RenderOneMesh();
	}
}

void MeshStorer::RenderOneMesh(const GLfloat* globalTransform, uint texID)
{
	RenderAABB();

	//Enable vertex arrays
	glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);

	glBindTexture(GL_TEXTURE_2D, texID);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	//Before draw, specify vertex and index arrays with their offsets
	glVertexPointer(3, GL_FLOAT, sizeof(float) * VERTEX_FEATURES, NULL);
	glTexCoordPointer(2, GL_FLOAT, sizeof(float) * VERTEX_FEATURES, (void*)(3 * sizeof(float)));
	glNormalPointer(GL_FLOAT, sizeof(float) * VERTEX_FEATURES, NULL);

	glPushMatrix();
	glMultMatrixf(globalTransform);

	glDrawElements(GL_TRIANGLES,            // primitive type
		num_index,                      // # of indices
		GL_UNSIGNED_INT,         // data type
		(void*)0);

	glPopMatrix();

	glClientActiveTexture(GL_TEXTURE0);

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


void MeshStorer::GenerateGlobalBoundingBox()
{
	globalOBB = localAABB;
	//globalOBB.Transform(GameObjects::gameObjectList[ID]->transform->GetGlobalMatrix());

	// Generate global AABB
	globalAABB.SetNegativeInfinity();
	globalAABB.Enclose(globalOBB);
}

void MeshStorer::GenerateLocalBoundingBox()
{
	float* vertex_positions = new float[num_vertex * 3];
	for (size_t i = 0; i < num_vertex; i++)
	{
		vertex_positions[i * 3] = vertex[i * VERTEX_FEATURES];
		vertex_positions[i * 3 + 1] = vertex[i * VERTEX_FEATURES + 1];
		vertex_positions[i * 3 + 2] = vertex[i * VERTEX_FEATURES + 2];
	}

	localAABB.SetNegativeInfinity();
	localAABB.Enclose((float3*)vertex_positions, num_vertex);
	localAABB_init = true;
}

void MeshStorer::RenderAABB()
{
	float3 vertexAABB[8];

	globalAABB.GetCornerPoints(vertexAABB);


	glBegin(GL_LINES);
	glVertex3f(vertexAABB[0].x, vertexAABB[0].y, vertexAABB[0].z);
	glVertex3f(vertexAABB[1].x, vertexAABB[1].y, vertexAABB[1].z);
	glVertex3f(vertexAABB[0].x, vertexAABB[0].y, vertexAABB[0].z);
	glVertex3f(vertexAABB[4].x, vertexAABB[4].y, vertexAABB[4].z);
	glVertex3f(vertexAABB[1].x, vertexAABB[1].y, vertexAABB[1].z);
	glVertex3f(vertexAABB[5].x, vertexAABB[5].y, vertexAABB[5].z);
	glVertex3f(vertexAABB[4].x, vertexAABB[4].y, vertexAABB[4].z);
	glVertex3f(vertexAABB[5].x, vertexAABB[5].y, vertexAABB[5].z);
	
	glVertex3f(vertexAABB[2].x, vertexAABB[2].y, vertexAABB[2].z);
	glVertex3f(vertexAABB[3].x, vertexAABB[3].y, vertexAABB[3].z);
	glVertex3f(vertexAABB[2].x, vertexAABB[2].y, vertexAABB[2].z);
	glVertex3f(vertexAABB[6].x, vertexAABB[6].y, vertexAABB[6].z);
	glVertex3f(vertexAABB[6].x, vertexAABB[6].y, vertexAABB[6].z);
	glVertex3f(vertexAABB[7].x, vertexAABB[7].y, vertexAABB[7].z);
	glVertex3f(vertexAABB[3].x, vertexAABB[3].y, vertexAABB[3].z);
	glVertex3f(vertexAABB[7].x, vertexAABB[7].y, vertexAABB[7].z);
	
	glVertex3f(vertexAABB[0].x, vertexAABB[0].y, vertexAABB[0].z);
	glVertex3f(vertexAABB[2].x, vertexAABB[2].y, vertexAABB[2].z);
	glVertex3f(vertexAABB[1].x, vertexAABB[1].y, vertexAABB[1].z);
	glVertex3f(vertexAABB[3].x, vertexAABB[3].y, vertexAABB[3].z);
	glVertex3f(vertexAABB[4].x, vertexAABB[4].y, vertexAABB[4].z);
	glVertex3f(vertexAABB[6].x, vertexAABB[6].y, vertexAABB[6].z);
	glVertex3f(vertexAABB[5].x, vertexAABB[5].y, vertexAABB[5].z);
	glVertex3f(vertexAABB[7].x, vertexAABB[7].y, vertexAABB[7].z);
	glEnd();
}

void FBXLoader::GetNodeInfo(const aiScene* rootScene, aiNode* rootNode, GameObjects* go)
{
	aiVector3D translation, scaling;
	aiQuaternion quatRot;
	rootNode->mTransformation.Decompose(scaling, quatRot, translation);

	float3 pos(translation.x, translation.y, translation.z);
	float3 scale(scaling.x, scaling.y, scaling.z);
	Quat rot(quatRot.x, quatRot.y, quatRot.z, quatRot.w);

	dynamic_cast<C_Transform*>(go->GetSingleComponent(Components::TYPE::TRANSFORM))->SetTransform(pos / 100, rot, scale / 100);

	// We make it recursive for its children
	if (rootNode->mNumChildren > 0)
	{
		for (int n = 0; n < rootNode->mNumChildren; n++)
		{
			GetNodeInfo(rootScene, rootNode->mChildren[n], go);
		}
	}
}

void FBXLoader::SetUpMesh(MeshStorer* mesh)
{
	//Create vertices and indices buffers
	glGenBuffers(1, (GLuint*)&(mesh->id_vertex));
	glGenBuffers(1, (GLuint*)&(mesh->id_index));

	//Bind and fill buffers
	glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->num_vertex * 5, mesh->vertex, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh->num_index, mesh->index, GL_STATIC_DRAW);

	//Unbind buffers
	glDisableClientState(GL_VERTEX_ARRAY);

	//Add mesh to meshes vector
	meshesVector.push_back(mesh);
}

void FBXLoader::CleanUp()
{
	// detach log stream
	aiDetachAllLogStreams();

	meshesVector.clear();
}
