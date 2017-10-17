#ifndef __COMMANDS_H__
#define __COMMANDS_H__

#include <vector>
#include <string>

using namespace std;

//clear
void ClearConsoleScreen(const vector<string>* args);

//area
void CreateArea(const vector<string>* args); //-c
void ListAreas(const vector<string>* args); //-l
void RemoveArea(const vector<string>* args); //-r
void EditArea(const vector<string>* args); //-e
void RedefineAreaSpace(const string& area_name); //helper


#endif // !__COMMANDS_H__

