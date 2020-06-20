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

#include "user_action.h"

UserAction::UserAction(const std::shared_ptr<Scene> _scene,
		       		   const std::shared_ptr<Map> _map,
		       		   const std::shared_ptr<TileManager>& _tile_manager) :
	scene(_scene),
	map(_map),
	tile_manager(_tile_manager) {

}

/**
 * @brief      Adds a tile.
 */
void UserAction::add_tile() {
	auto pos = this->scene->get_hexpos_highlight();
	if(QVector3D::dotProduct(QVector3D(1.0, 1.0, 1.0), pos) == 0 && this->active_tile_id >= 0) {
		this->map->add_tile(this->active_tile_id, pos[0], pos[1], pos[2]);
	}
}

/**
 * @brief      Remove a tile
 */
void UserAction::remove_tile() {
    auto pos = this->scene->get_hexpos_highlight();
    this->map->remove_tile(pos[0], pos[1]);
}

/**
 * @brief   Substitute a tile
 */
void UserAction::rotate_tile() {
	auto pos = this->scene->get_hexpos_highlight();
	if(QVector3D::dotProduct(QVector3D(1.0, 1.0, 1.0), pos) == 0) {
		int tile_id = this->map->get_tile_id(pos[0], pos[1]);
		if(tile_id >= 0) {
			auto name = this->tile_manager->get_tilename(tile_id);
			int angle = boost::lexical_cast<int>(name.substr(name.size()-3, 3));
			angle = (angle + 60) % 360;
			int new_tile_id = this->tile_manager->get_tile_id(name.substr(0,5) + (boost::format("%03i") % angle).str());
			this->map->substitute_tile(new_tile_id, pos[0], pos[1]);
		}
	}
}

/**
 * @brief   Substitute a tile
 */
void UserAction::substitute_tile() {
	auto pos = this->scene->get_hexpos_highlight();
	if(QVector3D::dotProduct(QVector3D(1.0, 1.0, 1.0), pos) == 0) {
		int tile_id = this->map->get_tile_id(pos[0], pos[1]);
		if(tile_id >= 0) {
			this->map->substitute_tile(this->active_tile_id, pos[0], pos[1]);
		}
	}
}

/**
 * @brief      Action when new tile is selected
 *
 * @param[in]  label  The label
 */
void UserAction::slot_new_tile(const QString& label) {
	this->active_tile_id = this->tile_manager->get_tile_id(label.toStdString() + "_000");
}
