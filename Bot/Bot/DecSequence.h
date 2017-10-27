#ifndef __DECSEQUENCE_H__
#define __DECSEQUENCE_H__

#include "DecoratorNode.h"

class DecSequence : public DecoratorNode
{
public:
	DecSequence(unsigned int uid);
	~DecSequence();

	void OnStart();
	NODERETURN Run();
	void OnExit();

private:
	unsigned int current_child = 0; //Current child node
};
#endif // !__DECSEQUENCE_H__

