#include "ObjectSystem.h"

void Script::Update(WorldManager* MyWorldManager, Object* MyObject)
{
	if (UpdateFun != nullptr)
	{
		UpdateFun(MyWorldManager, MyObject);
	}
}

void Script::Init(WorldManager* MyWorldManager, Object* MyObject)
{
	if (InitFun != nullptr)
	{
		InitFun(MyWorldManager, MyObject);
	}
}
