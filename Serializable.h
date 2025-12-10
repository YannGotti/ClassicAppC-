#pragma once
#include <vector>
#include <fstream>
#include "Entity.h"
#include "Monster.h"
#include "Goblin.h"
#include "Character.h"
#include <iostream>

class Serializable
{
public:
    static void serialize(const std::string& filename, const std::vector<Entity*>& entities)
    {
        std::ofstream f(filename, std::ios::binary | std::ios::trunc); // 👈 обязательно trunc!
        if (!f.is_open()) return;

        size_t count = entities.size();
        std::cout << "[Serialize] count = " << count << std::endl; // debug

        f.write((char*)&count, sizeof(count));

        for (auto* e : entities)
        {
            EntityType type = e->GetEntityType();
            f.write((char*)&type, sizeof(type));
            e->Save(f);
        }

        std::cout << "[Serialize] done, file closed.\n";
    }

    static std::vector<Entity*> deserialize(const std::string& filename)
    {
        std::vector<Entity*> result;
        std::ifstream f(filename, std::ios::binary);
        if (!f.is_open()) return result;

        size_t count = 0;
        f.read((char*)&count, sizeof(count));
        if (!f || count == 0) return result;

        result.reserve(count);

        for (size_t i = 0; i < count; i++)
        {
            EntityType type = EntityType::None;
            f.read((char*)&type, sizeof(type));

            Entity* e = nullptr;

            switch (type)
            {
            case EntityType::Player:
                e = new Character();
                break;
            case EntityType::Monster:
                e = new Monster(); // или просто Monster()
                break;
            default:
                e = new Entity();
                break;
            }

            if (e)
            {
                e->Load(f);
                result.push_back(e);
            }
        }

        return result;
    }
};
