#ifndef __WORLDMANAGER_H
#define __WORLDMANAGER_H
#include "ObjectSystem.h"
#include <vector>
#include<unordered_map>
#include<string>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
//世界管理器类
class WorldManager
{
private:
	unordered_map<string, Object*> WorldObject;
	
public:
	GLfloat deltaTime;
	Object* FindObject(string Name);

	bool RegObject(Object* obj);

	void WorldUpdate();
};



#endif // !__WORLDMANAGER_H
