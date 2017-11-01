#ifndef __ACWRITE_H__
#define __ACWRITE_H__

#include "ActionNode.h"

class AcWrite : public ActionNode
{
public:
	AcWrite(unsigned int uid, BlackBoard* bb);
	~AcWrite();

	void OnStart();
	NODERETURN Run();
	void OnExit();

	void Load(const Data& data);
	void Save(Data& data)const;

	void Print()const;
	string GetNodeHeader()const;

	bool AskParameters();

private:
	string text;

};
#endif // !__AcWrite_H__
