#ifndef Tile_hpp
#define Tile_hpp

#include "TileInfo.hpp"

struct Tile {
    /// @brief Tile properties are common for each type of 
    /// Tile object allocated, therefore we will use a pointer.
    TileInfo* m_properties;
    /// @brief Flag that indicates wether the tile warps to the
    /// next level or not.
    bool m_warp;
};

#endif