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

GLuint MyOpenGL::LoadShader(string FilePath,GLenum Type)
{
	string temp = readShaderSource(FilePath);
	const char* shaderSourace = temp.c_str();
	GLuint Shader = glCreateShader(Type);
	glShaderSource(Shader, 1, &shaderSourace, NULL);

	glCompileShader(Shader);
	checkOpenGLError();
	GLint vertCompiled;
	glGetShaderiv(Shader, GL_COMPILE_STATUS, &vertCompiled);
	if (vertCompiled != 1)
	{
		cout << " compilation failed" << endl;
		printShaderLog(Shader);
		return -1;
	}
	glAttachShader(renderingProgram, Shader);
	cout << "创建Shader：" << FilePath << "  " << Shader <<endl;
	return Shader;
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


	renderingProgram = glCreateProgram();


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

	glLinkProgram(renderingProgram);
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
int MyOpenGL::OpenGLUpdate()
{

	
	if (!glfwWindowShouldClose(window))
	{
		GLfloat deltaTime = glfwGetTime() - lastFrame;
		lastFrame = glfwGetTime();



		MyCamera.do_movement(deltaTime);
		DisPlay(window, glfwGetTime());
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



void MyOpenGL::DisPlay(GLFWwindow* window, double currentTime)
{
	glClear(GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(renderingProgram);
	
	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");
	GLuint textureSampleLoc = glGetUniformLocation(renderingProgram, "textureSample");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	vMat = glm::lookAt(MyCamera.cameraPos, MyCamera.cameraPos + MyCamera.cameraFront, MyCamera.cameraUp);
	//vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));


	for (int i = 0; i < RenderObjs.size(); i++)
	{
		Object* CurModel = RenderObjs[i];
		if (CurModel == nullptr)continue;


		auto CurRenderingModel = CurModel->GetComponent<Rendering>();
		if (CurRenderingModel == nullptr)continue;


		if (Models.count(CurRenderingModel->ModelName) == 0)continue;
		auto  Modelp = Models[CurRenderingModel->ModelName];

		if (CurModel == nullptr)continue;
		auto CurModelTransform = CurModel->GetComponent<Transform>();



		mMat = glm::translate(glm::mat4(1.0f), glm::vec3(CurModelTransform->PosX, CurModelTransform->PosY, CurModelTransform->PosZ));

		//mMat *= glm::translate(glm::mat4(1.0f), glm::vec3(sin((float)currentTime) * 5.0,0.0 , cos((float)currentTime) * 5.0));
		mMat *= glm::rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(1.0, 0.0, 0.0));
		mvMat = vMat * mMat;
		glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));


		glBindBuffer(GL_ARRAY_BUFFER, Modelp->vbo[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, Modelp->vbo[1]);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);
		
		if (Textures.count(CurRenderingModel->TextureName) > 0)
		{
			glUniform1i(textureSampleLoc, 1);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, Textures[CurRenderingModel->TextureName]);
		}
		else
		{
			glUniform1i(textureSampleLoc, 0);
		}

		glEnable(GL_DEPTH_TEST);

		glDepthFunc(GL_LEQUAL);

		glDrawArrays(GL_TRIANGLES, 0, Modelp->getNumVertices());







	}



	

}

void MyOpenGL::printShaderLog(GLuint shader)
{
	int len = 0;
	int chWrittn = 0;
	char* log;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
	if (len > 0)
	{
		log = new char[len];
		glGetShaderInfoLog(shader, len, &chWrittn, log);
		cout << shader << " Shader Info Log : " << log << endl;
		delete[]log;
	}
}


void MyOpenGL::printProgramLog(int Prog)
{


	int len = 0;
	int chWrittn = 0;
	char* log;
	glGetShaderiv(Prog, GL_INFO_LOG_LENGTH, &len);
	if (len > 0)
	{
		log = new char[len];
		glGetShaderInfoLog(Prog, len, &chWrittn, log);
		cout << " Program Info Log : " << log << endl;
		delete[]log;
	}
}

bool  MyOpenGL::checkOpenGLError()
{
	bool foundError = false;
	int glErr = glGetError();
	while (glErr != GL_NO_ERROR)
	{
		cout << "gl ERROR:" << glErr << endl;
		foundError = true;
		glErr = glGetError();
	}
	return foundError;
}


string  MyOpenGL::readShaderSource(string filePath)
{
	string content = "";
	ifstream fileStream(filePath.c_str(), ios::in);
	string line = "";
	while (!fileStream.eof())
	{
		getline(fileStream, line);
		content += (line + "\n");
	}
	fileStream.close();
	
	return content;
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
	GLfloat cameraSpeed = 5.0f * deltaTime;
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
