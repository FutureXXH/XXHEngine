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
	struct  Material
	{
		GLuint diffuse = 0;
		glm::vec3 specular = { 1.0,1.0,1.0 };
		float shininess = 0.5;
	};


	string ModelName;
	string TextureName;
	string ShaderName;
	Material ModelMaterial;

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

class Camera:public ComponentBase
{
private:
	
	bool keys[1024];
	GLfloat yaw = 0;
	GLfloat pitch = 0;
	GLfloat lastX, lastY;


public:
	int Width = 600;
	int Height = 600;
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	bool Mouse_Right_Down = false;
	bool firstMouse = true;

	void key_callback(int key, int action); 
	void mouse_callback(GLfloat xpos, GLfloat ypos);
	void SetW_H(int W,int H);

	void Update(WorldManager* MyWorldManager, Object* MyObject) override;

};







class MyOpenGL
{
private:


	GLFWwindow* window;
	float aspect;
	glm::mat4 pMat, vMat, mMat, mvMat, tMat, rMat;
	//һЩͳһ����
	GLuint vLoc, projLoc, tfLoc, mLoc, mvLoc;

	//�������ͼ
	GLuint depthMap, depthMapFBO;


	vector< Object*> RenderObjs;
	// ģ�ʹ洢
	unordered_map<string,Model*> Models;
	// ��ͼ�洢
	unordered_map<string,GLuint> Textures;
	//��ɫ���洢
	unordered_map<string, Shader*> Shaders;
	//û��ָ����ͼʱʹ��
	GLuint DefaultTexture;

	// ��պж���
	Object* skyBoxObj;
	
	// ��Ⱦ�ƹ� ��Ӱ��ͼ
	void DrawLight(Camera* MainCamera,WorldManager* MyWorldManager);
	//��Ⱦ��պ�
	void DrawSkyBox(Camera* MainCamera);
	//��Ⱦ��ͨ����
	void DrawNormalObj(Camera* MainCamera);

public:
	
	static Object* MainCameraObject;

	//�����Ҫ��Ⱦ�Ķ���
	bool AddRenderingObj(Object* obj);

	//���������
	static bool SetMainCamera(Object* setc);

	//ע��ģ��
	bool RegModel(string ModelName,string ModelPath);

	//������պ�
	bool SetSkyBox(vector<string> paths);

	//��ȡ��ɫ��
	Shader* GetShader(string Name);
	//ע����ɫ��
	Shader* RegShader(string Name, string vertexPath, string fragmentPath, string geometryPath = "");

	//ע����ͼ2D
	GLuint RegTexture(string TextureName, const char* texImagePath);
	//ע����������ͼ
	GLuint RegTexture(string TextureName,vector<string> Paths);


	// ��ʼ��
	bool OpenGLInit();
	// ����
	void OpenGLRun();

	int OpenGLUpdate(WorldManager* MyWorldManager);

	void DisPlay(GLFWwindow* window, double currentTime, WorldManager* MyWorldManager);




	

};



#endif // !__MYOPENGL_H
