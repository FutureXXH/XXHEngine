#ifndef __OBJECTSYSTEM_H
#define __OBJECTSYSTEM_H

#include <vector>
#include <string>


using namespace std;
#define CP_INIT 1
#define CP_ACTIVE 2
class WorldManager;
class Object;

class ComponentBase
{

public:
	short State = CP_INIT;
	virtual void Init(WorldManager* MyWorldManager, Object* MyObject) {};
	virtual void Update(WorldManager* MyWorldManager, Object* MyObject) {};




};

class Script : public ComponentBase
{
public:
	void (*UpdateFun)(WorldManager* MyWorldManager, Object* MyObject);
	virtual void Update(WorldManager* MyWorldManager, Object* MyObject);

	void (*InitFun)(WorldManager* MyWorldManager, Object* MyObject);
	virtual void Init(WorldManager* MyWorldManager, Object* MyObject);
};



class Transform :public ComponentBase
{
public:
	float PosX = 0, PosY = 0, PosZ = 0;


};


//===================================================








class Object
{
private:

	vector<ComponentBase*> Components;
	friend class WorldManager;
public:

	string name;

	bool RegComponent(ComponentBase* RegC);
	template<class T>
	T* GetComponent();


};




template<class T>
inline T* Object::GetComponent()
{
	for (int i = 0; i < Components.size(); i++)
	{
		T* p = dynamic_cast<T*>(Components[i]);
		if (p != nullptr)
		{
			return p;
		}
	}
	return nullptr;
}




#endif // !__OBJECTSYSTEM_H
