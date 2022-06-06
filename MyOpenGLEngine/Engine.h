#ifndef __ENGINE_H
#define __ENGINE_H
#include "MyOpenGL.h"
#include "ObjectSystem.h"
#include "WorldManager.h"
class MyEngine
{
	MyOpenGL* myOpenGL = new MyOpenGL();
	WorldManager* MyWorldManager = new WorldManager();



public:
	//�����ʼ��
	bool EngineInit();
	//��������
	void EngineRun();


	bool AddObject(Object* obj);

	Shader* CreateShader(string ShaderName, const char* vertexPath, const char* fragmentPath);

	Shader* GetShader(string ShaderName);

	GLuint RegTexture(string TextureName, const char* texImagePath);

	bool RegModel(string ModelName, string ModelPath);



};

extern MyEngine* __MyEngine;


#endif // !__ENGINE_H
