#pragma once
#include <vector>
#include "Entity.h"
#include "Character.h"

static class EntityController
{
public:
	static void AddEntity(Entity* entity);
	static void AddStatic(Entity* obj);

	static void LoadEntities();
	static void SaveEntities();

	static Character* GetPlayer();

	static vector<Entity*> GetDynamicEntities() { return dynamicEntities; };
	static vector<Entity*> GetStaticEntities() { return staticEntities; };

	static void ReplaceEntity(Entity* oldEntity, Entity* newEntity);

private:
	static void LoadAnimationsForEntities();
	inline static vector<Entity*> dynamicEntities;
	inline static vector<Entity*> staticEntities;

	inline static string static_filepath = "Data/SEntities.bin";
	inline static string dynamic_filepath = "Data/DEntities.bin";
};

