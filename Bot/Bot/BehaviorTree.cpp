#include "BehaviorTree.h"

#include "ConsoleMsgs.h"
#include "BlackBoard.h"
#include "ModuleFileSystem.h"
#include "Application.h"

#include "DecSequence.h"
#include "DecSelector.h"

#include "AcMove.h"
#include "AcClick.h"

#include <stack>
#include <queue>

BehaviorTree::BehaviorTree()
{
}

BehaviorTree::~BehaviorTree()
{
	if (bb)
		delete bb;

	//TODO delete nodes
}

void BehaviorTree::Init(const char * filename, const char* name)
{
	if (filename == nullptr) //Just created
	{
		if (name == nullptr)
		{
			MSG_ERROR("A BehaviorTree could not be initialize. No filename nor name");
			return;
		}

		bt_filename = name; bt_filename.append(".bt");
		bb_filename = name; bb_filename.append(".bb");//Maybe I shouldn't let the user change the name of the bb. This could bring trouble...

		bb = new BlackBoard();
		bb->Init(NULL, bb_filename.data());

		Save();
	}
	else //Load from file
	{
		bt_filename = filename; bt_filename.append(".bt");
		Load();
	}
}

bool BehaviorTree::Run()
{
	NODERETURN ret = root->Run();
	return (ret == RUNNING) ? false : true;
}

bool BehaviorTree::InsertNode(const string & type, const string & sub_type)
{
	bool ret = false;

	if (type.compare(type_decorator) == 0) //Decorator
	{
		ret = InsertDecorator(sub_type);
	}
	else if (type.compare(type_action) == 0) //Action
	{
		ret = InsertAction(sub_type);
	}
	else if (type.compare(type_condition) == 0)
	{
		//condition
	}
	else if (type.compare(type_decorator_sp) == 0)
	{
		//decorator sp
	}
	else
	{
		MSG_WARNING("Type %s is not a valid type", type.data());
	}

	return ret;
}

TreeNode* BehaviorTree::InsertNode(NODETYPE type, NODESUBTYPE subtype, unsigned int uid, TreeNode* parent)
{
	TreeNode* ret = nullptr;

	switch (type)
	{
	case ACTION:
		ret = InsertAction(subtype, uid, parent);
		break;
	case CONDITION:
		break;
	case DECORATOR:
		ret = InsertDecorator(subtype, uid, parent);
		break;
	case DECORATOR_SP:
		break;
	default:
		MSG_ERROR("Type %i is not a valid type", type);
		break;
	}
	return ret;
}

void BehaviorTree::RemoveNode(unsigned int uid)
{
	TreeNode* node = FindNodeById(uid);
	if (node == nullptr)
		return;

	RemoveNode(node);
}

void BehaviorTree::RemoveNode(TreeNode * node)
{
	if (node == nullptr)
		return;

	stack<TreeNode*> stack;
	stack.push(node);
	TreeNode* parent = node->GetParent();
	if (parent)
	{
		parent->RemoveChild(node);
	}
	
	if (root == node)
		root = current_node = nullptr;
	if (current_node == node)
		current_node = parent;

	TreeNode* item = nullptr;
	while (stack.empty() == false)
	{
		item = stack.top();
		stack.pop();
		if (item->HasChilds())
		{
			const vector<TreeNode*> childs = item->GetChilds();
			for (vector<TreeNode*>::const_iterator it = childs.begin(); it != childs.end(); ++it)
				stack.push(*it);
		}

		delete item;
	}

	if (current_node)
		SetCurrentNode(current_node);
	else
		header_current_node.clear();
}

bool BehaviorTree::Load()
{
	char* buf = nullptr;
	size_t size = App->file_system->Load(bt_filename.data(), &buf);
	if (size <= 0)
	{
		MSG_ERROR("Could not load BehaviorTree %s", bt_filename.data());
		if (buf)
			delete[] buf;
		return false;
	}

	Data data(buf);

	bb_filename = data.GetString("bb_name");
	last_uid = data.GetUInt("last_uid");

	bb = new BlackBoard();
	bb->Init(bb_filename.data());

	size_t size_nodes = data.GetArraySize("nodes"); //Size nodes should always be 1. Only one root
	if (size_nodes > 1)
		MSG_WARNING("This BT has more than one root node!");

	current_node = LoadNode(data.GetArray("nodes", 0), nullptr);
	root = current_node;

	if (current_node == nullptr)
		MSG_WARNING("This BT doesn't have any nodes");
	else
		header_current_node.append(current_node->GetNodeHeader());

	if (buf)
		delete[] buf;

	return true;
}

void BehaviorTree::Save() const
{
	Data data;

	data.AppendString("bb_name", bb_filename.data());
	data.AppendUInt("last_uid", last_uid);

	data.AppendArray("nodes");
	SaveNode(data, root);

	char* buf = nullptr;
	size_t size = data.Serialize(&buf);

	if (size > 0)
	{
		size = App->file_system->Save(bt_filename.data(), buf, size);
		if (size <= 0)
		{
			MSG_ERROR("Error while saving the BT %s", bt_filename.data());
		}
	}
	else
	{
		MSG_ERROR("Error while serializing the BT %s", bt_filename.data());
	}
	
	if (buf)
		delete[] buf;
}

void BehaviorTree::PrintChildNodes() const
{
	if (current_node == nullptr)
	{
		MSG_WARNING("This BT has no nodes");
		return;
	}

	const vector<TreeNode*>childs = current_node->GetChilds();
	if (childs.size() == 0)
	{
		MSG_INFO("This node has no childs");
		return;
	}

	for (vector<TreeNode*>::const_iterator child = childs.begin(); child != childs.end(); ++child)
	{
		(*child)->Print();
	}
}

bool BehaviorTree::SetCurrentNode(TreeNode * node)
{
	if (node == nullptr)
		return false;

	current_node = node;
	header_current_node.clear();

	TreeNode* item = node->GetParent();

	if (item == nullptr)
	{
		header_current_node.append(node->GetNodeHeader());
		return true;
	}

	queue<TreeNode*> fifo;
	while (item != nullptr)
	{
		fifo.push(item);
		item = item->GetParent();
	}

	while (fifo.empty() == false)
	{
		header_current_node.append(fifo.front()->GetNodeHeader());
		fifo.pop();
	}
	header_current_node.append(node->GetNodeHeader());

	return true;
}

TreeNode * BehaviorTree::FindNodeById(unsigned int uid) const
{
	if (root == nullptr)
		return nullptr;

	stack<TreeNode*> stack;
	stack.push(root);

	TreeNode* item;
	while (stack.empty() == false)
	{
		item = stack.top();
		stack.pop();

		if (item->GetUid() == uid)
			return item;

		const vector<TreeNode*> childs = item->GetChilds();
		for (vector<TreeNode*>::const_iterator child = childs.begin(); child != childs.end(); ++child)
			stack.push(*child);
	}

	return nullptr;
}

TreeNode * BehaviorTree::GetCurrentNode() const
{
	return current_node;
}

TreeNode * BehaviorTree::GetRootNode() const
{
	return root;
}

void BehaviorTree::SaveNode(Data & data, TreeNode * node) const
{
	if (node)
	{
		Data d_node;

		NODETYPE type;
		NODESUBTYPE subtype;
		node->GetTypeSubType(type, subtype);
		d_node.AppendInt("type", type);
		d_node.AppendInt("subtype", subtype);
		d_node.AppendUInt("uid", node->GetUid());
		d_node.AppendArray("childs");
		if (type == ACTION)
			((ActionNode*)node)->Save(d_node);
		if (node->HasChilds())
		{
			const vector<TreeNode*> childs = node->GetChilds();
			for (vector<TreeNode*>::const_iterator child = childs.begin(); child != childs.end(); ++child)
			{
				SaveNode(d_node, *child);
			}
		}

		data.AppendArrayValue(d_node);
	}
}

TreeNode* BehaviorTree::LoadNode(Data & data, TreeNode* parent)
{
	TreeNode* ret = nullptr;

	NODETYPE type = (NODETYPE)data.GetInt("type");
	NODESUBTYPE subtype = (NODESUBTYPE)data.GetInt("subtype");
	unsigned int uid = data.GetUInt("uid");

	ret = InsertNode(type, subtype, uid, parent);

	if (ret == nullptr)
	{
		MSG_ERROR("Error while loading the node %i. The data might be corrupted", uid);
	}
	else
	{
		if (type == ACTION)
			((ActionNode*)ret)->Load(data);
		TreeNode* child_ret = ret;
		int childs = data.GetArraySize("childs");
		for (int i = 0; i < childs; ++i)
		{
			child_ret = LoadNode(data.GetArray("childs", i), ret);
			if (child_ret == nullptr)
				break;
		}
		if (child_ret == nullptr)
		{
			MSG_ERROR("Error while loading a child of node %i", uid);
			return nullptr;
		}
	}
	
	return ret;
}

unsigned int BehaviorTree::GetNewNodeUid()
{
	return ++last_uid;
}

bool BehaviorTree::InsertDecorator(const string & sub_type)
{
	bool ret = false;
	if (sub_type.compare(dec_sequence) == 0) // Sequence
	{
		ret = InsertDecSequence();
	}
	else if (sub_type.compare(dec_selector) == 0) //Selector
	{
		ret = InsertDecSelector();
	}
	else
	{
		MSG_WARNING("Sub-type: %s is not valid", sub_type.data());
	}
	return ret;
}

TreeNode* BehaviorTree::InsertDecorator(NODESUBTYPE subtype, unsigned int uid, TreeNode* parent)
{
	TreeNode* ret = nullptr;
	switch (subtype)
	{
	case DECSELECTOR:
		ret = InsertDecSelector(uid, parent);
		break;
	case DECSEQUENCE:
		ret = InsertDecSequence(uid, parent);
		break;
	default:
		MSG_ERROR("Subtype: %i is not valid", subtype);
		break;
	}
	return ret;
}

bool BehaviorTree::InsertAction(const string & sub_type)
{
	bool ret = false;
	if (sub_type.compare(ac_move) == 0) // move
	{
		ret = InsertAcMove();
	}
	else if (sub_type.compare(ac_click) == 0)
	{
		ret = InsertAcClick();
	}
	else
	{
		MSG_WARNING("Sub-type: %s is not valid", sub_type.data());
	}
	return ret;
}

TreeNode * BehaviorTree::InsertAction(NODESUBTYPE subtype, unsigned int uid, TreeNode * parent)
{
	TreeNode* ret = nullptr;
	switch (subtype)
	{
	case ACMOVE:
		ret = InsertAcMove(uid, parent);
		break;
	case ACCLICK:
		ret = InsertAcClick(uid, parent);
		break;
	default:
		MSG_ERROR("Subtype: %i is not valid", subtype);
		break;
	}
	return ret;
}

bool BehaviorTree::HandleInsertion(TreeNode * node)
{
	bool ret;
	if (root == nullptr)
	{
		root = current_node = node;
		ret = true;
		header_current_node.append(current_node->GetNodeHeader());
	}
	else
	{
		ret = current_node->AddChild(node);
		if (!ret && node)
			delete node;
	}
	return ret;
}

bool BehaviorTree::InsertDecSequence()
{
	DecSequence* node = new DecSequence(GetNewNodeUid());
	return HandleInsertion(node);
}

TreeNode * BehaviorTree::InsertDecSequence(unsigned int uid, TreeNode * parent)
{
	DecSequence* node = new DecSequence(uid);
	if (node && parent)
		parent->AddChild(node);
	return node;
}

bool BehaviorTree::InsertDecSelector()
{
	DecSelector* node = new DecSelector(GetNewNodeUid());
	return HandleInsertion(node);
}

TreeNode * BehaviorTree::InsertDecSelector(unsigned int uid, TreeNode * parent)
{
	DecSelector* node = new DecSelector(uid);
	if(node && parent)
		parent->AddChild(node);
	return node;
}

bool BehaviorTree::InsertAcMove()
{
	AcMove* node = new AcMove(GetNewNodeUid(), bb);
	bool ret = HandleInsertion(node);
	if (ret)
		ret = node->AskParameters(); //TODO: Remove the node if this returns false
	return ret;
}

TreeNode * BehaviorTree::InsertAcMove(unsigned int uid, TreeNode * parent)
{
	AcMove* node = new AcMove(uid, bb);
	if (node && parent)
		parent->AddChild(node);
	return node;
}

bool BehaviorTree::InsertAcClick()
{
	AcClick* node = new AcClick(GetNewNodeUid(), bb);
	bool ret = HandleInsertion(node);
	if (ret)
		ret = node->AskParameters(); //TODO: Remove the node if this returns false
	return ret;
}

TreeNode * BehaviorTree::InsertAcClick(unsigned int uid, TreeNode * parent)
{
	AcClick* node = new AcClick(uid, bb);
	if (node && parent)
		parent->AddChild(node);
	return node;
}
