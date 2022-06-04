#include "Engine.h"

Object* GenerateObject(string name, string ModelName, string TextrueName = "", float setposx = 0, float setposy = 0, float setposz = 0)
{
	Object* obj = new Object();
	obj->name = name;
	Transform* t = new Transform();
	Rendering* r = new Rendering();

	obj->RegComponent(t);
	obj->RegComponent(r);
	r->ModelName = ModelName;
	r->TextureName = TextrueName;
	t->PosX = setposx;
	t->PosY = setposy;
	t->PosZ = setposz;
	return obj;
}


void test()
{
				__MyEngine->LoadShader("./glsl/vertShader.glsl", GL_VERTEX_SHADER);
				__MyEngine->LoadShader("./glsl/fragShader.glsl", GL_FRAGMENT_SHADER);

				__MyEngine->RegTexture("t1", "./res/wl2.png");
				__MyEngine->RegModel("m1", "./res/test.obj");


			auto temp = GenerateObject("cube1", "m1", "t1", 0, 1, 0);
			auto temp2 = GenerateObject("cube2","m1", "t1", 1, 3, 0);
			__MyEngine->AddObject(temp);
			__MyEngine->AddObject(temp2);
}