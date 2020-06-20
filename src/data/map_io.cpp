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

#include "map_io.h"

/**
 * @brief      Constructs a new instance.
 */
MapIO::MapIO(const std::shared_ptr<TileManager>& _tile_manager) :
    tile_manager(_tile_manager) {

}

/**
 * @brief      Load map from file
 *
 * @param[in]  filename  The filename
 *
 * @return     shared pointer to map
 */
std::shared_ptr<Map> MapIO::load(const QString& filename) {
    std::ifstream infile(filename.toStdString());

    std::string line;
    auto map = std::make_shared<Map>();
    while(std::getline(infile, line)) {
        boost::trim(line);
        std::vector<std::string> pieces;
        boost::split(pieces, line, boost::is_any_of("\t "), boost::token_compress_on);

        std::string name = pieces[0];
        std::string angle = pieces[1];
        int x = boost::lexical_cast<int>(pieces[2]);
        int y = boost::lexical_cast<int>(pieces[3]);
        int z = boost::lexical_cast<int>(pieces[4]);
        int tile_id = this->tile_manager->get_tile_id(name + "_" + angle);

        map->add_tile(tile_id, x, y, z);
    }

    infile.close();

    return map;
}

/**
 * @brief      Save map to filename
 *
 * @param[in]  filename  The filename
 */
void MapIO::save(const std::shared_ptr<Map>& map, const QString& filename) {
    std::ofstream outfile(filename.toStdString());

    for(const auto& tile : map->get_tiles()) {
        std::string name = this->tile_manager->get_tilename(tile.second.tile_id);
        std::string tilecode = name.substr(0,4);
        int angle = boost::lexical_cast<int>(name.substr(name.size() - 3, 3));
        outfile << (boost::format("%s  %03i  %+04i  %+04i  %+04i\n") % tilecode % angle % tile.second.x % tile.second.y % tile.second.z).str();
    }

    outfile.close();
}

/**
 * @brief      Build the bill of materials
 *
 * @return     The bill of materials
 */
QString MapIO::build_bom(const std::shared_ptr<Map>& map) const {
    QString result;

    std::unordered_map<std::string, unsigned int> tiles;
    std::vector<std::string> tilenames;

    for(const auto& tile : map->get_tiles()) {
        std::string name = this->tile_manager->get_tilename(tile.second.tile_id).substr(0,4);
        auto got = tiles.find(name);
        if(got != tiles.end()) {
            got->second++;
        } else {
            tilenames.push_back(name);
            tiles.emplace(name, 1);
        }
    }

    std::sort(tilenames.begin(), tilenames.end());

    for(const auto& tilename : tilenames) {
        auto got = tiles.find(tilename);
        result += QString("%1\t%2\n").arg(tilename.c_str()).arg(got->second);
    }

    return result;
}
