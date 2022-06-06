#include "Shader.h"



void Shader::printShaderLog(GLuint shader)
{
	int len = 0;
	int chWrittn = 0;
	char* log;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
	if (len > 0)
	{
		log = new char[len];
		glGetShaderInfoLog(shader, len, &chWrittn, log);
		cout << shader << " Shader Info Log : " << log << endl;
		delete[]log;
	}
}


void Shader::printProgramLog(int Prog)
{


	int len = 0;
	int chWrittn = 0;
	char* log;
	glGetShaderiv(Prog, GL_INFO_LOG_LENGTH, &len);
	if (len > 0)
	{
		log = new char[len];
		glGetShaderInfoLog(Prog, len, &chWrittn, log);
		cout << " Program Info Log : " << log << endl;
		delete[]log;
	}
}

bool  Shader::checkOpenGLError()
{
	bool foundError = false;
	int glErr = glGetError();
	while (glErr != GL_NO_ERROR)
	{
		cout << "gl ERROR:" << glErr << endl;
		foundError = true;
		glErr = glGetError();
	}
	return foundError;
}


string Shader::readShaderSource(string filePath)
{
	string content = "";
	ifstream fileStream(filePath.c_str(), ios::in);
	string line = "";
	while (!fileStream.eof())
	{
		getline(fileStream, line);
		content += (line + "\n");
	}
	fileStream.close();

	return content;
}

GLuint Shader::LoadShader(string FilePath, GLenum Type)
{
	string temp = readShaderSource(FilePath);
	const char* shaderSourace = temp.c_str();
	GLuint Shader = glCreateShader(Type);
	glShaderSource(Shader, 1, &shaderSourace, NULL);

	glCompileShader(Shader);
	checkOpenGLError();
	GLint vertCompiled;
	glGetShaderiv(Shader, GL_COMPILE_STATUS, &vertCompiled);
	if (vertCompiled != 1)
	{
		cout << " compilation failed" << endl;
		printShaderLog(Shader);
		return -1;
	}
	glAttachShader(ID, Shader);
	cout << "´´½¨Shader£º" << FilePath << "  " << Shader << endl;
	return Shader;
}



bool Shader::Init(const char* vertexPath, const char* fragmentPath)
{
	ID = glCreateProgram();
	if (LoadShader(vertexPath, GL_VERTEX_SHADER) < 0)return false;
	
	if (LoadShader(fragmentPath, GL_FRAGMENT_SHADER) < 0) return false;
	glLinkProgram(ID);
	cout << ID << endl;

	return true;
}

void Shader::use()
{
	glUseProgram(ID);
}



void Shader::SetBool(const string& name, bool value)
{
glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::SetInt(const string& name, int value)
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetFloat(const string& name, float value)
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetMatrix4fv(const string& name, glm::mat4 value)
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetVec3(const string& name, float a, float b, float c)
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), a, b, c);
}


void Shader::SetVec4(const string& name, float a, float b, float c, float d)
{
	glUniform4f(glGetUniformLocation(ID, name.c_str()), a, b, c, d);
}



