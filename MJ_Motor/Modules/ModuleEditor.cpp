#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"
#include <SDL/include/SDL_opengl.h>

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
                if (ImGui::MenuItem("Gui Demo"))

                if (ImGui::MenuItem("Documentation"))
                    ShellExecuteA(NULL, "open", "https://github.com/d0n3val/Edu-Game-Engine/wiki", NULL, NULL, SW_SHOWDEFAULT);

                if (ImGui::MenuItem("Download latest"))
                    ShellExecuteA(NULL, "open", "https://github.com/d0n3val/Edu-Game-Engine/releases", NULL, NULL, SW_SHOWDEFAULT);

                if (ImGui::MenuItem("Report a bug"))
                    ShellExecuteA(NULL, "open", "https://github.com/d0n3val/Edu-Game-Engine/issues", NULL, NULL, SW_SHOWDEFAULT);

                if (ImGui::MenuItem("About"))
                    ShellExecuteA(NULL, "open", "https://learnopengl.com/", NULL, NULL, SW_SHOWDEFAULT);

                ImGui::EndMenu();
            }

            // Window menu
            if (ImGui::BeginMenu("Window"))
            {
                if (ImGui::MenuItem("Scene"))
                    showScene = !showScene;

                if (ImGui::MenuItem("Inspector"))
                    showInspector = !showInspector;;

                if (ImGui::MenuItem("Console"))
                    showConsole = !showConsole;;

                ImGui::EndMenu();
            }

            // About menu
            if (ImGui::BeginMenu("About"))
            {
                if (ImGui::MenuItem("Options"))

                if (ImGui::MenuItem("Vsync"));

                if (ImGui::MenuItem("Input"));

                if (ImGui::MenuItem("Audio"));


                ImGui::EndMenu();
            }

            // Exit menu
            if (ImGui::BeginMenu("Exit"))
            {
                if (ImGui::MenuItem("Close Motor"))
                    return UPDATE_STOP;

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
            ImguiRenderWindow();
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

        //==============================================================================================================================================================

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
void ModuleEditor::ImguiRenderWindow() 
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

    ImGui::Text("MJ - Ventana 1");               // Display some text (you can use a format strings too)

    //ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

    ImGui::Checkbox("CubeDirectMode", &showCubeDirectMode);

    ImGui::Checkbox("Checkers Cube", &showCubeCheckers);

    if (ImGui::CollapsingHeader("FPS"))
    {
        char title[25];
        sprintf_s(title, 25, "Framerate %1.1f", fps_log[fps_log.size() - 1]);
        ImGui::PlotHistogram("##framerate", &fps_log[0], fps_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
        sprintf_s(title, 25, "Milliseconds %0.1f", ms_log[ms_log.size() - 1]);
        ImGui::PlotHistogram("##millisconds", &ms_log[0], ms_log.size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));
    }

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
    }

    ImGui::End();
}
