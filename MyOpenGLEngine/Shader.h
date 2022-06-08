#ifndef __SHADER_H
#define __SHADER_H

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SOIL2/SOIL2.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "WorldManager.h"
using namespace std;

//×ÅÉ«Æ÷Àà
class Shader
{
private:
	void printShaderLog(GLuint shader);

	void printProgramLog(int Prog);

	bool checkOpenGLError();

	string readShaderSource(string filePath);

	GLuint LoadShader(string FilePath, GLenum Type);

	
public:
	unsigned int ID;

	bool Init(string vertexPath, string fragmentPath, string geometryPath = "");
	void use();
	void (*update)(Shader* shader,double currentTime, WorldManager* MyWorldManager);
	void SetBool(const string& name, bool value);
	void SetInt(const string& name, int value);
	void SetFloat(const string& name, float value);
	void SetMatrix4fv(const string& name, glm::mat4 value);
	void SetVec3(const string& name, float a, float b, float c);
	void SetVec4(const string& name, float a, float b, float c, float d);

	void setMat4(const std::string& name, const glm::mat4& mat);
};



#endif // !__SHADER_H