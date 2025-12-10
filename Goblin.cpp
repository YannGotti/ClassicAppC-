#include "Goblin.h"


Goblin::Goblin(int x, int y) : Monster(x, y)
{
	type = EntityType::Monster;
	monsterType = MonsterType::Goblin;
	actionToAnimation[L"Attack"] = L"Attack";
	actionToAnimation[L"Throw"] = L"Throw";

	animController.LoadAnimations(L"Assets/Goblin");

	currentAnimation = L"IdleDown";
	animController.Play(currentAnimation);
}