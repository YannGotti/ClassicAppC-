#pragma once

enum class Direction { Up, Down, Left, Right };
enum class State { Idle, Walking, Attacking, Damage };

enum class EntityType
{
	None,
	Player,
	NPC,
	Monster,
	Mob
};

enum class MonsterType
{
	None,
	Goblin,
};


static std::wstring EntityTypeToString(EntityType type)
{
	switch (type)
	{
	case EntityType::None: return L"None";
	case EntityType::Player: return L"Player";
	case EntityType::NPC: return L"NPC";
	case EntityType::Monster: return L"Monster";
	case EntityType::Mob: return L"Mob";
	default:                  return L"None";
	}
}

static std::wstring MonsterTypeToString(MonsterType type)
{
	switch (type)
	{
	case MonsterType::Goblin: return L"Goblin";
	default:                  return L"None";
	}
}

static std::wstring StateToString(State state)
{
	switch (state)
	{
	case State::Idle: return L"Idle";
	case State::Walking: return L"Walking";
	case State::Attacking: return L"Attacking";
	case State::Damage: return L"Damage";
	default:                  return L"None";
	}
}

static std::wstring DirectionToString(Direction direction)
{
	switch (direction)
	{
	case Direction::Up: return L"Up";
	case Direction::Down: return L"Down";
	case Direction::Left: return L"Left";
	case Direction::Right: return L"Right";
	default:                  return L"None";
	}
}