#include "RgbaImageTexture.h"
#include <iostream>
#include "stb_image.h"

using std::cout;

RgbaImageTexture::RgbaImageTexture(char const * filename) {
	int n;
	unsigned char *rawdata = stbi_load(filename, &width, &height, &n, 4);
//	cout<<"width: "<<width<<"; height: "<<height<<"; n: "<<n<<"; ptr:"<<(void*)rawdata<<"; val:"<<(int)rawdata[10000]<<"\n";
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);   // 2d texture (x and y size)

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // scale linearly when image bigger than texture
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // scale linearly when image smalled than texture

	// 2d texture, level of detail 0 (normal), 3 components (red, green, blue), x size from image, y size from image,
	// border 0 (normal), rgb color data, unsigned byte data, and finally the data itself.
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, rawdata);
	stbi_image_free(rawdata);
	glEnable(GL_TEXTURE_2D);
}

void RgbaImageTexture::BindTexture() {
	glBindTexture(GL_TEXTURE_2D, textureID);
	//cout<<"bound "<<textureID<<"\n";
}

RgbaImageTexture::~RgbaImageTexture() { glDeleteTextures(1, &textureID); }
