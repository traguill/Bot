#ifndef __APLICATION_H__
#define __APLICATION_H__

#include <vector>
#include <string>
#include <mutex>
#include <condition_variable>

using namespace std;

class Editor;
class Input;
class Random;

class Application
{
public:
	Application();
	~Application();

	bool Update(float dt);

	//Blocks console input until Unblock() is called
	void BlockConsole();
	void UnblockConsole();

private:

	bool Menu();

public:
	Random* rnd = nullptr;
	Editor* editor = nullptr;
	Input* input = nullptr;
	bool quit_request = false;

	mutex mtx;
	condition_variable cv;

private:
	bool console_locked = false;
};

//Console commands
void Quit(const vector<string>* args);

extern Application* App;
#endif // !__APLICATION_H__
