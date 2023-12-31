#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	X = float3(1.0f, 0.0f, 0.0f);
	Y = float3(0.0f, 1.0f, 0.0f);
	Z = float3(0.0f, 0.0f, 1.0f);

	Position = float3(25.0f, 15.0f, 15.0f);
	Reference = float3(0.0f, 0.0f, 0.0f);
	ViewMatrix = IdentityMatrix;

	CalculateViewMatrix();
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	App->editor->console_log.AddLog(__FILE__, __LINE__, "Setting up the camera");

	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	// Implement a debug camera with keys and mouse
	// Now we can make this movememnt frame rate independant!

	float3 newPos(0,0,0);
	float speed = 10.0f * dt;
	if(App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 8.0f * dt;

	//Set camera in First Person
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		firstPersonView = !firstPersonView;

		if (firstPersonView)
		{
			LOG("First Person ON");
			App->editor->console_log.AddLog(__FILE__, __LINE__, "First Person ON");
			Reference = Position - (Position * 0.01);
		}
		else
		{
			LOG("First Person OFF");
			App->editor->console_log.AddLog(__FILE__, __LINE__, "First Person OFF");
			Reference = float3(0.0f, 0.0f, 0.0f);
		}
	}

	//if(App->input->GetMouseButton(SDL_MOUSEWHEEL) == KEY_REPEAT) newPos.z += speed;
	//if(App->input->GetMouseButton(SDL_MOUSEWHEEL) == KEY_REPEAT) newPos.z -= speed;

	if (App->input->GetMouseZ() != 0 && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT) newPos -= Z * (speed * 7) * App->input->GetMouseZ();

	// W controls
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos += Y * speed * 2;
	
	// S controls
	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos -= Y * speed * 2;

	// D controls
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * speed * 2;

	// A controls
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * speed * 2;

	Position += newPos;
	Reference += newPos;

	// Mouse motion ----------------

	if(App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		float Sensitivity = 0.35f * dt;

		Position -= Reference;

		if(dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;

			float3 rotationAxis(0.0f, 1.0f, 0.0f);
			Quat rotationQuat = Quat::RotateAxisAngle(rotationAxis, DeltaX);

			X = rotationQuat * X;
			Y = rotationQuat * Y;
			Z = rotationQuat * Z;
		}

		if(dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;

			Quat rotationQuat = Quat::RotateAxisAngle(X, DeltaY);

			Y = rotationQuat * Y;
			Z = rotationQuat * Z;

			if(Y.y < 0.0f)
			{
				Z = float3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				Y = Z.Cross(X);
			}
		}

		Position = Reference + Z * Position.Length();
	}

	LookAt(Reference);

	// Recalculate matrix -------------
	CalculateViewMatrix();

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const float3&Position, const float3&Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = (Position - Reference).Normalized();
	X = (float3(0.0f, 1.0f, 0.0f).Cross(Z)).Normalized();
	Y = Z.Cross(X);

	if(!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt( const float3&Spot)
{
	Reference = Spot;

	Z = (Position - Reference).Normalized();
	X = (float3(0.0f, 1.0f, 0.0f).Cross(Z)).Normalized();
	Y = Z.Cross(X);

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::Move(const float3&Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return ViewMatrix.M;
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	//todo: USE MATHGEOLIB here BEFORE 1st delivery! (TIP: Use MathGeoLib/Geometry/Frustum.h, view and projection matrices are managed internally.)
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -(X.Dot(Position)), -(Y.Dot(Position)), -(Z.Dot(Position)), 1.0f);
}

ImVec2 ModuleCamera3D::Normalize(float x, float y, float w, float h, ImVec2 point)
{
	ImVec2 normalizedPoint;

	normalizedPoint.x = (point.x - x) / ((x + w) - x);
	normalizedPoint.y = (point.y - y) / ((y + h) - y);

	return normalizedPoint;
}