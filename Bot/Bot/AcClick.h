#ifndef __ACCLICK_H__
#define __ACCLICK_H__

#include "ActionNode.h"

class AcClick : public ActionNode
{
public:
	AcClick(unsigned int uid, BlackBoard* bb);
	~AcClick();

	void OnStart();
	NODERETURN Run();
	void OnExit();

	void Load(const Data& data);
	void Save(Data& data)const;

	void Print()const;
	string GetNodeHeader()const;

	bool AskParameters();

private:
	//TODO: Humanize the click. Av human click duration 0.1-0.12s. Dif types: fast, normal, slow.
	float delay = 0.0f;

};
#endif // !__ACCLICK_H__
