#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <iostream>

using s32 = int32_t;
using u32 = uint32_t;

static u32 CompileShader(u32 type , const std::string &source )
{
	u32 id = glCreateShader(type);
	const char* src = source.c_str();//ptr to beginning of data
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	// #errorhandling
	s32 result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		s32 length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*) alloca(length * sizeof(char));//alloca on stack
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Fail to compile" << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}
	// #errorhandling

	return id;
}

static u32 CreateShader(const std::string& vertexShader, const std::string &fragmentShader)
{
	u32 program = glCreateProgram();
	u32 vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	u32 fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);
	
	return program;
}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(1920, 1080, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* Init glew */
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
	fprintf(stdout, "Status: Using GLFW %s\n", glewGetString(GL_VERSION));//? RETURN NULL

	float position[6]{
		-0.5f, -0.5f,
		0.0f, 0.5f,
		0.5f, -0.5f
	};

	//id for specific shader
	u32 buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);//Bind working buffer
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), position, GL_STATIC_DRAW);//stream in data
	//define the buffer layout
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT,GL_FALSE, sizeof(float) * 2, 0);//CAST TO CONST VOID PTR

	std::string vertexShader =
		"#version 330 core\n"
		"\n"
		"layout(location = 0) in vec4 position;"
		"\n"
		"void main()\n"
		"{\n"
		"gl_Position = position;\n"
		"}\n";
	
	std::string fragmentShader =
		"#version 330 core\n"
		"\n"
		"layout(location = 0) out vec4 color;"
		"\n"
		"void main()\n"
		"{\n"
		"color = vec4(1.0, 0.0, 0.0, 1.0);\n"
		"}\n";

	u32 shader = CreateShader(vertexShader,fragmentShader);
	glUseProgram(shader);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}