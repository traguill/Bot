#ifndef __COMMANDS_H__
#define __COMMANDS_H__

#include <vector>
#include <string>

using namespace std;

bool CheckNumParameters(const vector<string>* args, int min_number, int max_number, const char* function_name, char option);

//clear
void ClearConsoleScreen(const vector<string>* args);

//area
void CreateArea(const vector<string>* args); //-c
void ListAreas(const vector<string>* args); //-l
void RemoveArea(const vector<string>* args); //-r
void EditArea(const vector<string>* args); //-e
void RedefineAreaSpace(const string& area_name); //helper
void ShowArea(const vector<string>* args); //-s

//move
void MoveMouseToArea(const vector<string>* args); // <area> <delay>(Opt)

//bb
void BBShow(const vector<string>* args); //-s <name>
void BBInsert(const vector<string>* args); //-i <type> <name> <value>

//bt
void BTGoToNode(const vector<string>* args); // <node_uid>
void BTNew(const vector<string>* args); //-n <name>
void ShowBTs(const vector<string>* args); //-s
void BTEdit(const vector<string>* args); //-e <name> / -e quit
void BTInsert(const vector<string>* args); //-i
void BTList(const vector<string>* args); //-l
void BTRemove(const vector<string>* args); //-r <uid(Opt)>

//run
void RunBT(const vector<string>* args); //<bt_name>

//pause (debug only) may crash or something i dunno
void Pause(const vector<string>* args);


//hearthstone
void HSCardInfo(const vector<string>* args); // <id>
void HSLogTrack(const vector<string>* args); // -t
#endif // !__COMMANDS_H__

