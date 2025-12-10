#include "TileMapLoader.h"
#include <iostream>
#include <algorithm>

// --- Статические утилиты ---
std::string TileMapLoader::WStringToUTF8(const std::wstring& wstr)
{
    if (wstr.empty()) return {};
    int size = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), nullptr, 0, nullptr, nullptr);
    if (size <= 0) return {};
    std::string str(size, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), str.data(), size, nullptr, nullptr);
    return str;
}

std::wstring TileMapLoader::UTF8ToWString(const std::string& str)
{
    if (str.empty()) return {};
    int size = MultiByteToWideChar(CP_UTF8, 0, str.data(), (int)str.size(), nullptr, 0);
    if (size <= 0) return {};
    std::wstring wstr(size, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.data(), (int)str.size(), wstr.data(), size);
    return wstr;
}

// --- Конструктор / Деструктор ---
TileMapLoader::TileMapLoader() = default;

TileMapLoader::~TileMapLoader()
{
    for (auto& loadedMap : m_maps)
    {
        for (auto& img : loadedMap.images)
        {
            delete img.bmp;
        }
    }
}

// --- Загрузка проекта ---
bool TileMapLoader::LoadProject(const std::wstring& projectPath)
{
    m_maps.clear();
    m_activeMapIndex = static_cast<size_t>(-1);

    std::string utf8Path = WStringToUTF8(projectPath);
    std::filesystem::path projPath(utf8Path);

    if (!std::filesystem::exists(projPath))
    {
        std::wcerr << L"Project file not found: " << projectPath << L"\n";
        return false;
    }

    if (!m_project.parse(projPath))
    {
        std::wcerr << L"Failed to parse Tiled project.\n";
        return false;
    }

    tson::Tileson parser;

    // Проходим по всем папкам
    for (const auto& folder : m_project.getFolders())
    {
        std::filesystem::path folderPath = folder.getPath();

        // Проходим по всем файлам в папке
        for (const auto& file : folder.getFiles())
        {
            if (file.extension() == ".json")
            {
                std::filesystem::path mapFullPath = folderPath / file.filename();
                auto loaded = LoadSingleMap(mapFullPath);
                if (loaded)
                {
                    m_maps.push_back(std::move(*loaded));
                }
            }
        }
    }

    if (m_maps.empty())
    {
        std::wcerr << L"No maps loaded from project.\n";
        return false;
    }

    m_activeMapIndex = 0;
    return true;
}

// --- Загрузка одной карты ---
std::unique_ptr<TileMapLoader::LoadedMap> TileMapLoader::LoadSingleMap(const std::filesystem::path& mapPath)
{
    tson::Tileson parser;
    auto map = parser.parse(mapPath);

    if (!map || map->getStatus() != tson::ParseStatus::OK)
    {
        std::wcerr << L"Failed to load map: " << mapPath.wstring() << L"\n";
        if (map) std::wcerr << UTF8ToWString(map->getStatusMessage()) << L"\n";
        return nullptr;
    }

    auto loadedMap = std::make_unique<LoadedMap>();
    loadedMap->map = std::move(map);
    loadedMap->fullPath = mapPath.wstring();

    std::filesystem::path mapDir = mapPath.parent_path();

    for (const auto& ts : loadedMap->map->getTilesets())
    {
        // 🔥 КРИТИЧЕСКАЯ ПРОВЕРКА: если тайлсет не загружен — пропускаем
        if (ts.getTileSize().x <= 0 || ts.getTileSize().y <= 0)
        {
            std::wcerr << L"Tileset not properly loaded (invalid tile size). Check .tsx file!\n";
            continue;
        }

        int firstgid = ts.getFirstgid();
        std::filesystem::path imagePath = ts.getImage();

        if (imagePath.empty())
        {
            std::wcerr << L"Tileset (gid=" << firstgid << L") has no image path!\n";
            continue;
        }

        std::filesystem::path fullImagePath = mapDir / imagePath;
        std::wstring wImagePath = fullImagePath.wstring();

        Gdiplus::Bitmap* bmp = new Gdiplus::Bitmap(wImagePath.c_str());
        if (!bmp || bmp->GetLastStatus() != Gdiplus::Ok)
        {
            delete bmp;
            std::wcerr << L"Failed to load tileset image: " << wImagePath << L"\n";
            continue;
        }

        TilesetImage img;
        img.firstgid = firstgid;
        img.imagePath = wImagePath;
        img.bmp = bmp;
        loadedMap->images.push_back(img);
    }

    return loadedMap;
}

Gdiplus::Bitmap* TileMapLoader::GetBitmapByFirstGid(
    const std::vector<TilesetImage>& images, int firstgid)
{
    auto it = std::find_if(images.begin(), images.end(),
        [firstgid](const TilesetImage& img) { return img.firstgid == firstgid; });
    return (it != images.end()) ? it->bmp : nullptr;
}

bool TileMapLoader::SetActiveMap(size_t index)
{
    if (index < m_maps.size())
    {
        m_activeMapIndex = index;
        return true;
    }
    return false;
}

bool TileMapLoader::SetActiveMapByName(const std::string& name)
{
    for (size_t i = 0; i < m_maps.size(); ++i)
    {
        std::filesystem::path p(m_maps[i].fullPath);
        std::string fileName = p.stem().string(); 

        if (fileName == name)
        {
            m_activeMapIndex = i;
            return true;
        }
    }
    return false;
}

// --- Отрисовка ---
void TileMapLoader::Draw(Gdiplus::Graphics& g)
{
    if (m_activeMapIndex >= m_maps.size())
        return;

    auto& active = m_maps[m_activeMapIndex];
    if (!active.map) return;

    for (auto& layer : active.map->getLayers())
    {
        if (layer.getType() != tson::LayerType::TileLayer)
            continue;

        for (auto& [tilePos, tileObj] : layer.getTileObjects())
        {
            tson::Tile* tile = tileObj.getTile();
            if (!tile) continue;

            tson::Tileset* ts = tile->getTileset();
            if (!ts) continue;

            Gdiplus::Bitmap* bmp = GetBitmapByFirstGid(active.images, ts->getFirstgid());
            if (!bmp) continue;

            tson::Rect r = tileObj.getDrawingRect();
            tson::Vector2f pos = tileObj.getPosition();

            int tileW = ts->getTileSize().x;
            int tileH = ts->getTileSize().y;

            g.DrawImage(
                bmp,
                Gdiplus::Rect((int)pos.x, (int)pos.y, tileW, tileH),
                r.x, r.y, r.width, r.height,
                Gdiplus::UnitPixel
            );
        }
    }
}