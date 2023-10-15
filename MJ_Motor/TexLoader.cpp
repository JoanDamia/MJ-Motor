
#include "DevIL/include/il.h"
#include "DevIL/include/ilu.h"
#include "DevIL/include/ilut.h"

#pragma comment (lib, "DevIL/libx86/DevIL.lib" )
#pragma comment (lib, "DevIL/libx86/ILU.lib" )
#pragma comment (lib, "DevIL/libx86/ILUT.lib" )

#include "TexLoader.h"

void TexLoader::LoadTexture()
{
	ILuint tex_Lenna;

	ilGenImages(1, &tex_Lenna);

	ilBindImage(tex_Lenna);

	//Load an image
	ilLoadImage("Lenna_(test_image)");

	//Save an image
	ilEnable(IL_FILE_OVERWRITE);
	ilSaveImage("Lenna_(test_image)");

	//Detect an error
	ILenum Error;
	Error = ilGetError();

	//Get an image's width and height
	ILuint Width, Height;
	Width = ilGetInteger(IL_IMAGE_WIDTH);
	Height = ilGetInteger(IL_IMAGE_HEIGHT);

	//Getting an OpenGL texture
	GLuint Texture;
	Texture = ilutGLBindTexImage();

	//Delete an image
	ilDeleteImages(1, &tex_Lenna);
}