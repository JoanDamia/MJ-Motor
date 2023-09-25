#ifndef MODULE_EDITOR
#define MODULE_EDITOR

#include "Module.h"
#include <vector>

class ModuleEditor : public Module
{
public:

	ModuleEditor(Application* app, bool start_enabled = true);
	ModuleEditor() :

	bool Init() override;
	void DrawEditor();
	bool CleanUp() override;

}

#endif // !MODULE_EDITOR
