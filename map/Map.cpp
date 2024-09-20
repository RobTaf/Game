#include "Map.hpp"
#include "../state/StateManager.hpp"

Map::Map(SharedContext* context, BaseState* currentState):
    m_context(context),
    m_currentState(currentState),
    m_defaultTile(context, "", 0),
    m_maxMapSize(32, 32),
    m_tileCount(0),
    m_tileSetCount(0),
    m_mapGravity(512.f),
    m_loadNextMap(false) {
        m_context->m_gameMap = this;
        loadTiles("tiles.cfg");
}

Map::~Map() {
    purgeMap();
    purgeTileSet();
    m_context->m_gameMap = nullptr;
}

Tile* Map::getTile(unsigned int x, unsigned int y) {
    auto itr = m_tileMap.find(convertCoordinates(x, y));
    return itr != m_tileMap.end() ? itr->second : nullptr;
}

TileInfo* Map::getDefaultTile() {
    return &m_defaultTile;
}

unsigned int Map::getTileSize() const {
    return Sheet::TileSize;
}

const sf::Vector2u& Map::getMapSize() const {
    return m_maxMapSize;
}

const sf::Vector2f& Map::getPlayerStart() const {
    return m_playerStart;
}

float Map::getGravity() const {
    return m_mapGravity;
}

void Map::update(float deltaTime) {
    if (m_loadNextMap) {
        purgeMap();
        m_loadNextMap = false;
        if (m_nextMap != "") {
            loadMap(m_nextMap);
        } else {
            m_currentState->getStateManager()->switchTo(StateType::GameOver);
        }
        m_nextMap = "";
    }
    sf::FloatRect viewSpace = m_context->m_window->viewSpace();
    m_background.setPosition(viewSpace.left, viewSpace.top);
}

void Map::draw() {
    sf::RenderWindow* renderWindow = m_context->m_window->getRenderWindow();
    renderWindow->draw(m_background);
    sf::FloatRect viewSpace = m_context->m_window->viewSpace();

    sf::Vector2i tileBegin(
        floor(viewSpace.left / Sheet::TileSize), 
        floor(viewSpace.top / Sheet::TileSize));
    sf::Vector2i tileEnd(
        ceil((viewSpace.left + viewSpace.width) / Sheet::TileSize),
        ceil((viewSpace.top + viewSpace.height) / Sheet::TileSize));

    unsigned int count = 0;
    for (int x = tileBegin.x; x <= tileEnd.x; x++) {
        for (int y = tileBegin.y; y <= tileEnd.y; y++) {
            if (x < 0 or y < 0)
                continue;
            Tile* tile = getTile(x, y);
            if (not tile) {
                continue;
            }
            sf::Sprite& sprite = tile->m_properties->m_sprite;
            sprite.setPosition(x * Sheet::TileSize, y * Sheet::TileSize);
            renderWindow->draw(sprite);
            ++count;
        }
    }
}

unsigned int Map::convertCoordinates(unsigned int x, unsigned int y) {
    return x * m_maxMapSize.x + y;
}

void Map::loadTiles(const std::string& fileName) {
    std::ifstream file;
    file.open(Utility::workingDirectory() + "assets/config/" + fileName);
    if (not file.is_open()) {
        std::cout << "! Failed loading tile set file: " << fileName << std::endl;
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        if (line[0] == '|') continue;
        std::stringstream keystream(line);
        int tileId;
        keystream >> tileId;
        if (tileId < 0) continue;
        TileInfo* tileInfo = new TileInfo(m_context, "TileSet", tileId);
        keystream >> tileInfo->m_name >> tileInfo->m_friction.x >> tileInfo->m_friction.y >> tileInfo->m_deadly;
        if (not m_tileSet.emplace(tileId, tileInfo).second) {
            std::cout << "! Duplicate tile type: " << tileInfo->m_name << std::endl;
            delete tileInfo;
        }
    }
    file.close();
}

void Map::loadMap(const std::string &fileName) {
    std::ifstream file;
    file.open(Utility::workingDirectory() + "assets/maps/" + fileName + ".map");
    if (not file.is_open()) {
        std::cout << "! Failed loading map file: " << fileName << std::endl;
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        if (line[0] == '|') continue;
        std::stringstream keystream(line);
        std::string type;
        keystream >> type;
        if (type == "TILE") {
            int tileId = 0;
            keystream >> tileId;
            if (tileId < 0) {
                std::cout << "! Bad tile id: " << tileId << std::endl;
                continue;
            }
            auto itr = m_tileSet.find(tileId);
            if (itr == m_tileSet.end()) {
                std::cout << "! Tile id(" << tileId << ") was not found in tileset." << std::endl;
                continue;
            }
            sf::Vector2i tileCoordinates;
            keystream >> tileCoordinates.x >> tileCoordinates.y;
            if (tileCoordinates.x > m_maxMapSize.x or tileCoordinates.y > m_maxMapSize.y) {
                std::cout << "! Tile is out of range: " << tileCoordinates.x << " " << tileCoordinates.y << std::endl;
                continue;
            }
            Tile* tile = new Tile();
            tile->m_properties = itr->second;
            if (not m_tileMap.emplace(convertCoordinates(tileCoordinates.x, tileCoordinates.y), tile).second) {
                std::cout << "! Duplicate tile detected: " << tileCoordinates.x << " " << tileCoordinates.y << std::endl;
                delete tile;
                tile = nullptr;
                continue;
            }
            std::string warp;
            keystream >> warp;
            tile->m_warp = false;
            if (warp == "WARP") {
                tile->m_warp = true;
            }
        } else if (type == "BACKGROUND") {
            if (m_backgroundTexture != "") continue;
            keystream >> m_backgroundTexture;
            if (not m_context->m_textureManager->requireResource(m_backgroundTexture)) {
                m_backgroundTexture = "";
                continue;
            }
            sf::Texture* texture = m_context->m_textureManager->getResource(m_backgroundTexture);
            m_background.setTexture(*texture);

            sf::Vector2f viewSize = m_currentState->getView().getSize();
            sf::Vector2u textureSize = texture->getSize();
            sf::Vector2f scaleFactors;
            scaleFactors.x = viewSize.x / textureSize.x;
            scaleFactors.y = viewSize.y / textureSize.y;
            m_background.setScale(scaleFactors);
        } else if (type == "SIZE") {
            keystream >> m_maxMapSize.x >> m_maxMapSize.y;
        } else if (type == "GRAVITY") {
            keystream >> m_mapGravity;
        } else if (type == "DEFAULT_FRICTION") {
            keystream >> m_defaultTile.m_friction.x >> m_defaultTile.m_friction.y;
        } else if (type == "NEXT_MAP") {
            keystream >> m_nextMap;
        } else if (type == "PLAYER") {
            // unsigned int playerId;
            // float playerX = 0;
            // float playerY = 0;
            // keystream >> playerId >> playerX >> playerY;
            // if (m_context->m_entityManager->find(playerId)) { continue; }
            // if (playerId < 0) { continue; }
            // m_context->m_entityManager->find(playerId)->setPosition(playerX, playerY);
            // m_playerStart = sf::Vector2f(playerX, playerY);
        } else if (type == "ENEMY") {
            // std::string enemyName;
            // keystream >> enemyName;
            // int enemyId = m_context->m_entityManager->add(EntityType::Enemy, enemyName);
            // if (enemyId < 0) { continue; }
            // float enemyX = 0;
            // float enemyY = 0;
            // keystream >> enemyX >> enemyY;
            // m_context->m_entityManager->find(enemyId)->setPosition(enemyX, enemyY);
        }
    }
    file.close();
}

void Map::loadNext() {
    m_loadNextMap = true;
}

void Map::purgeMap() {
    m_tileCount = 0;
    for (auto& itr : m_tileMap) {
        delete itr.second;
    }
    m_tileMap.clear();
    m_context->m_entityManager->purge();
    if (m_backgroundTexture == "")
        return;
    m_context->m_textureManager->releaseResource(m_backgroundTexture);
    m_backgroundTexture = "";
}

void Map::purgeTileSet() {
    for (auto& itr : m_tileSet) {
        delete itr.second;
    }
    m_tileSet.clear();
    m_tileSetCount = 0;
}
