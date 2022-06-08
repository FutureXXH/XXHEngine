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
	//���������
	bool SetMainCamera(Object* setc);
	//������պ�
	bool SetSkyBox(vector<string> paths);
	//��Ӷ���
	bool AddObject(Object* obj);
	//������ɫ��
	Shader* CreateShader(string ShaderName, string vertexPath, string fragmentPath, string geometryPath = "");
	//��ȡ��ɫ��
	Shader* GetShader(string ShaderName);
	//ע����ͼ
	GLuint RegTexture(string TextureName, const char* texImagePath);
	//ע����ͼ ������
	GLuint RegTexture(string TextrueName, vector<string> Paths);
	//ע��ģ��
	bool RegModel(string ModelName, string ModelPath);



};

extern MyEngine* __MyEngine;


#endif // !__ENGINE_H
