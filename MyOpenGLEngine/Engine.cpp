#include "Engine.h"


MyEngine* __MyEngine = new MyEngine();


bool MyEngine::EngineInit()
{

    myOpenGL->OpenGLInit();







    return false;
}

void MyEngine::EngineRun()
{
    myOpenGL->OpenGLRun();

    int OpenGLState = 1;
    while (true)
    {
       MyWorldManager->WorldUpdate();
        OpenGLState = myOpenGL->OpenGLUpdate(MyWorldManager);


        if (OpenGLState == -1)break;

    }
    if (OpenGLState == -1)
    {
        cout << "OpenGL ¹Ø±Õ" << endl;
    }
}

bool MyEngine::SetMainCamera(Object* setc)
{
    if (setc == nullptr)return false;
    myOpenGL->SetMainCamera(setc);

    return true;
}

bool MyEngine::SetSkyBox(vector<string> paths)
{
    return myOpenGL->SetSkyBox(paths);
}

bool MyEngine::AddObject(Object* obj)
{
    auto rp = obj->GetComponent<Rendering>();
    if (rp != nullptr)
    {
        myOpenGL->AddRenderingObj(obj);
    }
    MyWorldManager->RegObject(obj);

    return true;
}

Shader* MyEngine::CreateShader(string ShaderName, string vertexPath, string fragmentPath,string geometryPath)
{
   
    return  myOpenGL->RegShader(ShaderName,vertexPath,fragmentPath,geometryPath);
}



Shader* MyEngine::GetShader(string ShaderName)
{
    return myOpenGL->GetShader(ShaderName);
}



GLuint MyEngine::RegTexture(string TextureName, const char* texImagePath)
{
    return myOpenGL->RegTexture(TextureName,texImagePath);
}

GLuint MyEngine::RegTexture(string TextureName, vector<string> Paths)
{
    return  myOpenGL->RegTexture(TextureName, Paths);
}

bool MyEngine::RegModel(string ModelName, string ModelPath)
{
    return myOpenGL->RegModel(ModelName,ModelPath);
}


