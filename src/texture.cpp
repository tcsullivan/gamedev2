#include "texture.hpp"

Texture::Texture(std::string filename)
{
    unsigned char* image = SOIL_load_image(filename.c_str(),
                                           &width, &height, 0,
                                           SOIL_LOAD_RGBA);

    glGenTextures(1, &tex);				// Turns "object" into a texture
	glBindTexture(GL_TEXTURE_2D, tex);	// Binds "object" to the top of the stack
	glPixelStoref(GL_UNPACK_ALIGNMENT, 1);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	// Sets the "min" filter
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);	// The the "max" filter of the stack
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Wrap the texture to the matrix
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); //
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
				 GL_RGBA, GL_UNSIGNED_BYTE, image);

    SOIL_free_image_data(image);
}
