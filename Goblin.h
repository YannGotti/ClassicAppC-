#pragma once
#include "Monster.h"
#include <map>
#include <windows.h>       
#include <objidl.h>        
#include <gdiplus.h>       
#pragma comment (lib,"Gdiplus.lib")

using namespace std;
using namespace Gdiplus;


class Goblin : public Monster
{
public:
	Goblin(int x, int y);

private:
	map<wstring, wstring> actionToAnimation;
};
