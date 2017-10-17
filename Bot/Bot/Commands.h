#ifndef __COMMANDS_H__
#define __COMMANDS_H__

#include <vector>
#include <string>

using namespace std;


//area
void CreateArea(const vector<string>* args); //-c
void ListAreas(const vector<string>* args); //-l
void RemoveArea(const vector<string>* args); //-r


#endif // !__COMMANDS_H__

