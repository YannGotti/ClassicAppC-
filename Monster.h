#pragma once
#include "Entity.h"
#include <map>
#include <windows.h>       
#include <objidl.h>        
#include <gdiplus.h>       
#pragma comment (lib,"Gdiplus.lib")
#include "IFollow.h"
#include "IAttack.h"

using namespace std;
using namespace Gdiplus;


class Monster : public Entity, public IAttack
{
public:

	Monster(float x = 0, float y = 0);
	void Update(float deltaTime) override;
	void Draw(Gdiplus::Graphics& g) override;

	MonsterType GetMonsterType() const { return monsterType; }

	void Save(ofstream& f) const override;
	void Load(ifstream& f) override;

	IFollow Ifollow;
protected:
	MonsterType monsterType = MonsterType::None;
	map<wstring, wstring> actionToAnimation;
	Inventory inventoty = Inventory(1);
};
