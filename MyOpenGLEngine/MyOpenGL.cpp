#include "MyOpenGL.h"



Object* MyOpenGL::MainCameraObject = nullptr;


bool Model::ImportModel(string ObjFilePath)
{


	ImportedModel myModel(ObjFilePath.c_str());
	vector<glm::vec3> vert = myModel.getVertices();
	vector<glm::vec2> tex = myModel.getTextureCoords();
	vector<glm::vec3> norm = myModel.getNormals();
	 NumVertices = myModel.getNumVertices();
	vector<float> Pvalues;
	vector<float> Tvalues;
	vector<float> Nvalues;


	for (int i = 0; i < NumVertices; i++)
	{
		Pvalues.emplace_back(vert[i].x);
		Pvalues.emplace_back(vert[i].y);
		Pvalues.emplace_back(vert[i].z);
		Tvalues.emplace_back(tex[i].s);
		Tvalues.emplace_back(tex[i].t);
		Nvalues.emplace_back(norm[i].x);
		Nvalues.emplace_back(norm[i].y);
		Nvalues.emplace_back(norm[i].z);
	}

	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(3, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, Pvalues.size() * 4, &Pvalues[0], GL_STATIC_DRAW);


	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, Tvalues.size() * 4, &Tvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, Nvalues.size() * 4, &Nvalues[0], GL_STATIC_DRAW);
	return true;
}

int Model::getNumVertices()
{
	return NumVertices;
}


//=========================================================================================














//============================================================================



bool  MyOpenGL::SetMainCamera(Object* setc)
{
	if (setc == nullptr)return false;
	MainCameraObject = setc;

	return true;
}


bool MyOpenGL::RegModel(string ModelName, string ModelPath)
{
	ifstream f(ModelPath.c_str());
	if (!f.good())
	{
		cout << "模型文件不存在  " << ModelPath << endl;
		return false;
	}

	Model* NewModel = new Model();
	NewModel->ImportModel(ModelPath);
	Models.emplace(ModelName, NewModel);

	return true;
}

void MyOpenGL::DrawLight(Camera* MainCamera, WorldManager* MyWorldManager)
{
	//生成shadow mapping

	glEnable(GL_DEPTH_TEST);


	auto lpos = MyWorldManager->FindObject("Sun")->GetComponent<Transform>();
	glm::mat4 lightSpaceMatrix;
	glm::vec3 lightPos = lpos->Position;
	glm::mat4 lightProjection, lightView;
	lightProjection = glm::perspective(glm::radians(90.0f), aspect, 0.1f, 100.0f);
	std::vector<glm::mat4> shadowTransforms;
	shadowTransforms.push_back(lightProjection * glm::lookAt(lightPos, lightPos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(lightProjection * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(lightProjection * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
	shadowTransforms.push_back(lightProjection * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
	shadowTransforms.push_back(lightProjection * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(lightProjection * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));


	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	auto simpleDepthShader = GetShader("simpleDepthShader");
	simpleDepthShader->use();
	for (GLuint i = 0; i < 6; ++i)
		simpleDepthShader->SetMatrix4fv(("shadowMatrices[" + std::to_string(i) + "]").c_str(), shadowTransforms[i]);

	simpleDepthShader->SetVec3("lightPos", lightPos.x, lightPos.y, lightPos.z);


	for (int i = 0; i < RenderObjs.size(); i++)
	{
		Object* CurModel = RenderObjs[i];
		if (CurModel == nullptr)continue;

		//获取渲染组件
		auto CurRenderingModel = CurModel->GetComponent<Rendering>();
		if (CurRenderingModel == nullptr)continue;

		//获取模型
		if (Models.count(CurRenderingModel->ModelName) == 0)continue;
		auto  Modelp = Models[CurRenderingModel->ModelName];

		//获取变换组件
		auto CurModelTransform = CurModel->GetComponent<Transform>();
		if (CurModelTransform == nullptr)continue;





		mMat = glm::translate(glm::mat4(1.0f), CurModelTransform->Position);
		simpleDepthShader->SetMatrix4fv("m_matrix", mMat);


		glBindBuffer(GL_ARRAY_BUFFER, Modelp->vbo[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(0);


		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glDrawBuffer(GL_NONE);


		glDepthFunc(GL_LEQUAL);

		glDrawArrays(GL_TRIANGLES, 0, Modelp->getNumVertices());
		
		
	}


	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void MyOpenGL::DrawSkyBox(Camera* MainCamera)
{
	
	if (skyBoxObj == nullptr)return;
	auto skyp = skyBoxObj->GetComponent<Rendering>();
	auto skyboxShader = GetShader(skyp->ShaderName);
	auto SkyModule = Models[skyp->ModelName];


	//glDepthMask(GL_FALSE);
	//glDepthFunc(GL_LEQUAL);
	skyboxShader->use();

	skyboxShader->setMat4("v_matrix", vMat);
	skyboxShader->setMat4("projection", pMat);
	// skybox cube
	glBindBuffer(GL_ARRAY_BUFFER, SkyModule->vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, Textures[skyp->TextureName]);


	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glDisable(GL_DEPTH_TEST);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);
	//glDepthMask(GL_TRUE);

}

void MyOpenGL::DrawNormalObj(Camera* MainCamera)
{




	glDrawBuffer(GL_FRONT);
	for (int i = 0; i < RenderObjs.size(); i++)
	{
		Object* CurModel = RenderObjs[i];
		if (CurModel == nullptr)continue;



		//获取渲染组件
		auto CurRenderingModel = CurModel->GetComponent<Rendering>();
		if (CurRenderingModel == nullptr)continue;

		//获取模型
		if (Models.count(CurRenderingModel->ModelName) == 0)continue;
		auto  Modelp = Models[CurRenderingModel->ModelName];

		//获取变换组件
		auto CurModelTransform = CurModel->GetComponent<Transform>();
		if (CurModelTransform == nullptr)continue;

		//获取shader
		if (Shaders.count(CurRenderingModel->ShaderName) <= 0)continue;
		auto CurShader = GetShader(CurRenderingModel->ShaderName);

		CurShader->use();

		mMat = glm::translate(glm::mat4(1.0f), CurModelTransform->Position);

		CurShader->SetMatrix4fv("m_matrix", mMat);
		CurShader->SetMatrix4fv("v_matrix", vMat);
		CurShader->SetMatrix4fv("proj_matrix", pMat);
		CurShader->SetVec3("viewPos", MainCamera->cameraPos.x, MainCamera->cameraPos.y, MainCamera->cameraPos.z);


		glBindBuffer(GL_ARRAY_BUFFER, Modelp->vbo[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, Modelp->vbo[1]);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, Modelp->vbo[2]);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(2);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthMap);

		if (Textures.count(CurRenderingModel->TextureName) > 0)
		{
			glUniform1i(glGetUniformLocation(CurShader->ID, "textureSample"), 1);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, Textures[CurRenderingModel->TextureName]);
			CurShader->SetVec3("material.specular", CurRenderingModel->ModelMaterial.specular.x, CurRenderingModel->ModelMaterial.specular.y, CurRenderingModel->ModelMaterial.specular.z);
			CurShader->SetFloat("material.shininess", CurRenderingModel->ModelMaterial.shininess);
			CurShader->SetInt("material.diffuse", 0);
		}
		else
		{
			glUniform1i(glGetUniformLocation(CurShader->ID, "textureSample"), 0);
		}




		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glDrawArrays(GL_TRIANGLES, 0, Modelp->getNumVertices());
	
	}

	
}

bool MyOpenGL::SetSkyBox(vector<string> paths)
{

	if (skyBoxObj == nullptr)
	{
		skyBoxObj = new Object();
	}

	auto rendering = skyBoxObj->GetComponent<Rendering>();
	if (rendering == nullptr)
	{
		rendering = new Rendering();
		skyBoxObj->RegComponent(rendering);
	}
	RegTexture("SkyBoxTex", paths);

	rendering->ModelName = "SkyBoxModel";
	rendering->TextureName = "SkyBoxTex";
	rendering->ShaderName = "SkyBoxShader";

	float skyboxVertices[] = {
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f

	};
	Model* NewModel = new Model();
	Models.emplace("SkyBoxModel", NewModel);
	glGenVertexArrays(1, NewModel->vao);
	glGenBuffers(1, NewModel->vbo);
	glBindVertexArray(NewModel->vao[0]);

	glBindBuffer(GL_ARRAY_BUFFER, NewModel->vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


	glBindVertexArray(0);



	return true;
}





Shader* MyOpenGL::GetShader(string Name)
{
	if (Shaders.count(Name) > 0)
	{
		return Shaders[Name];
	}

	return nullptr;
}





Shader* MyOpenGL::RegShader(string Name, string vertexPath, string fragmentPath, string geometryPath)
{
	if (Shaders.count(Name) > 0)
	{
		cout << "Shader :" << Name << "重名" << endl;
		return nullptr;
	}

	Shader* newshader = new Shader();
	if (!newshader->Init(vertexPath, fragmentPath, geometryPath))
	{
		cout << "Shader :" << Name << "创建失败" << endl;
		delete newshader;
		return nullptr;
	}


	Shaders.emplace(Name, newshader);


	return newshader;
}







bool MyOpenGL::OpenGLInit()
{

	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	window = glfwCreateWindow(600, 600, "XXH Engine", NULL, NULL);
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }

	glfwSwapInterval(1);


	


	return true;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	//cout << key << endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	auto MainCamera = MyOpenGL::MainCameraObject->GetComponent<Camera>();
	if(MainCamera != nullptr)
		MainCamera->key_callback(key, action);
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	auto MainCamera = MyOpenGL::MainCameraObject->GetComponent<Camera>();
	if (MainCamera != nullptr)
		MainCamera->mouse_callback(xpos, ypos);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mod)
{
	if (action == GLFW_PRESS)
	{
		switch (button)
		{
		case GLFW_MOUSE_BUTTON_RIGHT:
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			auto MainCamera = MyOpenGL::MainCameraObject->GetComponent<Camera>();
			if (MainCamera != nullptr)
				MainCamera->Mouse_Right_Down = true;
		}
		break;
		default:
			break;
		}
	}
	else if (action == GLFW_RELEASE)
	{
		switch (button)
		{
		case GLFW_MOUSE_BUTTON_RIGHT:
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			auto MainCamera = MyOpenGL::MainCameraObject->GetComponent<Camera>();
			if (MainCamera != nullptr)
			{
				MainCamera->Mouse_Right_Down = false;
				MainCamera->firstMouse = true;
			}
		}
		break;
		default:
			break;
		}
	}

}
void window_reshape_callback(GLFWwindow* window, int width, int height)
{
	glfwGetFramebufferSize(window, &width, &height);
	cout << width << "  " << height << endl;
	glViewport(0, 0, width, height);

}


void MyOpenGL::OpenGLRun()
{
	auto MainCamera = MyOpenGL::MainCameraObject->GetComponent<Camera>();
	if (MainCamera == nullptr)
	{
		throw "未设置主相机";
	}
	




	glfwSetWindowSizeCallback(window, window_reshape_callback);
	
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);


	//================================设置阴影贴图========================================

	glGenFramebuffers(1, &depthMapFBO);
	// create depth texture

	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthMap);
	for (GLuint i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, MainCamera->Width, MainCamera->Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	//======================================================================



	return ;
}
GLfloat lastFrame = glfwGetTime();
int MyOpenGL::OpenGLUpdate(WorldManager* MyWorldManager)
{

	
	if (!glfwWindowShouldClose(window))
	{
		MyWorldManager->deltaTime = glfwGetTime() - lastFrame;
		lastFrame = glfwGetTime();



		DisPlay(window, glfwGetTime(),MyWorldManager);
		glfwSwapBuffers(window);
		glfwPollEvents();
		return 1;
	}
	else
	{
		glfwDestroyWindow(window);
		glfwTerminate();
		return -1;
	}


}





void MyOpenGL::DisPlay(GLFWwindow* window, double currentTime, WorldManager* MyWorldManager)
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);


	glClear(GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);


	//获取主相机
	auto MainCamera = MyOpenGL::MainCameraObject->GetComponent<Camera>();
	glfwGetFramebufferSize(window, &MainCamera->Width, &MainCamera->Height);
	aspect = (float)MainCamera->Width / (float)MainCamera->Height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
	vMat = glm::lookAt(MainCamera->cameraPos, MainCamera->cameraPos + MainCamera->cameraFront, MainCamera->cameraUp);




	DrawSkyBox(MainCamera);
	
	DrawLight(MainCamera, MyWorldManager);
	

	DrawNormalObj(MainCamera);



	
	

}


bool MyOpenGL::AddRenderingObj(Object* obj)
{
	RenderObjs.emplace_back(obj);
	return true;
}


GLuint MyOpenGL::RegTexture(string TextureName, const char* texImagePath)
{

	GLuint textureID = 0;
	textureID = SOIL_load_OGL_texture(texImagePath, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (textureID == 0)
	{
		cout << "加载图片贴图失败" << endl;
		return -1;

	}
	Textures.emplace(TextureName, textureID);

	return textureID;
}

GLuint MyOpenGL::RegTexture(string TextureName, vector<string> Paths)
{
	if (Textures.count(TextureName) > 0||Paths.size() != 6)return -1;
	GLuint textureID = 0;



	textureID = SOIL_load_OGL_cubemap(Paths[0].c_str(), Paths[1].c_str(), Paths[2].c_str(), Paths[3].c_str(), Paths[4].c_str(), Paths[5].c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	if (textureID == 0)
	{
		cout << "创建立方体贴图失败" << endl;
		return -1;
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	Textures.emplace(TextureName, textureID);

	return textureID;
}





//=====================================================




void Camera::key_callback(int key,int action)
{
	
	if (action == GLFW_PRESS)
	{
		
		keys[key] = true;
	}
	else if (action == GLFW_RELEASE)
		keys[key] = false;
}

void Camera::mouse_callback(GLfloat xpos, GLfloat ypos)
{
	if (!Mouse_Right_Down)return;

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos; // 注意这里是相反的，因为y坐标的范围是从下往上的

	

	lastX = xpos;
	lastY = ypos;

	GLfloat sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;
	
	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.z = -cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.x = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	//cout << front.x<<"|"<< cameraFront.x << "   " << front.y<<"|" << cameraFront.y << " " << front.z << "|" << cameraFront.z << endl;
	cameraFront = glm::normalize(front);

}

void Camera::SetW_H(int W, int H)
{
	Width = W;
	Height = H;


}

void Camera::Update(WorldManager* MyWorldManager, Object* MyObject)
{
	auto trans = MyObject->GetComponent<Transform>();

	// 摄像机控制
	GLfloat cameraSpeed = 10.0f * MyWorldManager->deltaTime;
	if (keys[GLFW_KEY_W])
		trans->Position += cameraSpeed * cameraFront;
	if (keys[GLFW_KEY_S])
		trans->Position -= cameraSpeed * cameraFront;
	if (keys[GLFW_KEY_A])
		trans->Position -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (keys[GLFW_KEY_D])
		trans->Position += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;


	cameraPos = trans->Position;
}
