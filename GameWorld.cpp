#include "GameWorld.h"
#include "EntityController.h"
#include "Goblin.h"
#include "Goblin.h"
#include "Swords.h"

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

        EntityController::ReplaceEntity(loadedPlayer, player);
    }
    else
    {
        player = new Character(10, 10);
        player->animController.LoadAnimations(playerAssetFolder);
        player->SetAnimation(L"IdleDown");

        EntityController::AddEntity(player);
    }

    shared_ptr<Item> iron_sword = make_shared<IronSword>();
    shared_ptr<Item> bronze_sword = make_shared<BronzeSword>();

    player->GetInventory().AddItem(iron_sword);
    player->GetInventory().AddItem(bronze_sword);


    LayerController::Add(player, 20);
}

void GameWorld::Update(float deltaTime)
{
    if (GetAsyncKeyState('C')) {
        Goblin* boblin = new Goblin(100, 100);
        EntityController::AddEntity(boblin);
        EntityController::SaveEntities();
        LayerController::Add(boblin, 20);
    }
    
    //collisionMgr.Update();

    for (auto* e : EntityController::GetDynamicEntities())
    {
        e->Update(deltaTime);
    }

    camera.Update();
}

void GameWorld::Draw(Gdiplus::Graphics& g, const RECT& clientRect)
{
    camera.SetViewSize(clientRect.right, clientRect.bottom);
    camera.ApplyTransform(g);

    /*int locX = 0;
    int locY = 0;
    Pen pen(Color(255, 150, 150, 150), 2);

    int sizeX = 100;
    int sizeY = 100;

    for (size_t i = 0; i < sizeX; i++)
    {
        for (size_t i = 0; i < sizeY; i++)
        {
            Rect rect(locX, locY, 16, 16);
            g.DrawRectangle(&pen, rect);
            locY += 16;

        }
        locX += 16;
        locY = 0;
    }*/

    LayerController::DrawAll(g);

    //m_mapLoader.Draw(&g);

    /*for (auto* e : EntityController::GetDynamicEntities())
    {
        if (e->GetEntityType() != EntityType::Player)
            e->Draw(g);
    }

    for (auto* e : EntityController::GetDynamicEntities())
    {
        if (e->GetEntityType() == EntityType::Player)
            e->Draw(g);
    }*/
    //collisionMgr.Draw(g);

    Camera::ResetTransform(g, clientRect.right, clientRect.bottom);

    /*for (auto* e : EntityController::GetDynamicEntities())
    {
        if (e->GetEntityType() == EntityType::Monster) {
            auto* monster = static_cast<Monster*>(e);
            monster->DebugAttack(g);
            monster->GetDebugInfo(g);
        }
    }*/
    
    //player->GetDebugInfo(g);
    //player->DebugAttack(g);
}

void GameWorld::SetWorldBounds(float left, float top, float right, float bottom)
{
    camera.SetWorldBounds(left, top, right, bottom);
}
