#pragma once
#ifdef _MSC_VER
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#include <GL/gl.h>

class RgbaImageTexture {
	RgbaImageTexture& operator=(const RgbaImageTexture&);	// Disallow copying
	RgbaImageTexture(const RgbaImageTexture&);
public:

	int width, height;
	GLuint textureID;
	RgbaImageTexture(char const * filename);
	void BindTexture();
	~RgbaImageTexture();
};