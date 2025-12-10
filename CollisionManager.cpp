#include "CollisionManager.h"
#include <objidl.h>
#include <gdiplus.h>
#include <iostream>

#include "EntityController.h"
using namespace Gdiplus;



void CollisionManager::Update()
{
    for (auto* entity : EntityController::GetDynamicEntities())
    {
        if (!entity)
            continue;


        int nextX = entity->x + entity->dx;
        int nextY = entity->y + entity->dy;

        FloatRect rectX = entity->GetBoundsF();
        rectX.left += entity->dx;
        rectX.right += entity->dx;

        bool blockedX = false;
        for (auto* obj : EntityController::GetStaticEntities())
        {
            if (entity == obj) continue;
            if (entity->IntersectsAt(*obj, rectX))
            {
                blockedX = true;
                if (entity->dx > 0 && entity->dy == 0)
                {
                    entity->x = obj->GetBoundsF().left - entity->width;
                    entity->collideRight = true;
                    entity->LockMovement(Direction::Right);
                }
                else if (entity->dx < 0 && entity->dy == 0)
                {
                    entity->x = obj->GetBoundsF().right;
                    entity->collideLeft = true;
                    entity->LockMovement(Direction::Left);
                }
                break;
            }
        }
        if (!blockedX)
        {
            entity->x = nextX;
            if (entity->IsMovementLocked(Direction::Left) || entity->IsMovementLocked(Direction::Right))
                entity->UnlockMovement();
        }

        FloatRect rectY = entity->GetBoundsF();
        rectY.top += entity->dy;
        rectY.bottom += entity->dy;

        bool blockedY = false;
        for (auto* obj : EntityController::GetStaticEntities())
        {
            if (entity == obj) continue;
            if (entity->IntersectsAt(*obj, rectY))
            {
                blockedY = true;
                if (entity->dy > 0 && entity->dx == 0)
                {
                    entity->y = obj->GetBoundsF().top - entity->height;
                    entity->collideDown = true;
                    entity->LockMovement(Direction::Down);
                }
                else if (entity->dy < 0 && entity->dx == 0)
                {
                    entity->y = obj->GetBoundsF().bottom;
                    entity->collideUp = true;
                    entity->LockMovement(Direction::Up);
                }
                break;
            }
        }
        if (!blockedY)
        {
            entity->y = nextY;
            if (entity->IsMovementLocked(Direction::Up) || entity->IsMovementLocked(Direction::Down))
                entity->UnlockMovement();
        }

        for (auto* obj : EntityController::GetStaticEntities())
        {
            if (entity->Intersects(*obj))
            {
                FloatRect r1 = entity->GetBoundsF();
                FloatRect r2 = obj->GetBoundsF();
                int overlapX = min(r1.right, r2.right) - max(r1.left, r2.left);
                int overlapY = min(r1.bottom, r2.bottom) - max(r1.top, r2.top);

                if (overlapX < overlapY)
                    entity->x += (r1.left < r2.left ? -overlapX : overlapX);
                else
                    entity->y += (r1.top < r2.top ? -overlapY : overlapY);
            }
        }
    }
}

void CollisionManager::Draw(Gdiplus::Graphics& g)
{
	Pen myPen(Color(255, 0, 0, 255), 2);

	for (auto* other : EntityController::GetDynamicEntities())
	{
		g.DrawRectangle(
			&myPen,
			other->GetX(),
			other->GetY(),
			other->width,
			other->height
		);
	}

	for (auto* obj : EntityController::GetStaticEntities())
	{
		g.DrawRectangle(
			&myPen,
			obj->GetX(),
			obj->GetY(),
			obj->width,
			obj->height
		);
	}
}

Entity* CollisionManager::CollideEntity(Entity* owner)
{
    for (auto* obj : EntityController::GetDynamicEntities())
    {
        if (obj != owner && owner->Intersects(*obj, 2)) {
            return obj;
        }
    }
    return nullptr;
}
