#pragma once
#include "AnimationController.h"
#include <string>
#include <windows.h>       
#include <objidl.h>       
#include <gdiplus.h>       
#pragma comment (lib,"Gdiplus.lib")

#include "EnumsEntity.h"
#include "LayerController.h"
#include "Inventory.h"

using namespace Gdiplus;
using namespace std;


struct FloatRect
{
	float left, top, right, bottom;
};

class Entity : public IRenderable
{
public:
	Entity(float x = 0.f, float y = 0.f, float w = 32.f, float h = 32.f);
	virtual ~Entity() = default;

	virtual void Update(float deltaTime);
	virtual void StatementTimerUpdate(float deltaTime);
	virtual void Draw(Gdiplus::Graphics& g) override;

	void Move(float dx, float dy);
	void SetAnimation(const wstring& animName);

	int GetX() const { return x; }
	int GetY() const { return y; }

	FloatRect GetBoundsF() const {
		return FloatRect{ x, y, x + width, y + height };
	}

	PointF GetCenterF() const {
		return PointF(x + width * 0.5f, y + height * 0.5f);
	}

	bool Intersects(const Entity& other) const;
	bool Intersects(const Entity& other, float offset) const;

	bool IntersectsAt(const Entity& other, const FloatRect& r) const;

	AnimaionController animController;
	wstring currentAnimation;

	void ModifyHealth(bool isHealing, int value);
	void SetHealth(int value);
	void GetDebugInfo(Graphics& g);
	void MarkForDeletion() { markedForDeletion = true; }
	void SetDirection(Direction dir);

	void SetState(State state);
	void ForceState(State newState, float lockDuration);
	void UnlockState();

	int GetDamage() const { return damage; }
	void SetDamage(int _damage) { damage = _damage; };
	int GetHealth() const { return health; }
	Direction GetDirection() const { return direction; }
	EntityType GetEntityType() const { return type; }
	State GetState() const { return state; }
	wstring GetAnimationName() const;
	

	void Die() {
		isAlive = false;
	}

	void virtual Save(ofstream& f) const;
	void virtual Load(ifstream& f);

	void OnCollision(Entity* entity);
	PointF GetNextPosition() const;
	void ApplyMovement();
	void RevertMovement();

	bool IsMovementLocked(Direction dir) const {
		return movementLocked && dir == blockedDir;
	}

	void LockMovement(Direction dir) {
		movementLocked = true;
		blockedDir = dir;
	}

	void UnlockMovement() {
		movementLocked = false;
	}

	bool IsDynamic() const { return true; }

	Inventory& GetInventory() { return inventory; }

	bool collideLeft = false;
	bool collideRight = false;
	bool collideUp = false;
	bool collideDown = false;

	float x, y;
	float dx, dy;
	int width, height;
	bool isAlive = true;
	float speed = 0.f;
	float velocityX = 0.f;
	float velocityY = 0.f;

	bool stateLocked = false;
	float stateLockTimer = 0.0f;

	bool wantAttack = false;

protected:
	State state = State::Idle;
	Direction direction = Direction::Down;
	int health = 100;
	int damage = 5;
	bool markedForDeletion = false;
	EntityType type = EntityType::None;
	bool movementLocked = false;
	Direction blockedDir = Direction::Down;
	Inventory inventory;
};
