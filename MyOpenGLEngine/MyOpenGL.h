#ifndef __MYOPENGL_H
#define __MYOPENGL_H

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SOIL2/SOIL2.h>
#include <fstream>
#include <sstream>
#include <glm\glm.hpp>
#include <vector>
#include <unordered_map>
#include "Shader.h"
#include "WorldManager.h"
using namespace std;
class ModelImporter;

class ImportedModel
{
private:
	int numVertices;
	vector<glm::vec3> vertices;
	vector<glm::vec2> texCoords;
	vector<glm::vec3> normalVecs;
public:
	ImportedModel(const char* filePath);
	int getNumVertices();
	vector<glm::vec3> getVertices();
	vector<glm::vec2> getTextureCoords();
	vector<glm::vec3> getNormals();


};



class ModelImporter
{
private:
	vector<float> vertVals;
	vector<float> stVals;
	vector<float> normVals;

	vector<float> triangleVerts;
	vector<float> textureCoords;
	vector<float> normals;
public:
	ModelImporter();
	void parseOBJ(const char* filePath);
	int getNumVertices();
	vector<float> getVertices();
	vector<float> getTextureCoordinates();
	vector<float> getNormals();
};




class Rendering :public ComponentBase
{
public:
	string ModelName;
	string TextureName;
	string ShaderName;

};





class Model
{
private:
	int NumVertices = 0;
	
public:
	
	bool ImportModel(string ObjFilePath);
	int getNumVertices();
	GLuint vao[1];
	GLuint vbo[3];

};

class Camera
{
private:
	
	bool keys[1024];
	GLfloat yaw = 0;
	GLfloat pitch = 0;
	GLfloat lastX, lastY;


public:
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	bool Mouse_Right_Down = false;
	bool firstMouse = true;
	void do_movement(GLfloat deltaTime);
	void key_callback(int key, int action); 
	void mouse_callback(GLfloat xpos, GLfloat ypos);

};





class MyOpenGL
{
private:


	GLFWwindow* window;
	float aspect;
	glm::mat4 pMat, vMat, mMat, mvMat, tMat, rMat;
	int width, height;
	GLuint vLoc, projLoc, tfLoc, mLoc, mvLoc;

	float cameraX = 0, cameraY =0, cameraZ = 10;

	
	vector< Object*> RenderObjs;

	unordered_map<string,Model*> Models;
	unordered_map<string,GLuint> Textures;
	unordered_map<string, Shader*> Shaders;

	GLuint DefaultTexture;



public:
	


	//添加需要渲染的对象
	bool AddRenderingObj(Object* obj);

	

	bool RegModel(string ModelName,string ModelPath);

	Shader* GetShader(string Name);

	Shader* RegShader(string Name, const char* vertexPath, const char* fragmentPath);

	GLuint RegTexture(string TextureName, const char* texImagePath);

	void window_reshape_callback(GLFWwindow* window, int newWidth, int newHeight);

	bool OpenGLInit(int width, int height);

	void OpenGLRun();

	int OpenGLUpdate(WorldManager* MyWorldManager);

	void DisPlay(GLFWwindow* window, double currentTime, WorldManager* MyWorldManager);


	

};



#endif // !__MYOPENGL_H
