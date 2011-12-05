#pragma once
#include <GL/gl.h>

struct RgbaImageTexture {
	int width, height;
	GLuint textureID;
	RgbaImageTexture(char const * filename);
	void BindTexture();
	~RgbaImageTexture();

	RgbaImageTexture& operator=(const RgbaImageTexture&) = delete;	// Disallow copying
	RgbaImageTexture(const RgbaImageTexture&) = delete;
};


