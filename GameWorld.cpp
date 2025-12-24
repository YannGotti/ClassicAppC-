#include "GameWorld.h"
#include "EntityController.h"
#include "InputController.h"
#include "Goblin.h"
#include "Swords.h"
#include "Potions.h"

#pragma comment(lib, "gdiplus.lib")

GameWorld::GameWorld(int viewW, int viewH)
    : camera()
{
    camera.SetViewSize(viewW, viewH);
    camera.SetSmoothing(0.25f);
    camera.SetZoom(3.0f);

    EntityController::LoadEntities();
    EntityController::GetDynamicEntities();
}


void GameWorld::InitAssets(const std::wstring& playerAssetFolder)
{
    
	LoadPlayerData(playerAssetFolder);

    if (!m_mapLoader.LoadProject(L"TiledMap/map.tiled-project"))
    {
        OutputDebugString(L"Ошибка загрузки Tiled проекта!\n");
    }
    else
    {
        m_mapLoader.SetActiveMap(0);

        LayerController::Add(&m_mapLoader, 0); 
    }


    camera.Follow(player);
}

void GameWorld::LoadPlayerData(const std::wstring& playerAssetFolder)
{
    Character* loadedPlayer = EntityController::GetPlayer();

    if (loadedPlayer)
    {
        player = new Character(loadedPlayer->x, loadedPlayer->y);

        player->SetHealth(loadedPlayer->GetHealth());
        player->currentAnimation = loadedPlayer->currentAnimation;

        player->animController.LoadAnimations(playerAssetFolder);
        player->SetAnimation(player->currentAnimation);

        //EntityController::ReplaceEntity(loadedPlayer, player);
    }
    else
    {
        player = new Character(10, 10);
        player->animController.LoadAnimations(playerAssetFolder);
        player->SetAnimation(L"IdleDown");

        //EntityController::AddEntity(player);
    }

    shared_ptr<Item> iron_sword = make_shared<IronSword>();
    shared_ptr<Item> bronze_sword = make_shared<BronzeSword>();
    shared_ptr<Item> health_potion = make_shared<HealthPotion>();

    player->GetInventory().AddItem(iron_sword);
    player->GetInventory().AddItem(bronze_sword);
    player->GetInventory().AddItem(health_potion);
    player->GetInventory().AddItem(bronze_sword);
    player->GetInventory().AddItem(bronze_sword);
    player->GetInventory().AddItem(bronze_sword);
    player->GetInventory().AddItem(bronze_sword);
    player->GetInventory().AddItem(bronze_sword);
    player->GetInventory().AddItem(bronze_sword);


    LayerController::Add(player, 20);
}

void GameWorld::Update(float deltaTime)
{
    if (GetAsyncKeyState('C')) {
        Goblin* boblin = new Goblin(100, 100);
        EntityController::AddEntity(boblin);
        //EntityController::SaveEntities();
        LayerController::Add(boblin, 20);
    }

    InputController::Update(deltaTime);
    LayerController::UpdateAll(deltaTime);
    //collisionMgr.Update();
    camera.Update();
}

void GameWorld::Draw(Gdiplus::Graphics& g, const RECT& clientRect)
{
    camera.SetViewSize(clientRect.right, clientRect.bottom);
    camera.ApplyTransform(g);

    LayerController::DrawAll(g);

    Camera::ResetTransform(g, clientRect.right, clientRect.bottom);

    LayerController::DrawScreen(g, clientRect);

    //player->GetDebugInfo(g);
    //player->DebugAttack(g);
}

void GameWorld::SetWorldBounds(float left, float top, float right, float bottom)
{
    camera.SetWorldBounds(left, top, right, bottom);
}
