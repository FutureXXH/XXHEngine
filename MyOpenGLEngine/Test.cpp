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
	t->Position = { setposx ,setposy,setposz };


	__MyEngine->AddObject(obj);
	return obj;
}

int speed = 5;
void sc(WorldManager* MyWorldManager, Object* MyObject)
{

	auto trans = MyObject->GetComponent<Transform>();
	trans->Position.x += MyWorldManager->deltaTime * speed;
	if (speed == 5 && trans->Position.x > 20)
		speed = -5;
	else if(speed == -5 && trans->Position.x < -20)
		speed = 5;

	
	auto shader = __MyEngine->GetShader("normal");
	shader->use();
	shader->SetVec3("lightColor", 1.0f, 1.0f, 1.0f);
	shader->SetVec3("lightPos", trans->Position.x, trans->Position.y, trans->Position.z);

	//cout << trans->PosX << "-" << trans->PosY << "-" << trans->PosZ << endl;
}

void test()
{
			auto shader =	__MyEngine->CreateShader("normal","./glsl/vertShader.glsl", "./glsl/fragShader.glsl");
			auto lightshader = __MyEngine->CreateShader("light", "./glsl/vertShader.glsl", "./glsl/light.glsl");
			auto Shadowshader = __MyEngine->CreateShader("simpleDepthShader", "./glsl/vert_shadow_mapping.glsl", "./glsl/frag_shadow_mapping.glsl", "./glsl/geometry_shadow_mapping.glsl");
			auto SkyBoxshader = __MyEngine->CreateShader("SkyBoxShader", "./glsl/VertSkyBox.glsl", "./glsl/FragSkyBox.glsl");

			vector<string> BoxTexs = {
					"./res/skybox/right.jpg",
					"./res/skybox/left.jpg",
					"./res/skybox/top.jpg",
					"./res/skybox/bottom.jpg",
					"./res/skybox/front.jpg",
					"./res/skybox/back.jpg"
			};

			__MyEngine->RegTexture("green", "./res/green.png");
			__MyEngine->RegTexture("blue", "./res/blue.png");

			__MyEngine->SetSkyBox(BoxTexs);

			__MyEngine->RegModel("m1", "./res/test.obj");
			__MyEngine->RegModel("q", "./res/test2.obj");
			__MyEngine->RegModel("ground", "./res/ground.obj");

			//生成主相机obj
			Camera* myMainCamera = new Camera();
			myMainCamera->SetW_H(600, 600);
			auto c =GenerateObject("Camera", "", "", "", 0, 0, 0);
			c->RegComponent(myMainCamera);
			__MyEngine->SetMainCamera(c);


			 GenerateObject("ground", "ground", "normal", "green", 0, -1, 0);
			GenerateObject("cube2", "m1", "normal", "blue", 5, 2, 5);
			GenerateObject("cube3", "m1", "normal", "blue", 2, 7, 9);
			GenerateObject("cube4", "m1", "normal", "blue", 2, 6, 2);
			GenerateObject("cube5", "m1", "normal", "blue", 6, 1, 6);
			GenerateObject("cube6", "m1", "normal", "blue", 3, 6, 7);
			

			auto temp = GenerateObject("Sun","q", "light","", 10, 20, 0);
			temp->GetComponent<Script>()->UpdateFun = sc;

}