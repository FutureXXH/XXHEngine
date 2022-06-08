#include "ObjectSystem.h"

bool Object::RegComponent(ComponentBase* RegC)
{
	Components.emplace_back(RegC);


	return true;
}
