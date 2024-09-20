#ifndef Map_hpp
#define Map_hpp

#include <unordered_map>
#include <sstream>
#include <fstream>
#include <cmath>

#include "tile/Tile.hpp"
#include "../state/states/BaseState.hpp"

/// @brief Dictionary which holds a pointer to Tile
/// objects that are addressed by an unsigned integer.
using TileMap = std::unordered_map<TileID, Tile*>;
/// @brief Represents the container of all different
/// types of tiles, which are tied to a an unsigned
/// integer.
using TileSet = std::unordered_map<TileID, TileInfo*>;

class Map {
public:
    Map(SharedContext* context, BaseState* currentState);
    ~Map();

    Tile* getTile(unsigned int x, unsigned int y);
    TileInfo* getDefaultTile();
    unsigned int getTileSize() const;

    const sf::Vector2u& getMapSize() const;
    const sf::Vector2f& getPlayerStart() const;
    float getGravity() const;

    void loadMap(const std::string& fileName);
    void loadNext();
    void update(float deltaTime);
    void draw();
private:
    unsigned int convertCoordinates(unsigned int x, unsigned int y);
    void loadTiles(const std::string& fileName);
    void purgeMap();
    void purgeTileSet();

    TileSet m_tileSet;
    TileMap m_tileMap;
    
    sf::Sprite m_background;
    std::string m_backgroundTexture;
    TileInfo m_defaultTile;
    sf::Vector2u m_maxMapSize;
    sf::Vector2f m_playerStart;
    unsigned int m_tileCount;
    unsigned int m_tileSetCount;
    float m_mapGravity;
    std::string m_nextMap;
    bool m_loadNextMap;

    BaseState* m_currentState;
    SharedContext* m_context;
};

#endif