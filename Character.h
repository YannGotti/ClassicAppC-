#pragma once
#include "Entity.h"
#include "IAttack.h"
#include <map>
#include <windows.h>       
#include <objidl.h>        
#include <gdiplus.h>       
#include "EnumsEntity.h"
#pragma comment (lib,"Gdiplus.lib")

using namespace std;
using namespace Gdiplus;

class Character : public Entity, public IAttack
{
public:
	Character(int x = 0, int y = 0);
	void Init();

	void Update(float deltaTime) override;
	void Draw(Gdiplus::Graphics& g) override;
	void PerformAction(const wstring& actionName);

	void DropItem();

private:
	map<wstring, wstring> actionToAnimation;
	Inventory inventoty = Inventory();
};

