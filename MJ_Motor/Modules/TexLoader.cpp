
#include "DevIL/include/il.h"
#include "DevIL/include/ilu.h"
#include "DevIL/include/ilut.h"

#pragma comment (lib, "DevIL/libx86/DevIL.lib" )
#pragma comment (lib, "DevIL/libx86/ILU.lib" )
#pragma comment (lib, "DevIL/libx86/ILUT.lib" )

#include "TexLoader.h"

std::map<std::string, uint> TexLoader::texture_names;

GLuint TexLoader::LoadTexture(char const* thefilename)
{
	if (texture_names.find(thefilename) != texture_names.end())
	{
		return texture_names[thefilename];
	}

	ILuint tex_id;
	ilGenImages(1, &tex_id);
	ilBindImage(tex_id);

	//Load an image
	ilLoadImage(thefilename);

	tex_id = ilutGLBindTexImage();

	// Clean Image
	ilBindImage(0);
	ilDeleteImages(1, &tex_id);

	texture_names[thefilename] = tex_id;

	return tex_id;
}