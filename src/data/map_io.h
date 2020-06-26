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

#include <memory>
#include <fstream>

#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#include "tile_manager.h"
#include "map.h"

class MapIO {
private:
    std::shared_ptr<TileManager> tile_manager;

public:
    /**
     * @brief      Constructs a new instance.
     */
    MapIO(const std::shared_ptr<TileManager>& _tile_manager);

    /**
     * @brief      Load map from file
     *
     * @param[in]  filename  The filename
     *
     * @return     shared pointer to map
     */
    std::shared_ptr<Map> load(const QString& filename);

    /**
     * @brief      Save map to filename
     *
     * @param[in]  filename  The filename
     */
    void save(const std::shared_ptr<Map>& map, const QString& filename);

    /**
     * @brief      Build the bill of materials
     *
     * @return     The bill of materials
     */
    QString build_bom_string(const std::shared_ptr<Map>& map) const;

    /**
     * @brief      Build the bill of materials
     *
     * @return     The bill of materials
     */
    std::unordered_map<std::string, unsigned int> build_bom_map(const std::shared_ptr<Map>& map) const;

private:
};
