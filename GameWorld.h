#pragma once
#include <windows.h>       
#include <objidl.h>        
#include <gdiplus.h>       
#pragma comment (lib,"Gdiplus.lib")
#include "Character.h"
#include "CollisionManager.h"
#include "Camera.h"
#include "TileMapLoader.h"
using namespace Gdiplus;

class GameWorld
{
public:
    GameWorld(int viewW = 800, int viewH = 600);

    void InitAssets(const std::wstring& playerAssetFolder);
    void LoadPlayerData(const std::wstring& playerAssetFolder);

    void Update(float deltaTime);
    void Draw(Gdiplus::Graphics& g, const RECT& clientRect);

    Character* GetPlayer() { return player; }
    Camera& GetCamera() { return camera; }
    CollisionManager& GetCollisionManager() { return collisionMgr; }

    void SetWorldBounds(float left, float top, float right, float bottom);

private:
    Character* player = nullptr;
    CollisionManager collisionMgr;
    Camera camera;
    TileMapLoader m_mapLoader;
};
