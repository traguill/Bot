#include "BehaviorTree.h"

#include "ConsoleMsgs.h"
#include "BlackBoard.h"
#include "ModuleFileSystem.h"
#include "Application.h"

#include "DecSequence.h"
#include "DecSelector.h"

BehaviorTree::BehaviorTree()
{
}

BehaviorTree::~BehaviorTree()
{
	if (bb)
		delete bb;
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

bool BehaviorTree::InsertNode(const string & type, const string & sub_type)
{
	bool ret = false;

	if (type.compare(type_decorator) == 0) //Decorator
	{
		ret = InsertDecorator(sub_type);
	}
	else if (type.compare(type_action) == 0)
	{
		//Action
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

	size_t size_nodes = data.GetArraySize("nodes"); //Size nodes should always be 1. Only one root
	if (size_nodes > 1)
		MSG_WARNING("This BT has more than one root node!");

	current_node = LoadNode(data.GetArray("nodes", 0), nullptr);
	root = current_node;

	if (current_node == nullptr)
		MSG_WARNING("This BT doesn't have any nodes");

	if (buf)
		delete[] buf;

	bb = new BlackBoard();
	bb->Init(bb_filename.data());

	return true;
}

void BehaviorTree::Save() const
{
	Data data;

	data.AppendString("bb_name", bb_filename.data());
	data.AppendUInt("last_uid", last_uid);

	data.AppendArray("nodes");
	SaveNode(data, current_node);

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
		TreeNode* child_ret = ret;
		size_t childs = data.GetArraySize("childs");
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

bool BehaviorTree::InsertDecSequence()
{
	bool ret = false;
	
	unsigned int id = GetNewNodeUid();
	DecSequence* node = new DecSequence(id);

	if (current_node == nullptr)
	{
		current_node = node;
		ret = true;
		
		header_current_node = "/[D]Sequence(" + std::to_string(id);
		header_current_node.append(")/");
	}
	else
	{
		ret = current_node->AddChild(node);
		if (!ret && node)
			delete node;
	}

	return ret;
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
	bool ret = false;
	unsigned int id = GetNewNodeUid();
	DecSelector* node = new DecSelector(id);

	if (current_node == nullptr)
	{
		current_node = node;
		ret = true;
		header_current_node = "/[D]Selector(" + std::to_string(id);
		header_current_node.append(")/");
	}
	else
	{
		ret = current_node->AddChild(node);
		if (!ret && node)
			delete node;
	}

	return ret;
}

TreeNode * BehaviorTree::InsertDecSelector(unsigned int uid, TreeNode * parent)
{
	DecSelector* node = new DecSelector(uid);
	if(node)
		parent->AddChild(node);

	return node;
}
