#ifndef __ACMOVE_H__
#define __ACMOVE_H__

#include "ActionNode.h"

class Area;
class BlackBoard;

class AcMove : public ActionNode
{
public:
	AcMove(unsigned int uid, BlackBoard* bb);
	~AcMove();

	void OnStart();
	NODERETURN Run();
	void OnExit();

	void Load(const Data& data);
	void Save(Data& data)const;

	void Print()const;
	string GetNodeHeader()const;

	bool AskParameters();

private:
	Area* area_dst;
	string area_name;
	float delay = 0;
};
#endif // !__ACMOVE_H__

