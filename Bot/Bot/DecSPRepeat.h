#ifndef __DECSPREPEAT_H__
#define __DECSPREPEAT_H__

#include "DecoratorSPNode.h"

class DecSPRepeat : public DecoratorSPNode
{
public:
	DecSPRepeat(unsigned int uid);
	~DecSPRepeat();

	void OnStart();
	NODERETURN Run();
	void OnExit();

	void Print()const;
	string GetNodeHeader()const;

private:
	
};

#endif // !__DECSPREPEAT_H__

