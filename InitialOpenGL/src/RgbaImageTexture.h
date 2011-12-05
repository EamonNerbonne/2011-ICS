#pragma once
#ifdef _MSC_VER
#include <windows.h>
#endif
#include <GL/gl.h>

struct RgbaImageTexture {
	int width, height;
	GLuint textureID;
	RgbaImageTexture(char const * filename);
	void BindTexture();
	~RgbaImageTexture();

private:
	RgbaImageTexture& operator=(const RgbaImageTexture&);	// Disallow copying
	RgbaImageTexture(const RgbaImageTexture&);
};


