#include "Monster.h"
#include <iostream>
#include <fstream>
#include <string>
#include "EntityController.h"

Monster::Monster(float x, float y) : Entity(x, y)
{
	type = EntityType::Monster;
	actionToAnimation[L"Attack"] = L"Attack";
	actionToAnimation[L"Throw"] = L"Throw";

	currentAnimation = L"IdleDown";
	animController.Play(currentAnimation);

	Ifollow = IFollow();
	speed = 12.0f;
	dx = dy = 0;
}

void Monster::Update(float deltaTime)
{
	if (!isAlive) return;

	Ifollow.Update(this, deltaTime);

	if (wantAttack)
	{
		Entity* player = EntityController::GetPlayer();

		if (player)
		{
			MonsterAttackTick(this, player, deltaTime);
		}
	}

	SetAnimation(GetAnimationName());
	Entity::Update(deltaTime);
}

void Monster::Draw(Gdiplus::Graphics& g) {
	if (!isAlive) return;
	Entity::Draw(g);
}

void Monster::Save(ofstream& f) const
{
	Entity::Save(f);
	f.write((char*)&monsterType, sizeof(monsterType));
}

void Monster::Load(ifstream& f)
{
	Entity::Load(f);
	f.read((char*)&monsterType, sizeof(monsterType));
}



