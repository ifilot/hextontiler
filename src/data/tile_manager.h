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

#include <QFile>
#include <QTemporaryDir>
#include <QVector4D>
#include <QVector3D>

#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>
#include <exception>

// boost headers
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

class TileManager {
private:
    std::unordered_map<std::string, unsigned int> tile_ids;
    std::vector<std::string> tilenames;
    std::vector<QVector4D> uvs;
    std::vector<QVector3D> scales;
    std::vector<QVector3D> colors;

public:
    TileManager();

    inline const QVector4D& get_uv(unsigned int id) const {
        return this->uvs[id];
    }

    inline const QVector3D& get_scale(unsigned int id) const {
        return this->scales[id];
    }

    inline const QVector3D& get_color(unsigned int id) const {
        return this->colors[id];
    }

    /**
     * @brief      Gets the tile identifier.
     *
     * @return     The tile identifier.
     */
    unsigned int get_tile_id(const std::string& tile_id) const;

    inline const std::string& get_tilename(unsigned int tile_id) const {
        return this->tilenames[tile_id];
    }

private:
    QVector3D get_color_from_tilecode(const std::string& tile_id) const;
};
