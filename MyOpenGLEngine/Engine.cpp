#include "Engine.h"


MyEngine* __MyEngine = new MyEngine();

bool MyEngine::EngineInit()
{

    myOpenGL->OpenGLInit(600, 600);







    return false;
}

void MyEngine::EngineRun()
{
    myOpenGL->OpenGLRun();

    int OpenGLState = 1;
    while (true)
    {
        MyObjectSystem->ObjUpdate();
        OpenGLState = myOpenGL->OpenGLUpdate();
        if (OpenGLState == -1)break;

    }
    if (OpenGLState == -1)
    {
        cout << "OpenGL ¹Ø±Õ" << endl;
    }
}

bool MyEngine::AddObject(Object* obj)
{
    auto rp = obj->GetComponent<Rendering>();
    if (rp != nullptr)
    {
        myOpenGL->AddRenderingObj(obj);
    }
    MyObjectSystem->RegObjectList(obj);


    return true;
}

bool MyEngine::LoadShader(string FilePath, GLenum Type)
{
    return myOpenGL->LoadShader(FilePath,Type);
}

GLuint MyEngine::RegTexture(string TextureName, const char* texImagePath)
{
    return myOpenGL->RegTexture(TextureName,texImagePath);
}

bool MyEngine::RegModel(string ModelName, string ModelPath)
{
    return myOpenGL->RegModel(ModelName,ModelPath);
}


