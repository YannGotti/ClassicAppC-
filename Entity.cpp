#include "Entity.h"
#include <iostream>
#include <fstream>
#include <string>
#include "ItemObject.h"

Entity::Entity(float x_, float y_, float w_, float h_)
	: x(x_), y(y_), width(w_), height(h_)
{
}

void Entity::StatementTimerUpdate(float deltaTime)
{
	if (stateLocked)
	{
		stateLockTimer -= deltaTime;
		if (stateLockTimer <= 0.0f)
		{
			stateLocked = false;
			stateLockTimer = 0.0f;
		}
	}
}

void Entity::Update(float deltaTime)
{
	if (!isAlive) return;

	x += velocityX * deltaTime;
	y += velocityY * deltaTime;


	StatementTimerUpdate(deltaTime);

	animController.Update();
}

void Entity::Draw(Gdiplus::Graphics& g) 
{
	if (!isAlive) return;

	Gdiplus::Image* frame = animController.GetCurrentFrame();
	if (frame)
		g.DrawImage(
			frame,
			x, y,
			frame->GetWidth(),
			frame->GetHeight()
		);
}

void Entity::Move(float dx, float dy)
{
	this->dx = dx;
	this->dy = dy;
}

void Entity::SetAnimation(const wstring& animName)
{
	currentAnimation = animName;
	animController.Play(animName);

	auto frame = animController.GetCurrentFrame();
	if (frame) {
		width = frame->GetWidth();
		height = frame->GetHeight();
	}
}

bool Entity::Intersects(const Entity& other) const
{
	FloatRect a = GetBoundsF();
	FloatRect b = other.GetBoundsF();
	return a.left < b.right && a.right > b.left &&
		a.top < b.bottom && a.bottom > b.top;
}

bool Entity::Intersects(const Entity& other, float offset) const
{
	FloatRect a = GetBoundsF();
	FloatRect b = other.GetBoundsF();

	LONG a_left = a.left - offset;
	LONG a_right = a.right + offset;
	LONG a_top = a.top - offset;
	LONG a_bottom = a.bottom + offset;

	return a_left < b.right && a_right > b.left &&
		a_top < b.bottom && a_bottom > b.top;
}

bool Entity::IntersectsAt(const Entity& other, const FloatRect& testBounds) const
{
	FloatRect b = other.GetBoundsF();
	return testBounds.left < b.right && testBounds.right > b.left &&
		testBounds.top < b.bottom && testBounds.bottom > b.top;
}

void Entity::SetHealth(int value)
{
	health = value;
}

void Entity::ModifyHealth(bool isHealing, int value)
{
	if (isHealing) {
		health += value;

		if (health > 100) health = 100;
	}

	if (!isHealing) health -= value;


	if (health <= 0) {
		Die();
	}
}


void Entity::Save(ofstream& f) const
{
	// Примитивные типы — сохраняем напрямую
	f.write((char*)&x, sizeof(x));
	f.write((char*)&y, sizeof(y));
	f.write((char*)&width, sizeof(width));
	f.write((char*)&height, sizeof(height));

	f.write((char*)&health, sizeof(health));
	f.write((char*)&damage, sizeof(damage));

	f.write((char*)&isAlive, sizeof(isAlive));
	f.write((char*)&markedForDeletion, sizeof(markedForDeletion));

	f.write((char*)&type, sizeof(type));

	size_t animSize = currentAnimation.size();
	f.write((char*)&animSize, sizeof(animSize));
	f.write((char*)currentAnimation.data(), animSize * sizeof(wchar_t));
}

void Entity::Load(ifstream& f)
{
	f.read((char*)&x, sizeof(x));
	f.read((char*)&y, sizeof(y));
	f.read((char*)&width, sizeof(width));
	f.read((char*)&height, sizeof(height));

	f.read((char*)&health, sizeof(health));
	f.read((char*)&damage, sizeof(damage));

	f.read((char*)&isAlive, sizeof(isAlive));
	f.read((char*)&markedForDeletion, sizeof(markedForDeletion));

	f.read((char*)&type, sizeof(type));

	// wstring currentAnimation
	size_t animSize;
	f.read((char*)&animSize, sizeof(animSize));
	currentAnimation.resize(animSize);
	f.read((char*)currentAnimation.data(), animSize * sizeof(wchar_t));
}


void Entity::SetDirection(Direction dir)
{
	direction = dir;
}

void Entity::SetState(State st)
{
	if (stateLocked) return;
	state = st;
}

void Entity::ForceState(State newState, float lockDuration)
{
	state = newState;
	stateLocked = true;
	stateLockTimer = lockDuration;
}

void Entity::UnlockState()
{
	stateLocked = false;
}

wstring Entity::GetAnimationName() const
{
	switch (state)
	{
	case State::Idle:
		switch (direction)
		{
		case Direction::Up:
			return L"IdleUp";
		case Direction::Down:
			return L"IdleDown";
		case Direction::Left:
			return L"IdleLeft";
		case Direction::Right:
			return L"IdleRight";
		}
		break;

	case State::Walking:
		switch (direction)
		{
		case Direction::Up:
			return L"WalkingUp";
		case Direction::Down:
			return L"WalkingDown";
		case Direction::Left:
			return L"WalkingLeft";
		case Direction::Right:
			return L"WalkingRight";
		}
		break;
	case State::Attacking:
		switch (direction)
		{
		case Direction::Up:
			return L"FightUp";
		case Direction::Down:
			return L"FightDown";
		case Direction::Left:
			return L"FightLeft";
		case Direction::Right:
			return L"FightRight";
		}
		break;
	case State::Damage:
		switch (direction)
		{
			case Direction::Right:
				return L"DamageRight";
			case Direction::Left:
				return L"DamageLeft";
			case Direction::Up:
				return L"DamageUp";
			case Direction::Down:
				return L"DamageDown";
		}
	default:
		return L"IdleDown";
	}

	return L"IdleDown";
}


void Entity::OnCollision(Entity* entity)
{
	dx = dy = 0;
}

PointF Entity::GetNextPosition() const
{
	return { x + dx, y + dy };
}

void Entity::ApplyMovement()
{
	x += dx;
	y += dy;
}

void Entity::RevertMovement()
{
	dx = 0;
	dy = 0;
}

void Entity::TryPickup()
{
	for (IRenderable* r : LayerController::GetLayerObjects(19))
	{
		auto item = dynamic_cast<Objects::Item*>(r);
		if (!item) continue;

		RECT a = GetBounds();
		RECT b = item->GetBounds();

		RECT intersection;
		if (IntersectRect(&intersection, &a, &b))
		{
			if (inventory.AddItem(item->GetSlotData().item))
			{
				item->Destroy();
				break;
			}
		}
	}
}


void Entity::GetDebugInfo(Graphics& g)
{
	FontFamily fontFamily(L"Arial");
	Font font(&fontFamily, 14, FontStyleRegular, UnitPixel);
	SolidBrush brush(Color(255, 255, 0, 0));

	wstring debugText =
		L"X: " + to_wstring(x) + L"Y: " + to_wstring(y) +
		L" | Anim: " + currentAnimation +
		L"\n | BoundX: " + to_wstring(x + width) +
		L" | DX: " + to_wstring(dx) +
		L" | DY: " + to_wstring(dy) +
		L"\n | HEALTH: " + to_wstring(health) +
		L"\n | STATE: " + StateToString(state) +
		L"\n | Direction: " + DirectionToString(direction) +
		L"\n | isAlive?: " + to_wstring(isAlive);

	g.DrawString(
		debugText.c_str(),
		-1,
		&font,
		PointF((float)(300), 10),
		&brush
	);
}