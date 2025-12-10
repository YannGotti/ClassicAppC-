#include "MapManager.h"
#include <fstream>
#include <iostream>

MapManager::MapManager(int w, int h, int tW, int tH)
    : width(w), height(h), tileWidth(tW), tileHeight(tH)
{
    layerData.resize(width * height, 0); // Изначально все тайлы 0
}

void MapManager::setTile(int x, int y, int tileId) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        layerData[y * width + x] = tileId;
    }
}

int MapManager::getTile(int x, int y) const {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        return layerData[y * width + x];
    }
    return -1;
}

void MapManager::saveMap(const std::string& filename) {
    json mapJson;

    mapJson["compressionlevel"] = -1;
    mapJson["height"] = height;
    mapJson["width"] = width;
    mapJson["infinite"] = false;

    json layer;
    layer["data"] = layerData;
    layer["height"] = height;
    layer["width"] = width;
    layer["id"] = 1;
    layer["name"] = "Tile Layer 1";
    layer["opacity"] = 1.0;
    layer["type"] = "tilelayer";
    layer["visible"] = true;
    layer["x"] = 0;
    layer["y"] = 0;

    mapJson["layers"] = { layer };
    mapJson["nextlayerid"] = 2;
    mapJson["nextobjectid"] = 1;
    mapJson["orientation"] = "orthogonal";
    mapJson["renderorder"] = "right-down";
    mapJson["tiledversion"] = "1.10.2";
    mapJson["tilewidth"] = tileWidth;
    mapJson["tileheight"] = tileHeight;
    mapJson["type"] = "map";
    mapJson["version"] = "1.10";
    mapJson["tilesets"] = { { {"firstgid", 1}, {"source", "tiles.tsx"} } };

    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cerr << "Ошибка открытия файла для записи: " << filename << "\n";
        return;
    }
    outFile << mapJson.dump(4);
}

void MapManager::loadMap(const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile.is_open()) {
        std::cerr << "Ошибка открытия файла для чтения: " << filename << "\n";
        return;
    }
    json mapJson;
    inFile >> mapJson;

    width = mapJson["width"];
    height = mapJson["height"];
    tileWidth = mapJson["tilewidth"];
    tileHeight = mapJson["tileheight"];

    layerData = mapJson["layers"][0]["data"].get<std::vector<int>>();
}
