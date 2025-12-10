#pragma once
#include <string>
#include <vector>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

class MapManager {
public:
    MapManager(int width = 10, int height = 10, int tileWidth = 32, int tileHeight = 32);

    void setTile(int x, int y, int tileId);
    int getTile(int x, int y) const;

    void saveMap(const std::string& filename);
    void loadMap(const std::string& filename);

private:
    int width, height;
    int tileWidth, tileHeight;
    std::vector<int> layerData;
};
