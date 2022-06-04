#include "ObjectSystem.h"

bool Object::RegComponent(ComponentBase* RegC)
{
	Components.emplace_back(RegC);


	return false;
}




bool ObjectSystem::RegObjectList(Object* obj)
{
	ObjectList.emplace_back(obj);
	return false;
}

void ObjectSystem::ObjUpdate()
{

	for (int i = 0; i < ObjectList.size(); i++)
	{
		for (int j = 0; j < ObjectList[i]->Components.size(); j++)
		{
			switch (ObjectList[i]->Components[j]->State)
			{
			case CP_INIT:
			{
				ObjectList[i]->Components[j]->Init();
				ObjectList[i]->Components[j]->State = CP_INIT;
			} 
				break;
			case CP_ACTIVE:
			{
				ObjectList[i]->Components[j]->Update();
			}
			break;
			default:
				break;
			}

		}
	}
}
