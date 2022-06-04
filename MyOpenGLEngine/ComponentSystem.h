#ifndef __COMPONENTSYSTEM_H
#define __COMPONENTSYSTEM_H
#include <vector>
#include <string>
using namespace std;


#define CP_INIT 1
#define CP_ACTIVE 2

class ComponentBase
{

public:
	short State = CP_INIT;
	virtual void Init(){};
	virtual void Update(){};

	


};







class Transform :public ComponentBase
{
public:
	float PosX = 0, PosY = 0, PosZ = 0;


};






#endif // !__COMPONENTSYSTEM_H


