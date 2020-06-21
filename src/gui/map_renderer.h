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

#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QDebug>
#include <QMatrix4x4>
#include <QtMath>
#include <QOpenGLTexture>

#include "shader_program_manager.h"
#include "scene.h"
#include "../data/tile_manager.h"
#include "../data/map.h"

class MapRenderer {
private:
    std::shared_ptr<ShaderProgramManager> shader_manager;
    std::shared_ptr<Scene> scene;

    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vbo[3];

    QOpenGLTexture *tilespackage;

    std::shared_ptr<TileManager> tile_manager;

    std::shared_ptr<Map> map;

public:
    /**
     * @brief      Constructs a new instance.
     *
     * @param[in]  _shader_manager  The shader manager
     * @param[in]  _scene           The scene
     * @param[in]  _tile_manager    The tile manager
     */
    MapRenderer(const std::shared_ptr<ShaderProgramManager>& _shader_manager,
                const std::shared_ptr<Scene>& _scene,
                const std::shared_ptr<TileManager>& _tile_manager);

    /**
     * @brief      Draw the actual tiles
     */
    void draw();

    /**
     * @brief      Draw map background
     */
    void draw_template_map();

    /**
     * @brief      Sets the map.
     *
     * @param[in]  _map  The map
     */
    inline void set_map(const std::shared_ptr<Map>& _map) {
        this->map = _map;
    }

private:
    /**
     * @brief      Build vertex array objects
     */
    void build_vao();
};
