#ifndef __ENGINE_H
#define __ENGINE_H
#include "MyOpenGL.h"
#include "ObjectSystem.h"

class MyEngine
{
	MyOpenGL* myOpenGL = new MyOpenGL();;
	ObjectSystem* MyObjectSystem = new ObjectSystem();;
public:
	bool EngineInit();

	void EngineRun();


	bool AddObject(Object* obj);

	bool LoadShader(string FilePath, GLenum Type);
	GLuint RegTexture(string TextureName, const char* texImagePath);
	bool RegModel(string ModelName, string ModelPath);

};

extern MyEngine* __MyEngine;


#endif // !__ENGINE_H
