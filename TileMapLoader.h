#pragma once

#include <vector>
#include <memory>
#include <filesystem>
#include <string>

#include <windows.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

#include "tileson.hpp"
#include "IRenderable.h"

class TileMapLoader : public IRenderable
{
public:
    TileMapLoader();
    ~TileMapLoader();

    // Загружает .tiled-project и все карты внутри
    bool LoadProject(const std::wstring& projectPath);

    // Устанавливает активную карту по индексу или имени
    bool SetActiveMap(size_t index);
    bool SetActiveMapByName(const std::string& name);

    // Отрисовка активной карты
    void Draw(Gdiplus::Graphics& g) override;
    void Update(float deltaTime) override { /**/ };

    bool IsDynamic() const { return false; }

private:
    struct TilesetImage
    {
        int firstgid = 0;
        std::wstring imagePath;
        Gdiplus::Bitmap* bmp = nullptr;
    };

    struct LoadedMap
    {
        std::unique_ptr<tson::Map> map;
        std::vector<TilesetImage> images;
        std::wstring fullPath;
    };

    tson::Project m_project;
    std::vector<LoadedMap> m_maps;
    size_t m_activeMapIndex = static_cast<size_t>(-1);

    // Загружает одну карту и её тайлсеты
    std::unique_ptr<LoadedMap> LoadSingleMap(const std::filesystem::path& mapPath);

    // Находит изображение по firstgid в конкретной карте
    Gdiplus::Bitmap* GetBitmapByFirstGid(const std::vector<TilesetImage>& images, int firstgid);
};