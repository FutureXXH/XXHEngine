#ifndef __OBJECTSYSTEM_H
#define __OBJECTSYSTEM_H

#include "ComponentSystem.h"

class ObjectSystem;


class Object
{
	
	vector<ComponentBase*> Components;
	friend class ObjectSystem;
public:

	string name;

	bool RegComponent(ComponentBase* RegC);
	template<class T>
	T* GetComponent();


};

class ObjectSystem
{
private:
	vector<Object*> ObjectList;

public:
	bool RegObjectList(Object* obj);

	void ObjUpdate();



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
