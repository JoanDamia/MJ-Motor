#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"
#include <SDL/include/SDL_opengl.h>

#include "ModuleRenderer3D.h"
#include "GameObjects.h"

ModuleEditor::ModuleEditor(Application* app, bool start_enabled) : Module(app, start_enabled)
{


}

ModuleEditor::~ModuleEditor()
{}

bool ModuleEditor::Init() {

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui::StyleColorsClassic();

	return false;
}

//------------------------------------------------------------------
void ModuleEditor::DrawEditor() {

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

// -----------------------------------------------------------------
bool ModuleEditor::Start()
{
	LOG("Setting up the editor");
    App->editor->console_log.AddLog(__FILE__, __LINE__, "Setting up the editor");

	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
update_status ModuleEditor::Update(float dt)
{
    float FPS = floorf(App->GetFrameRate());
    float MS = (App->GetDt() * 1000.f);

    PushLog(&fps_log, FPS);
    PushLog(&ms_log, MS);

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
update_status ModuleEditor::PostUpdate(float dt)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    //ImGui::ShowDemoWindow();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    {
        static int counter = 0;

        //==============================================================================================================================================================

        //Docking Base Window
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
        ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_MenuBar;

        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGui::Begin("BackGround Window", nullptr, windowFlags);

        if (ImGui::BeginMenuBar())
        {
            // File menu
            if (ImGui::BeginMenu("File"))
            {
                ImGui::Text("\n");

                if (ImGui::MenuItem(" Clear Meshes"))
                FBXLoader::meshesVector.clear();

                ImGui::Text("\n");

                ImGui::EndMenu();
            }

            // Window menu
            if (ImGui::BeginMenu("Window"))
            {
                ImGui::Text("\n");

                if (ImGui::MenuItem(" Scene"))
                    showScene = !showScene;

                ImGui::Text("\n");

                if (ImGui::MenuItem(" Inspector"))
                    showInspector = !showInspector;

                ImGui::Text("\n");

                if (ImGui::MenuItem(" Console"))
                    showConsole = !showConsole;

                ImGui::Text("\n");

                if (ImGui::MenuItem(" Hierarchy"))
                    showHierarchy = !showHierarchy;

                ImGui::Text("\n");

                ImGui::EndMenu();
            }

            // About menu
            if (ImGui::BeginMenu("About"))
            {
                ImGui::Text("\n");

                if (ImGui::Button("Our GitHub")) {
                    ::ShellExecuteA(NULL, "open", "https://github.com/JoanDamia/MJ-Motor", NULL, NULL, SW_SHOWDEFAULT);
                }

                ImGui::Text("\n");

                if (ImGui::MenuItem(" Credits"))
                    showCredits = !showCredits;

                ImGui::Text("\n");

                if (ImGui::MenuItem(" License"))
                    showLicense = !showLicense;

                ImGui::Text("\n");

                ImGui::EndMenu();
            }

            // Exit menu
            if (ImGui::BeginMenu("Exit"))
            {
                ImGui::Text("\n");

                if (ImGui::MenuItem(" Close Motor"))
                    return UPDATE_STOP;

                ImGui::Text("\n");

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        ImGui::PopStyleVar(3);

        ImGuiID dockSpaceId = ImGui::GetID("BackGroundWindowDockSpace");

        ImGui::DockSpace(dockSpaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
        ImGui::End();

        //==============================================================================================================================================================

        //ImGui Render Window
        if (showScene)
        {
            ImGuiRenderWindow();
        }
       
        //ImGui Inspector Window
        if (showInspector)
        {
            ImGuiInspectorWindow();
        }

        //ImGui Console Window
        if (showConsole)
        {
            console_log.Draw("Console");
        }

        //Imgui Credits Window
        if (showCredits)
        {
            ImGuiCreditsWindow();
        }

        //Imgui License Window
        if (showLicense)
        {
            ImGuiLicenseWindow();
        }

        //Imgui Hierarchy Window
        if (showHierarchy)
        {
            ImGuiHierarchyWindow();
        }
    }

    // 3. Show another simple window.
    
    // Rendering
    ImGui::Render();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
bool ModuleEditor::CleanUp()
{
	LOG("Cleaning editor");

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return true;
}

// -----------------------------------------------------------------
void ModuleEditor::PushLog(std::vector<float>* Log, float toPush)
{
    if (Log->size() > 100)
    {
        Log->erase(Log->begin());
        Log->push_back(toPush);
    }
    else
    {
        Log->push_back(toPush);
    }
}
    

//ImGui Windows
// -----------------------------------------------------------------        
void ModuleEditor::ImGuiRenderWindow() 
{
    ImGui::Begin("Scene", NULL, ImGuiWindowFlags_NoBringToFrontOnFocus);
    //ImVec2 wsize = ImGui::GetWindowSize();
    float width = ImGui::GetContentRegionAvail().x;
    float height = width * (9.0f / 16.0f);
    ImGui::Image((ImTextureID)App->renderer3D->texColorBuffer, ImVec2(width, height), ImVec2(0, 1), ImVec2(1, 0));
    ImGui::End();
}

// -----------------------------------------------------------------        
void ModuleEditor::ImGuiInspectorWindow()
{
    ImGui::Begin("Inspector", NULL, ImGuiWindowFlags_AlwaysAutoResize);

    //ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

    //ImGui::Checkbox("CubeDirectMode", &showCubeDirectMode);

    ImGui::Text("Basic Shapes");

    if (ImGui::Button("Cube"))
    {
        FBXLoader::FileLoader(cube_file_path);
    }
    if (ImGui::Button("Sphere"))
    {
        FBXLoader::FileLoader(sphere_file_path);
    }
    if (ImGui::Button("Cone"))
    {
        FBXLoader::FileLoader(cone_file_path);
    }
    if (ImGui::Button("Cylinder"))
    {
        FBXLoader::FileLoader(cylinder_file_path);
    }

    ImGui::Checkbox("Checkers Cube", &showCubeCheckers);

    ImGui::Text("\n");

    if (ImGui::CollapsingHeader("Modules", NULL, ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Text("Renderer:");
        ImGui::Checkbox("VSYNC", &activateVSYNC);
        ImGui::Checkbox("Depth Test", &activateDepthTest);
        ImGui::Checkbox("Cull Face", &activateCullFace);
        ImGui::Checkbox("Lightning", &activateLighting);
        ImGui::Checkbox("Color Material", &activateColorMaterial);

        ImGui::Text("\n");

        ImGui::Text("Input:");
        ImGui::Checkbox("Drag&Drop", &activateDragAndDrop);

        ImGui::Text("\n");
    }

    if (ImGui::CollapsingHeader("Information"))
    {
        //Software versions
        ImGui::Text("Software Versions");
        ImGui::Text("SDL: 2.0");
        ImGui::Text("OpenGL: 3.1.0");
        ImGui::Text("Glew: 2.1.0");
        ImGui::Text("ImGui: 1.89.9");
        ImGui::Text("Assimp: 5.3.1");
        ImGui::Text("DevIl: 7.0");
        ImGui::Text("MathGeoLib: 2.0");

        ImGui::Text("\n");

        //Hardware Detection
        ImGui::Text("System");
        ImGui::Text("CPU cores:%d", SDL_GetCPUCount());
        ImGui::Text("RAM:%dGB", (SDL_GetSystemRAM()) / 1000);

        ImGui::Text("\n");

        //FPS Graph
        ImGui::Text("FPS Graph");
        char title[25];
        sprintf_s(title, 25, "Framerate %1.1f", fps_log[fps_log.size() - 1]);
        ImGui::PlotHistogram("##framerate", &fps_log[0], fps_log.size(), 0, title, 0.0f, 100.0f, ImVec2(210, 70));
        sprintf_s(title, 25, "Milliseconds %0.1f", ms_log[ms_log.size() - 1]);
        ImGui::PlotHistogram("##millisconds", &ms_log[0], ms_log.size(), 0, title, 0.0f, 40.0f, ImVec2(210, 70));
    }

    ImGui::End();
}

// -----------------------------------------------------------------        
void ModuleEditor::ImGuiCreditsWindow()
{
    ImGui::Begin("Credits", NULL, ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::Text("\n");

    ImGui::Text(" MJ ENGINE");
    ImGui::Text("\n");
    ImGui::TextDisabled(" By Miguel Rodriguez and Joan Damia\n\n");

    ImGui::Text(" Engine made by two CITM students, capable of multible features like \n");
    ImGui::Text(" fbx rendering, drag and drop, camera movement and much more to come.\n\n");

    ImGui::Text("\n\n");

    ImGui::Text(" Features: \n\n");

    ImGui::BulletText(" Fbx loader (Baker_house loaded at the start)\n");
    ImGui::BulletText(" Fbx Drag and Drop\n");
    ImGui::BulletText(" Texture loader\n");
    ImGui::BulletText(" ImGui menus and console Window\n");
    ImGui::BulletText(" Unity-like camera\n");

    ImGui::Text("\n\n");

    ImGui::Text(" Controls: \n\n");

    ImGui::BulletText(" Camera WASD movement while pressing right click\n");
    ImGui::BulletText(" Alt + Left Click for mouse based view\n");
    ImGui::BulletText(" SPACE for first person view\n");
    ImGui::BulletText(" Alt + MouseWheel for zoom\n");
  
    ImGui::Text("\n");

    ImGui::End();
}

// -----------------------------------------------------------------        
void ModuleEditor::ImGuiLicenseWindow()
{
    ImGui::Begin("License", NULL, ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::Text("\n");

    ImGui::Text(" MIT License");
    ImGui::Text("\n");
    ImGui::Text(" Copyright (c) 2023 Miguel Rodriguez and Joan Damia\n\n");

    ImGui::Text(" Permission is hereby granted, free of charge, to any person obtaining a copy\n");
    ImGui::Text(" of this software and associated documentation files (the 'Software'), to deal\n");
    ImGui::Text(" in the Software without restriction, including without limitation the rights\n");
    ImGui::Text(" to use, copy, modify, merge, publish, distribute, sublicense, and/or sell\n");
    ImGui::Text(" copies of the Software, and to permit persons to whom the Software is\n");
    ImGui::Text(" furnished to do so, subject to the following conditions:\n\n");

    ImGui::Text(" The above copyright notice and this permission notice shall be included in all\n");
    ImGui::Text(" copies or substantial portions of the Software.\n\n");

    ImGui::Text(" THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\n");
    ImGui::Text(" IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n");
    ImGui::Text(" FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE\n");
    ImGui::Text(" AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\n");
    ImGui::Text(" LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\n");
    ImGui::Text(" OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\n");
    ImGui::Text(" SOFTWARE.\n");

    ImGui::Text("\n");

    ImGui::End();
}

// -----------------------------------------------------------------        
void ModuleEditor::ImGuiHierarchyWindow()
{
    ImGui::Begin("Hierarchy", NULL, ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::Text("Game Objects");

    DisplayGameObjects(GameObjects::gameObjectList[0]);

    ImGui::End();
}


void ModuleEditor::DisplayGameObjects(GameObjects* game_object)
{
    ImGuiTreeNodeFlags TreeFlags = ImGuiTreeNodeFlags_OpenOnArrow;
    TreeFlags |= ImGuiTreeNodeFlags_DefaultOpen;


    if (game_object == gameobject_selected)
    {
        TreeFlags |= ImGuiTreeNodeFlags_Selected;
    }
    if (game_object->GetChildren().empty())
    {
        TreeFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
        ImGui::TreeNodeEx(game_object->name.c_str(), TreeFlags);

        if (ImGui::IsItemHovered() && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
        {
            gameobject_selected = game_object;
        }
    }
    else
    {
        if (ImGui::TreeNodeEx(game_object->name.c_str(), TreeFlags))
        {
            if (ImGui::IsItemHovered() && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
            {
                gameobject_selected = game_object;
            }
            for (size_t i = 0; i < game_object->GetChildren().size(); i++)
            {
                DisplayGameObjects(game_object->GetChild(i));
            }
            ImGui::TreePop();
        }
    }

}
