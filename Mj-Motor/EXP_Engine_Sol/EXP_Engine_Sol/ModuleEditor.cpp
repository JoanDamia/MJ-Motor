#include "ModuleEditor.h"
#include "imGui/imgui.h"
#include "ImGui/backends/imgui_impl_opengl3.h"

ModuleEditor::ModuleEditor(Application* app, bool start_enabled) : Module(app, start_enabled) {

}

ModuleEditor::~ModuleEditor() {

}

bool ModuleEditor::Init() {
	IMGUI_CHECKVERSION() :
		ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGameplad;

	imGui::StyleolorsDark();

	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL3_Init();




return false
}

void ModuleEditor::DrawEditor() {

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();

	//here

	ImGui::render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

bool ModuleEditor::CleanUp()

