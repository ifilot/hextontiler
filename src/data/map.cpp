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

#include "map.h"

/**
 * @brief      Constructs a new instance.
 */
Map::Map() {

}

/**
 * @brief      Adds a tile.
 *
 * @param[in]  tile_id  The tile identifier
 * @param[in]  x        x coordinate
 * @param[in]  y        y coordinate
 * @param[in]  z        z coordinate
 */
void Map::add_tile(unsigned int tile_id, int x, int y, int z) {
	auto pair = std::pair<int, int>(x,y);
	auto got = this->tiles.find(pair);
	if(got == this->tiles.end()) {
		this->tiles.emplace(pair, Tile(tile_id, x, y, z));
	}
}

/**
 * @brief      Remove a tile
 *
 * @param[in]  x        x coordinate
 * @param[in]  y        y coordinate
 */
void Map::remove_tile(int x, int y) {
    auto pair = std::pair<int, int>(x,y);
    auto got = this->tiles.find(pair);
    if(got != this->tiles.end()) {
        this->tiles.erase(pair);
    }
}

/**
 * @brief      Substitute a tile for a tile of a different type
 *
 * @param[in]  tile_id  The tile identifier
 * @param[in]  x        x coordinate
 * @param[in]  y        y coordinate
 */
void Map::substitute_tile(unsigned int tile_id, int x, int y) {
	auto pair = std::pair<int, int>(x,y);
	auto got = this->tiles.find(pair);
	if(got != this->tiles.end()) {
		got->second.tile_id = tile_id;
	}
}

/**
 * @brief      Get tile id on coordinate, returns -1 on empty
 *
 * @param[in]  x     x coordinate
 * @param[in]  y     y coordinate
 *
 * @return     The tile identifier.
 */
int Map::get_tile_id(int x, int y) {
	auto pair = std::pair<int, int>(x,y);
	auto got = this->tiles.find(pair);
	if(got != this->tiles.end()) {
		return got->second.tile_id;
	} else {
		return -1;
	}
}
