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

#include "tile_manager.h"

TileManager::TileManager() {
    try {
        boost::property_tree::ptree root;
        QTemporaryDir tmp_dir;
        QFile::copy(":/assets/configuration/tiledata.json", tmp_dir.path() + "/tiledata.json");
        boost::property_tree::read_json(tmp_dir.path().toStdString() + "/tiledata.json", root);

        for(auto iter = root.begin(); iter != root.end(); iter++) {
            std::string name = iter->first;
            float uvx1 = root.get<double>(iter->first + ".uvx1");
            float uvx2 = root.get<double>(iter->first + ".uvx2");
            float uvy1 = root.get<double>(iter->first + ".uvy1");
            float uvy2 = root.get<double>(iter->first + ".uvy2");

            QVector4D uv(uvx1, uvy1, uvx2, uvy2);
            this->tile_ids.emplace(name, this->tilenames.size());
            this->tilenames.push_back(name);
            this->uvs.push_back(uv);
            this->colors.push_back(this->get_color_from_tilecode(iter->first.substr(0,2)));
        }

    } catch(std::exception const& ex) {
        std::cerr << "[ERROR] There was an error parsing the JSON tree" << std::endl;
        std::cerr << ex.what() << std::endl;
        std::cerr << "[ERROR] Terminating program" << std::endl;
        exit(-1);
    }
}

/**
 * @brief      Gets the tile identifier.
 *
 * @return     The tile identifier.
 */
unsigned int TileManager::get_tile_id(const std::string& tile_id) const {
    auto got = this->tile_ids.find(tile_id);
    if(got != this->tile_ids.end()) {
        return got->second;
    } else {
        throw std::runtime_error("Invalid tile id requested: " + tile_id);
    }
}

QVector3D TileManager::get_color_from_tilecode(const std::string& tile_id) const {
    if(tile_id == "AS") { // settlements
        return QVector3D(0.477, 0.352, 0.262);
    }

    if(tile_id == "AF") { // forts
        return QVector3D(0.694, 0.294, 0.400);
    }

    if(tile_id == "AH") { // hills
        return QVector3D(0.494, 0.537, 0.271);
    }

    if(tile_id == "AL") { // legendary
        return QVector3D(0.773, 0.624, 0.000);
    }

    if(tile_id == "AR") { // roads
        return QVector3D(0.471, 0.369, 0.231);
    }

    if(tile_id == "AM") { // mountains
        return QVector3D(0.471, 0.369, 0.545);
    }

    if(tile_id == "AM") { // mountains
        return QVector3D(0.471, 0.369, 0.545);
    }

    if(tile_id == "AP") { // plains
        return QVector3D(0.251, 0.475, 0.259);
    }

    if(tile_id == "AV") { // river
        return QVector3D(0.000, 0.447, 0.733);
    }

    if(tile_id == "AW") { // woodlands
        return QVector3D(0.114, 0.306, 0.090);
    }

    return QVector3D(1.0, 1.0, 1.0);
}
