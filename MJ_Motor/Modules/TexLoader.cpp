
#include "DevIL/include/il.h"
#include "DevIL/include/ilu.h"
#include "DevIL/include/ilut.h"

#pragma comment (lib, "DevIL/libx86/DevIL.lib" )
#pragma comment (lib, "DevIL/libx86/ILU.lib" )
#pragma comment (lib, "DevIL/libx86/ILUT.lib" )

#include "TexLoader.h"

GLuint TexLoader::LoadTexture(char const* thefilename)
{
	ilInit();
	iluInit();
	ilutInit();

	ILuint tex_Lenna;
	ilGenImages(1, &tex_Lenna);
	ilBindImage(tex_Lenna);

	//Load an image
	ilLoadImage(thefilename);

	tex_Lenna = ilutGLBindTexImage();

	// Clean Image
	ilBindImage(0);
	ilDeleteImages(1, &tex_Lenna);

	return tex_Lenna;

	/*
	int const width = ilGetInteger(IL_IMAGE_WIDTH);
	int const height = ilGetInteger(IL_IMAGE_HEIGHT);
	int const type = ilGetInteger(IL_IMAGE_TYPE); // matches OpenGL
	int const format = ilGetInteger(IL_IMAGE_FORMAT); // matches OpenGL

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glPixelStorei(GL_UNPACK_SWAP_BYTES, GL_FALSE);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0); // rows are tightly packed
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // pixels are tightly packed

	glTexImage2D(GL_TEXTURE_2D, 0, 1, 10, 10, 0, 1, 1, 0);
	*/
}