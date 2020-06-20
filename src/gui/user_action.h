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

#include <QObject>
#include <QString>

#include <boost/lexical_cast.hpp>

#include <memory>
#include "scene.h"
#include "../data/map.h"
#include "../data/tile_manager.h"

class UserAction : public QObject {
	Q_OBJECT

private:
	std::shared_ptr<Scene> scene;
	std::shared_ptr<Map> map;
	std::shared_ptr<TileManager> tile_manager;

	int active_tile_id = -1;

public:
	UserAction(const std::shared_ptr<Scene> _scene,
		       const std::shared_ptr<Map> _map,
		       const std::shared_ptr<TileManager>& _tile_manager);

    /**
     * @brief      Sets the map.
     *
     * @param[in]  _map  The map
     */
    inline void set_map(const std::shared_ptr<Map> _map) {
        this->map = _map;
    }

	/**
     * @brief      Adds a tile.
     */
    void add_tile();

    /**
     * @brief      Remove a tile
     */
    void remove_tile();

	/**
     * @brief   Rotate a tile
     */
    void rotate_tile();

	/**
     * @brief   Substitute a tile
     */
    void substitute_tile();

private:

public slots:
    /**
     * @brief      Action when new tile is selected
     *
     * @param[in]  label  The label
     */
	void slot_new_tile(const QString& label);
};
