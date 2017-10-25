#ifndef __DECSELECTOR_H__
#define __DECSELECTOR_H__

#include "DecoratorNode.h"

class DecSelector : public DecoratorNode
{
public:
	DecSelector();
	~DecSelector();

	void OnStart();
	NODERETURN Run();
	void OnExit();

private:
	unsigned int current_child = 0; //Current child node
};

#endif // !__DECSELECTOR_H__