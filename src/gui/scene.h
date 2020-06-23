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

#include <cmath>
#include <QMatrix4x4>
#include <QtMath>

/**
 * @brief      This class describes a camera alignment.
 */

class Scene : public QObject {
    Q_OBJECT

public:
    QMatrix4x4 projection;
    QMatrix4x4 view;
    QVector3D camera_position;
    QVector3D camera_look_at;

    static constexpr float blender_projection_angle = 45.0f;
    static constexpr float tiledist = 0.92;

    int canvas_width;
    int canvas_height;

    // transformation matrices
    QMatrix4x4 hex2cart;
    QMatrix4x4 cart2hex;

    QVector3D tile_highlight; // which tile to highlight

    bool flag_dragging = false;
    bool tile_colors = true;

    Scene();

    /**
     * @brief       calculate a ray originating based on mouse position and current view
     *
     * @param       mouse position
     * @param       pointer to vector holding ray origin
     * @param       pointer to vector holding ray direction
     * @return      void
     */
    void calculate_ray(const QPoint& mouse_position, QVector3D* ray_origin, QVector3D* ray_direction) const;

    /**
     * @brief      Calculates the point of intersection of a ray with a plane
     *
     * @param[in]  ray_origin    The ray origin
     * @param[in]  ray_vector    The ray vector
     * @param[in]  plane_origin  The plane origin
     * @param[in]  plane_normal  The plane normal
     *
     * @return     The ray plane intersection.
     */
    QVector3D calculate_ray_plane_intersection(const QVector3D& ray_origin,
                                               const QVector3D& ray_vector,
                                               const QVector3D& plane_origin,
                                               const QVector3D& plane_normal) const;

    /**
     * @brief      Convert hexcube coordinates to Cartesian
     *
     * @param[in]  hexcoord  The hexcoord
     *
     * @return     The 3D vector.
     */
    QVector3D hexcube_to_cartesian(const QVector3D& hexcoord) const;

    /**
     * @brief      Convert hexcube coordinates to Cartesian tilecenter
     *
     * @param[in]  scale  Tile scale
     *
     * @return     The 3D vector.
     */
    QVector3D get_tile_offset(float scale) const;

    /**
     * @brief      Convert Cartesian coordinates to hexcube coordinates
     *
     * @param[in]  cartcoord  The Cartesian coordinates
     *
     * @return     The 3D vector.
     */
    QVector3D cartesian_to_hexcube(const QVector3D& cartcoord) const;

    /**
     * @brief      Set mouse position in world space to determine which tile to highlight
     */
    void set_mouse_pos(const QVector3D& worldpos);

    /**
     * @brief      Gets the coordinates of the to be highlighted hextile
     *
     * @return     The hexpos highlight.
     */
    inline const QVector3D& get_hexpos_highlight() const {
        return this->tile_highlight;
    }

    /**
     * @brief      Gets the hexpos given mouse position.
     *
     * @param[in]  mouse_position  The mouse position
     *
     * @return     The hexpos at mouse position.
     */
    QVector3D get_hexpos_at_mousepos(const QPoint& mouse_position) const;

    /**
     * @brief      Update view matrix
     */
    void update_view();

    /**
     * @brief      Sets the dragging.
     *
     * @param[in]  dragging  The dragging
     */
    inline void set_dragging(bool dragging) {
        this->flag_dragging = dragging;
    }

private:
    /**
     * @brief      Builds transformation matrices.
     */
    void build_transformation_matrices();

signals:
    /**
     * @brief      Signal to update the screen
     */
    void signal_update_screen();

};
