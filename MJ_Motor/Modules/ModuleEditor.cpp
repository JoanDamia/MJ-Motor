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

	ImGui::StyleColorsDark();

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
        static float f = 0.0f;
        static int counter = 0;

        //==============================================================================================================================================================

        //ImGui Render Window
        ImGui::Begin("MJ Render", NULL, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoBringToFrontOnFocus);
        ImGui::BeginChild("Render", ImVec2(SCREEN_WIDTH, SCREEN_HEIGHT));
        ImVec2 wsize = ImGui::GetWindowSize();
        ImGui::Image((ImTextureID)App->renderer3D->texColorBuffer, wsize, ImVec2(0, 1), ImVec2(1, 0));
        ImGui::EndChild();
        ImGui::End();

        //==============================================================================================================================================================
       
        //ImGui Basic Window
        ImGui::Begin("MJ Motor", NULL, ImGuiWindowFlags_MenuBar|ImGuiWindowFlags_AlwaysAutoResize);                        // Create a window called "Hello, world!" and append into it.

        if (ImGui::BeginMenuBar())
        {
            // Help menu
            if (ImGui::BeginMenu("Help"))
            {
                if (ImGui::MenuItem("Gui Demo"))
                    showDemo = !showDemo;

                if (ImGui::MenuItem("Documentation"))
                    ShellExecuteA(NULL, "open", "https://github.com/d0n3val/Edu-Game-Engine/wiki", NULL, NULL, SW_SHOWDEFAULT);

                if (ImGui::MenuItem("Download latest"))
                    ShellExecuteA(NULL, "open", "https://github.com/d0n3val/Edu-Game-Engine/releases", NULL, NULL, SW_SHOWDEFAULT);

                if (ImGui::MenuItem("Report a bug"))
                    ShellExecuteA(NULL, "open", "https://github.com/d0n3val/Edu-Game-Engine/issues", NULL, NULL, SW_SHOWDEFAULT);

                if (ImGui::MenuItem("About"))
                    ShellExecuteA(NULL, "open", "https://learnopengl.com/", NULL, NULL, SW_SHOWDEFAULT);

                if (ImGui::MenuItem("Support"))
                    ShellExecuteA(NULL, "open", "https://www.youtube.com/watch?v=2C4lFUpI_4U", NULL, NULL, SW_SHOWDEFAULT);

                ImGui::EndMenu();
            }

            // Configuration menu
            if (ImGui::BeginMenu("Configuration"))
            {
                if (ImGui::MenuItem("Options"))
                    showDemo = !showDemo;

                if (ImGui::MenuItem("Vsync"));

                if (ImGui::MenuItem("Input"));

                if (ImGui::MenuItem("Audio"));


                ImGui::EndMenu();
            }
        

            ImGui::EndMenuBar();
        }
        
        ImGui::Text("MJ - Ventana 1");               // Display some text (you can use a format strings too)

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

        ImGui::Checkbox("CubeDirectMode", &showCubeDirectMode);

        ImGui::Checkbox("CubeBufferColors", &showCubeBufferColors);

        if (ImGui::CollapsingHeader("FPS"))
        {
            char title[25];
            sprintf_s(title, 25, "Framerate %1.1f", fps_log[fps_log.size() - 1]);
            ImGui::PlotHistogram("##framerate", &fps_log[0], fps_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
            sprintf_s(title, 25, "Milliseconds %0.1f", ms_log[ms_log.size() - 1]);
            ImGui::PlotHistogram("##millisconds", &ms_log[0], ms_log.size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));
        }

        ImGui::End();
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