#include "MyOpenGL.h"


Camera MyCamera;



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

Shader* MyOpenGL::GetShader(string Name)
{
	if (Shaders.count(Name) > 0)
	{
		return Shaders[Name];
	}

	return nullptr;
}

Shader* MyOpenGL::RegShader(string Name, const char* vertexPath, const char* fragmentPath)
{
	if (Shaders.count(Name) > 0)
	{
		cout << "Shader :" << Name << "重名" << endl;
		return nullptr;
	}

	Shader* newshader = new Shader();
	if (!newshader->Init(vertexPath, fragmentPath))
	{
		cout << "Shader :" << Name << "创建失败" << endl;
		delete newshader;
		return nullptr;
	}


	Shaders.emplace(Name, newshader);


	return newshader;
}



void MyOpenGL::window_reshape_callback(GLFWwindow* window, int newWidth, int newHeight)
{
	glfwGetFramebufferSize(window, &newWidth, &newHeight);
	aspect = (float)newWidth / (float)newHeight;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
}



bool MyOpenGL::OpenGLInit(int setwidth, int setheight)
{
	width = setwidth;
	height = setheight;
	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);



	window = glfwCreateWindow(width, height, "Hello OpenGL", NULL, NULL);
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
	MyCamera.key_callback(key, action);
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

	MyCamera.mouse_callback(xpos, ypos);
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
			MyCamera.Mouse_Right_Down = true;
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
			MyCamera.Mouse_Right_Down = false;
			MyCamera.firstMouse = true;
		}
		break;
		default:
			break;
		}
	}

}

void MyOpenGL::OpenGLRun()
{

	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);

	//glfwSetWindowSizeCallback(window, window_reshape_callback);
	
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);


	return ;
}
GLfloat lastFrame = glfwGetTime();
int MyOpenGL::OpenGLUpdate(WorldManager* MyWorldManager)
{

	
	if (!glfwWindowShouldClose(window))
	{
		MyWorldManager->deltaTime = glfwGetTime() - lastFrame;
		lastFrame = glfwGetTime();



		MyCamera.do_movement(MyWorldManager->deltaTime);
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
	glClear(GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);
	vMat = glm::lookAt(MyCamera.cameraPos, MyCamera.cameraPos + MyCamera.cameraFront, MyCamera.cameraUp);
	//vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));


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

		if (CurShader->update != nullptr)
			CurShader->update(CurShader, currentTime, MyWorldManager);


	


		mMat = glm::translate(glm::mat4(1.0f), glm::vec3(CurModelTransform->PosX, CurModelTransform->PosY, CurModelTransform->PosZ));

		//mMat *= glm::translate(glm::mat4(1.0f), glm::vec3(sin((float)currentTime) * 5.0,0.0 , cos((float)currentTime) * 5.0));
		//mMat *= glm::rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(1.0, 0.0, 0.0));






		CurShader->SetMatrix4fv("m_matrix", mMat);
		CurShader->SetMatrix4fv("v_matrix", vMat);
		CurShader->SetMatrix4fv("proj_matrix", pMat);
		CurShader->SetVec3("viewPos", MyCamera.cameraPos.x, MyCamera.cameraPos.y, MyCamera.cameraPos.z);

		glBindBuffer(GL_ARRAY_BUFFER, Modelp->vbo[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, Modelp->vbo[1]);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, Modelp->vbo[2]);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(2);
		
		if (Textures.count(CurRenderingModel->TextureName) > 0)
		{
			glUniform1i(glGetUniformLocation(CurShader->ID, "textureSample"), 1);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, Textures[CurRenderingModel->TextureName]);
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





//=====================================================

void Camera::do_movement(GLfloat deltaTime)
{

		// 摄像机控制
	GLfloat cameraSpeed = 10.0f * deltaTime;
		if (keys[GLFW_KEY_W])
			cameraPos += cameraSpeed * cameraFront;
		if (keys[GLFW_KEY_S])
			cameraPos -= cameraSpeed * cameraFront;
		if (keys[GLFW_KEY_A])
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		if (keys[GLFW_KEY_D])
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	
}


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
