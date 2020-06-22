/**************************************************************************
 *   This file is part of HEXTONTILER.                                    *
 *                                                                        *
 *   Author: Ivo Filot <ivo@ivofilot.nl>                                  *
 *                                                                        *
 *   HEXTONTILER is free software:                                        *
 *   you can redistribute it and/or modify it under the terms of the      *
 *   GNU General Public License as published by the Free Software         *
 *   Foundation, either version 3 of the License, or (at your option)     *
 *   any later version.                                                   *
 *                                                                        *
 *   HEXTONTILER is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty          *
 *   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.              *
 *   See the GNU General Public License for more details.                 *
 *                                                                        *
 *   You should have received a copy of the GNU General Public License    *
 *   along with this program.  If not, see http://www.gnu.org/licenses/.  *
 *                                                                        *
 **************************************************************************/

#pragma once

#include <QString>

#include <vector>
#include <map>

#include "tile.h"

class Map {
private:
    std::map<std::pair<int, int>, Tile, std::greater<std::pair<int, int>>> tiles;

public:
    /**
     * @brief      Constructs a new instance.
     */
    Map();

    /**
     * @brief      Gets the tiles.
     *
     * @return     The tiles.
     */
    inline const auto& get_tiles() const {
        return this->tiles;
    }

    /**
     * @brief      Adds a tile.
     *
     * @param[in]  tile_id  The tile identifier
     * @param[in]  x        x coordinate
     * @param[in]  y        y coordinate
     * @param[in]  z        z coordinate
     */
    void add_tile(unsigned int tile_id, int x, int y, int z);

    /**
     * @brief      Remove a tile
     *
     * @param[in]  x        x coordinate
     * @param[in]  y        y coordinate
     */
    void remove_tile(int x, int y);

    /**
     * @brief      Substitute a tile for a tile of a different type
     *
     * @param[in]  tile_id  The tile identifier
     * @param[in]  x        x coordinate
     * @param[in]  y        y coordinate
     */
    void substitute_tile(unsigned int tile_id, int x, int y);

    /**
     * @brief      Get tile id on coordinate, returns -1 on empty
     *
     * @param[in]  x     x coordinate
     * @param[in]  y     y coordinate
     *
     * @return     The tile identifier.
     */
    int get_tile_id(int x, int y);

private:
};
