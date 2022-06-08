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
	//引擎初始化
	bool EngineInit();
	//引擎运行
	void EngineRun();
	//设置主相机
	bool SetMainCamera(Object* setc);
	//设置天空盒
	bool SetSkyBox(vector<string> paths);
	//添加对象
	bool AddObject(Object* obj);
	//创建着色器
	Shader* CreateShader(string ShaderName, string vertexPath, string fragmentPath, string geometryPath = "");
	//获取着色器
	Shader* GetShader(string ShaderName);
	//注册贴图
	GLuint RegTexture(string TextureName, const char* texImagePath);
	//注册贴图 立方体
	GLuint RegTexture(string TextrueName, vector<string> Paths);
	//注册模型
	bool RegModel(string ModelName, string ModelPath);



};

extern MyEngine* __MyEngine;


#endif // !__ENGINE_H
