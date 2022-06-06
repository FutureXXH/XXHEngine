#include "Engine.h"


Object* GenerateObject(string name, string ModelName, string ShaderName, string TextrueName = "", float setposx = 0, float setposy = 0, float setposz = 0)
{
	Object* obj = new Object();
	obj->name = name;
	Transform* t = new Transform();
	Rendering* r = new Rendering();
	Script* s = new Script();
	obj->RegComponent(t);
	obj->RegComponent(r);
	obj->RegComponent(s);
	r->ModelName = ModelName;
	r->TextureName = TextrueName;
	r->ShaderName = ShaderName;
	t->PosX = setposx;
	t->PosY = setposy;
	t->PosZ = setposz;

	__MyEngine->AddObject(obj);
	return obj;
}

int speed = 5;
void sc(WorldManager* MyWorldManager, Object* MyObject)
{

	auto trans = MyObject->GetComponent<Transform>();
	trans->PosX += MyWorldManager->deltaTime * speed;
	if (speed == 5 &&trans->PosX > 20)
		speed = -5;
	else if(speed == -5 && trans->PosX < -20)
		speed = 5;

	




	auto shader = __MyEngine->GetShader("test1");
	shader->use();
	 shader->SetVec3("lightColor", 1.0f, 1.0f, 1.0f);
	shader->SetVec3("lightPos", trans->PosX, trans->PosY, trans->PosZ);



	
}

void test()
{
			auto shader =	__MyEngine->CreateShader("test1","./glsl/vertShader.glsl", "./glsl/fragShader.glsl");
			auto lightshader = __MyEngine->CreateShader("light", "./glsl/vertShader.glsl", "./glsl/light.glsl");


			__MyEngine->RegTexture("green", "./res/green.png");
			__MyEngine->RegTexture("blue", "./res/blue.png");

			__MyEngine->RegModel("m1", "./res/test.obj");
			__MyEngine->RegModel("q", "./res/test2.obj");
			__MyEngine->RegModel("ground", "./res/ground.obj");


			auto temp = GenerateObject("cube1", "m1", "test1", "blue", 2, 1, 0);
			//auto temp2 = GenerateObject("g", "ground", "test1", "green", 0, 0, 0);
			GenerateObject("cube1", "m1", "test1", "blue", 5, 1, 5);
			GenerateObject("cube2", "m1", "test1", "blue", 2, 7, 10);
			GenerateObject("cube3", "m1", "test1", "blue", 2, 6, 2);
			GenerateObject("cube4", "m1", "test1", "blue", 6, 1, 6);
			GenerateObject("cube5", "m1", "test1", "blue", 3, 10, 7);

			auto temp3 = GenerateObject("Sun","q", "light","", 10, 20, 0);
			temp3->GetComponent<Script>()->UpdateFun = sc;

}