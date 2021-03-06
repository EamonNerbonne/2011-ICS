#include "RgbaImageTexture.h"
#include <iostream>
#include <fstream>
#include "stb_image.h"
#include <string>
#include <algorithm>
#include <GL/GLU.h>

using std::cout;
using std::string;
char buf[1024];

#ifdef _MSC_VER
char pathsep = '\\';
char const * const parentPath = "..\\";
#else
char pathsep = '/';
char const * const parentPath = "../";
#endif

int file_size(const char* sFileName)
{
  std::ifstream f;
  f.open(sFileName, std::ios_base::binary | std::ios_base::in);
  if (!f.good() || f.eof() || !f.is_open()) { return 0; }
  f.seekg(0, std::ios_base::beg);
  std::ifstream::pos_type begin_pos = f.tellg();
  f.seekg(0, std::ios_base::end);
  return static_cast<int>(f.tellg() - begin_pos);
}

string find_texture(char const * filename) {
	string path(filename);
	std::replace( path.begin(), path.end(), '/', pathsep);

	for(int i=0; i<20;i++ ) {
		cout << path << "\n";
		std::ifstream test_file(path);
		if (test_file.good()) {
			cout << file_size(path.c_str()) << "\n";
			return path;
		}
		path = parentPath + path;
	}
	return string();
}

typedef void  (APIENTRY * MY_GLGENERATEMIPMAPS)(GLenum);


RgbaImageTexture::RgbaImageTexture(char const * filename) {
	MY_GLGENERATEMIPMAPS  glGenerateMipmap = (MY_GLGENERATEMIPMAPS)wglGetProcAddress("glGenerateMipmap");



	string fixed_path = find_texture(filename);
	int n;
	unsigned char *rawdata = stbi_load(fixed_path.c_str(), &width, &height, &n, 4);
//	cout<<"width: "<<width<<"; height: "<<height<<"; n: "<<n<<"; ptr:"<<(void*)rawdata<<"; val:"<<(int)rawdata[10000]<<"\n";
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);   // 2d texture (x and y size)

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // scale linearly when image bigger than texture
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR); // scale linearly when image smalled than texture
	

	// 2d texture, level of detail 0 (normal), 3 components (red, green, blue), x size from image, y size from image,
	// border 0 (normal), rgb color data, unsigned byte data, and finally the data itself.
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, rawdata);
	//gluBuild2DMipmaps( GL_TEXTURE_2D, 4, width, height, GL_RGBA, GL_UNSIGNED_BYTE, rawdata );
	//glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST); 
	glGenerateMipmap(GL_TEXTURE_2D);  //Generate mipmaps now!!!
	stbi_image_free(rawdata);
}

void RgbaImageTexture::BindTexture() {
	glBindTexture(GL_TEXTURE_2D, textureID);
	//cout<<"bound "<<textureID<<"\n";
}

RgbaImageTexture::~RgbaImageTexture() { glDeleteTextures(1, &textureID); }
