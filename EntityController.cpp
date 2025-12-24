#include "EntityController.h"
#include "Serializable.h"
#include "EnumsEntity.h"

void EntityController::AddEntity(Entity* entity)
{
	dynamicEntities.push_back(entity);
}

void EntityController::AddStatic(Entity* obj)
{
	staticEntities.push_back(obj);
}

Character* EntityController::GetPlayer()
{
	for (Entity* e : dynamicEntities)
	{
		if (e->GetEntityType() == EntityType::Player)
			return static_cast<Character*>(e);
	}
	return nullptr;
}

void EntityController::ReplaceEntity(Entity* oldEntity, Entity* newEntity)
{
	if (!oldEntity || !newEntity) return;

	for (size_t i = 0; i < dynamicEntities.size(); i++)
	{
		if (dynamicEntities[i] == oldEntity)
		{
			dynamicEntities[i] = newEntity;
			break;
		}
	}

	delete oldEntity;
}



void EntityController::LoadAnimationsForEntities() {
	for (Entity* e : EntityController::GetDynamicEntities())
	{
		if (e->GetEntityType() == EntityType::Player)
		{
			e->animController.LoadAnimations(L"Assets/Entities/Player");
			e->SetAnimation(L"IdleDown");
		}

		if (e->GetEntityType() == EntityType::Monster)
		{
			Monster* cast = static_cast<Monster*>(e);
			e->animController.LoadAnimations(L"Assets/Entities/" + MonsterTypeToString(cast->GetMonsterType()));
			e->SetAnimation(L"IdleDown");
		}
	}


	for (Entity* e : EntityController::GetStaticEntities())
	{
		/*if (e->GetEntityType() == Player)
		{
			e->animController.LoadAnimations(L"Assets/Player");
			e->SetAnimation(L"IdleDown");
		}*/
	}
}


void EntityController::LoadEntities()
{
	staticEntities = Serializable::deserialize(static_filepath);
	dynamicEntities = Serializable::deserialize(dynamic_filepath);

	LoadAnimationsForEntities();
}

void EntityController::SaveEntities()
{
	Serializable::serialize(static_filepath, staticEntities);
	Serializable::serialize(dynamic_filepath, dynamicEntities);
}
