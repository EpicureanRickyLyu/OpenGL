#pragma once
#include <GL/glew.h>

#include <GLFW/glfw3.h>


#include <iostream>


#define ASSERT(x) if(!(x)) __debugbreak();

#define GLCALL(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

//# in macro will turn function name into string
static void GLClearError()
{
	while (glGetError() != GL_NO_ERROR)
	{

	}
}

static bool GLLogCall(const char* function, const char* file, int line) 
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error]" << "(" << error <<")"<< function << " "<< file << " "<< std::endl;
		return false;
	}
	return true;
}



