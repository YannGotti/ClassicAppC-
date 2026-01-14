#include "Character.h"
#include "EventsTypes.h"
#include "ItemObject.h"

Character::Character(int x, int y) : Entity(x, y)
{
	Init(); // Подписка на событие

	type = EntityType::Player;
	actionToAnimation[L"Attack"] = L"Attack";
	actionToAnimation[L"Throw"] = L"Throw";

	currentAnimation = L"IdleDown";
	animController.Play(currentAnimation);
	damage = 10;
	speed = 16.0f;

}

void Character::Init()
{
	EventBus::Subscribe<PlayerDropItemEvent>(
		[this](const PlayerDropItemEvent& e) {
			this->DropItem();
		}
	);
}

void Character::Update(float deltaTime)
{
#pragma region MovePerson
	dx = 0;
	dy = 0;

	if (GetAsyncKeyState('A') & 0x8000) {
		if (!IsMovementLocked(Direction::Left)) {
			dx = -1;
			SetDirection(Direction::Left);
		}
	}
	else if (GetAsyncKeyState('D') & 0x8000) {
		if (!IsMovementLocked(Direction::Right)) {
			dx = 1;
			SetDirection(Direction::Right);
		}
	}

	if (GetAsyncKeyState('W') & 0x8000) {
		if (!IsMovementLocked(Direction::Up)) {
			dy = -1;
			SetDirection(Direction::Up);
		}
	}
	else if (GetAsyncKeyState('S') & 0x8000) {
		if (!IsMovementLocked(Direction::Down)) {
			dy = 1;
			SetDirection(Direction::Down);
		}
	}

	// --- Скорость движения по осям ---
	velocityX = dx * speed;
	velocityY = dy * speed;

	// --- Статус ---
	if (dx != 0 || dy != 0)
		SetState(State::Walking);
	else
		SetState(State::Idle);

#pragma endregion

	if (GetAsyncKeyState(VK_LBUTTON)) {
		LogicAttack(this);
	}

	UpdateCulldown();

	SetAnimation(GetAnimationName());
	Entity::Update(deltaTime);
}

void Character::Draw(Gdiplus::Graphics& g)
{
	Entity::Draw(g);
}

void Character::PerformAction(const wstring& actionName)
{
	if (actionToAnimation.find(actionName) != actionToAnimation.end())
	{
		SetState(State::Attacking);
		SetAnimation(actionToAnimation[actionName]);
	}
}

void Character::DropItem()
{
	inventory.DropItem();
	Objects::Item::CreateItem(inventory.GetCurrentSlot(), GetX(), GetY());
}




