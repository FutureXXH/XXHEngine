#include "WorldManager.h"

Object* WorldManager::FindObject(string Name)
{
	if (WorldObject.count(Name) <= 0)
	{
		return nullptr;
	}

	return WorldObject[Name];
}

bool WorldManager::RegObject(Object* obj)
{
	if (WorldObject.count(obj->name) > 0)
	{
		cout << "对象注册失败 已存在相同名称" << endl;
		return false;
	}
	WorldObject.emplace(obj->name, obj);
	return false;
}

void WorldManager::WorldUpdate()
{
	for (auto it = WorldObject.begin();it != WorldObject.end();it++)
	{
		
		for (int j = 0; j < it->second->Components.size(); j++)
		{
			ComponentBase* CP = it->second->Components[j];
			if (CP == nullptr)continue;

			switch (CP->State)
			{
			case CP_INIT:
			{
				CP->Init(this, it->second);
				CP->State = CP_ACTIVE;
			}
			break;
			case CP_ACTIVE:
			{
				CP->Update(this,it->second);
			}
			break;
			default:
				break;
			}

		}
	}
}
