#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl2.h"
#include "ImGui/imgui_impl_opengl3.h"

#include "Glew/include/glew.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "glu32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "Glew/libx86/glew32.lib")

#include "ModuleEditor.h";
#include <DevIL/include/ilut.h>
#include <DevIL/include/ilu.h>
#include <DevIL/include/il.h>

#include "GameObjects.h"


//===================================================
//MathGeoLib make errors, it stays comented for now
//===================================================

//#ifdef _DEBUG
//#pragma comment (lib, "MathGeoLib/libx86/Debug/MathGeoLib.lib") /* link Microsoft OpenGL lib   */
//#else
//#pragma comment (lib, "MathGeoLib/libx86/Release/MathGeoLib.lib") /* link Microsoft OpenGL lib   */
//#endif // _DEBUG


ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init()
{		
	LOG("Creating 3D Renderer context");
	App->editor->console_log.AddLog(__FILE__, __LINE__, "Creating 3D Renderer context");
	bool ret = true;
	
	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if(context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		App->editor->console_log.AddLog(__FILE__, __LINE__, "OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	//Glew Initialitation
	App->editor->console_log.AddLog(__FILE__, __LINE__, "Initializing Glew");

	GLenum err = glewInit();
	// … check for errors
	LOG("Using Glew %s", glewGetString(GLEW_VERSION));
	// Should be 2.0

	LOG("Vendor: %s", glGetString(GL_VENDOR));
	LOG("Renderer: %s", glGetString(GL_RENDERER));
	LOG("OpenGL version supported %s", glGetString(GL_VERSION));
	LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	SDL_GL_MakeCurrent(App->window->window, context);

	if(ret == true)
	{
		//Use Vsync
		if(VSYNC && SDL_GL_SetSwapInterval(1) < 0)
		{ 
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
			App->editor->console_log.AddLog(__FILE__, __LINE__, "Warning: Unable to set VSync!");
		}

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			App->editor->console_log.AddLog(__FILE__, __LINE__, "Error initializing OpenGL!");
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			App->editor->console_log.AddLog(__FILE__, __LINE__, "Error initializing OpenGL!");
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			App->editor->console_log.AddLog(__FILE__, __LINE__, "Error initializing OpenGL!");
			ret = false;
		}
		
		GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		lights[0].Active(true);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
	}

	// Projection matrix for
	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);

	Grid.axis = true;

	ImGui_ImplSDL2_InitForOpenGL(App->window->window, context);
	ImGui_ImplOpenGL3_Init("#version 130");

	//DevIL Initialization
	LOG("Initialaizing il");
	App->editor->console_log.AddLog(__FILE__, __LINE__, "Initialaizing il");
	ilInit();

	LOG("Initialaizing ilu");
	App->editor->console_log.AddLog(__FILE__, __LINE__, "Initialaizing ilu");
	iluInit();

	LOG("Initialaizing ilut");
	App->editor->console_log.AddLog(__FILE__, __LINE__, "Initialaizing ilut");
	ilutInit();

	//==============================================================================================================================================================

	float3 pos(0, 0, 0);
	float3 scale(1, 1, 1);
	Quat rot(0, 0, 0, 0);

	Root = new GameObjects(NULL, "World");

	GameObjects::gameObjectList[0] = Root;
	dynamic_cast<C_Transform*>(Root->GetSingleComponent(Components::TYPE::TRANSFORM))->SetTransform(pos, rot, scale);

	//==============================================================================================================================================================

	//Frame Buffer
	GenerateFrameBuffer();

	//Render Mesh with File path
	FBXLoader::FileLoader(file_path);

	//Render Mesh with File drop
	if (App->input->fileDrop == true)
	{
		FBXLoader::FileLoader(App->input->droppedDir);
	}

	//==============================================================================================================================================================
	
	//FBXLoader::FileLoader(file_path, &myMesh);

	return ret;
}

bool ModuleRenderer3D::Start()
{
	return false;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());

	// light 0 on cam pos
	lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	//Clear Frame Buffer
	glBindFramebuffer(GL_FRAMEBUFFER, texColorBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	return UPDATE_CONTINUE;
}

update_status ModuleRenderer3D::Update(float dt) {
	
	Grid.Render();

	for (auto& gameObject : GameObjects::gameObjectList)
	{
		gameObject.second->Update();
	}

	//FBXLoader::RenderAll();

	//Show Checkers Cube with CheckBox
	if (App->editor->showCubeCheckers)
	{
		// Checkers Texture
		// Load the imageData array with checkerboad pattern
		LoadTextureImageData();

		// Create texture from image data
		CreateTextureImageData();

		//Translate the cubes position
		glTranslatef(-3, 1, 0);
		glPopMatrix();

		//Render Checkers Cube
		CheckersCube();
	}

	//Show VSYNC with CheckBox
	if (App->editor->activateVSYNC)
	{
		SDL_GL_SetSwapInterval(1);
	}
	else
	{
		SDL_GL_SetSwapInterval(0);
	}

	//Show Depth Test with CheckBox
	if (App->editor->activateDepthTest)
	{
		glDisable(GL_DEPTH_TEST);
	}
	else
	{
		glEnable(GL_DEPTH_TEST);
	}

	//Show Cull Face with CheckBox
	if (App->editor->activateCullFace)
	{
		glEnable(GL_CULL_FACE);
	}
	else
	{
		glDisable(GL_CULL_FACE);
	}

	//Show Lighting with CheckBox
	if (App->editor->activateLighting)
	{
		glDisable(GL_LIGHTING);
	}
	else
	{
		glEnable(GL_LIGHTING);
	}

	//Show Color Material with CheckBox
	if (App->editor->activateColorMaterial)
	{
		glEnable(GL_COLOR_MATERIAL);
	}
	else
	{
		glDisable(GL_COLOR_MATERIAL);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return UPDATE_CONTINUE;
}


// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{	
	App->editor->DrawEditor();

	SDL_GL_SwapWindow(App->window->window);

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	if (vboId != 0)
	{
		glDeleteBuffers(1, &vboId);
		vboId = 0;
	}

	//Frame Buffer Elimination
	glDeleteFramebuffers(1, &frameBuffer);

	//Render Buffer Object Elimination
	glDeleteFramebuffers(1, &rboDepthStencil);
	FBXLoader::CleanUp();
	SDL_GL_DeleteContext(context);

	return true;
}


void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//todo: USE MATHGEOLIB here BEFORE 1st delivery! (TIP: Use MathGeoLib/Geometry/Frustum.h, view and projection matrices are managed internally.)
	ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(ProjectionMatrix.M);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void ModuleRenderer3D::GenerateCubeBuffer()
{
	//Cube Buffer Creation
	glGenBuffers(1, &vboId); //it's like the buffer ID
	glBindBuffer(GL_ARRAY_BUFFER, vboId); //indicator that we are about to work with the buffer in the upper line

	//Cube Buffer Data
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(normals) + sizeof(colors), 0, GL_STATIC_DRAW); //what the buffer will work with. Data.
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);                             // copy vertices starting from 0 offest, 0 as array start
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(normals), normals);                // copy normals after vertices, We start with the vertices size, when in the last buffer we started from 0
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(normals), sizeof(colors), colors);  // copy colours after normals, same as beffore adding normals, as it's the space we occupied

	//Close Cube Buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &iboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


void ModuleRenderer3D::RenderCubeBuffer()
{
	// bind VBOs with IDs and set the buffer offsets of the bound VBOs
	// When buffer object is bound with its ID, all pointers in gl*Pointer()
	// are treated as offset instead of real pointer.


	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);

	// enable vertex arrays
	glEnableClientState(GL_NORMAL_ARRAY);

	glEnableClientState(GL_COLOR_ARRAY);

	glEnableClientState(GL_VERTEX_ARRAY);


	// before draw, specify vertex and index arrays with their offsets
	glNormalPointer(GL_FLOAT, 0, (void*)sizeof(vertices));
	glColorPointer(3, GL_FLOAT, 0, (void*)(sizeof(vertices) + sizeof(normals)));
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glDrawElements(GL_TRIANGLES,            // primitive type
		36,                      // # of indices
		GL_UNSIGNED_INT,         // data type
		(void*)0);               // ptr to indices

	glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	// it is good idea to release VBOs with ID 0 after use.
	// Once bound with 0, all pointers in gl*Pointer() behave as real
	// pointer, so, normal vertex array operations are re-activated
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


void ModuleRenderer3D::GenerateFrameBuffer()
{
	//Frame Buffer Creation
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	//Frame Buffer Attachments
	//Frame Buffer Texture
	glGenTextures(1, &texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);

	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGB, 1280, 700, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(
		GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

	//Render Buffer
	//Render Buffer Creation
	glGenRenderbuffers(1, &rboDepthStencil);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepthStencil);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1280, 700);
	glBindRenderbuffer(GL_FRAMEBUFFER, 0);

	glFramebufferRenderbuffer(
		GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboDepthStencil);

	//Using Frame Buffer
	//glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

}


void ModuleRenderer3D::CheckersCube()
{
	glBegin(GL_QUADS);

	// Front Face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	// Back Face
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	// Top Face
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	// Bottom Face
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	// Right face
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	// Left Face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glEnd();

}


void ModuleRenderer3D::LoadTextureImageData() 
{
	int value;
	for (int row = 0; row < IMAGE_ROWS; row++) {
		for (int col = 0; col < IMAGE_COLS; col++) {
			// Each cell is 8x8, value is 0 or 255 (black or white)
			value = (((row & 0x8) == 0) ^ ((col & 0x8) == 0)) * 255;
			imageData[row][col][0] = (GLubyte)value;
			imageData[row][col][1] = (GLubyte)value;
			imageData[row][col][2] = (GLubyte)value;
		}
	}
}

void ModuleRenderer3D::CreateTextureImageData()
{
	glTexImage2D(GL_TEXTURE_2D, 0, 3, IMAGE_COLS, IMAGE_ROWS, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glEnable(GL_TEXTURE_2D);  // Enable 2D texture 
}

void ModuleRenderer3D::DirectModeCube() {
	
	if (App->editor->showCubeDirectMode)
	{		
		//creación de triangulos
		glLineWidth(2.0f);
		glBegin(GL_TRIANGLES);

		//primera cara
		glVertex3d(0, 0, 0); glVertex3d(1, 1, 0); glVertex3d(1, 0, 0); //Make triangles this way to easy distinction
		glVertex3d(0, 0, 0); glVertex3d(0, 1, 0); glVertex3d(1, 1, 0);
		//segunda cara
		glVertex3d(0, 0, 0); glVertex3d(0, 1, 1); glVertex3d(0, 1, 0);
		glVertex3d(0, 0, 1); glVertex3d(0, 1, 1); glVertex3d(0, 0, 0);
		//tercera cara
		glVertex3d(0, 0, 0); glVertex3d(1, 0, 0); glVertex3d(1, 0, 1);
		glVertex3d(0, 0, 1); glVertex3d(0, 0, 0); glVertex3d(1, 0, 1);
		//quarta cara
		glVertex3d(0, 1, 0); glVertex3d(1, 1, 1); glVertex3d(1, 1, 0);
		glVertex3d(0, 1, 1); glVertex3d(1, 1, 1); glVertex3d(0, 1, 0);
		//quinta cara
		glVertex3d(1, 0, 0); glVertex3d(1, 1, 1); glVertex3d(1, 0, 1);
		glVertex3d(1, 0, 0); glVertex3d(1, 1, 0); glVertex3d(1, 1, 1);
		//sexta cara
		glVertex3d(1, 0, 1); glVertex3d(0, 1, 1); glVertex3d(0, 0, 1);
		glVertex3d(1, 0, 1); glVertex3d(1, 1, 1); glVertex3d(0, 1, 1);
		glEnd();
		glLineWidth(1.0f);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);

	}
}