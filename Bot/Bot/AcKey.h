#ifndef __ACKEY_H__
#define __ACKEY_H__

#include "ActionNode.h"
#include "KeyboardEmulator.h"

class AcKey : public ActionNode
{
public:
	AcKey(unsigned int uid, BlackBoard* bb);
	~AcKey();

	void OnStart();
	NODERETURN Run();
	void OnExit();

	void Load(const Data& data);
	void Save(Data& data)const;

	void Print()const;
	string GetNodeHeader()const;

	bool AskParameters();

private:
	SPECIAL_KEY key;
	string key_string;
};
#endif // !__ACKEY_H__
