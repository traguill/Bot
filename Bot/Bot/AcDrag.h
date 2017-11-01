#ifndef __ACDRAG_H__
#define __ACDRAG_H__

#include "ActionNode.h"

class Area;
class BlackBoard;

class AcDrag : public ActionNode
{
public:
	AcDrag(unsigned int uid, BlackBoard* bb);
	~AcDrag();

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
#endif // !__ACDRAG_H__
