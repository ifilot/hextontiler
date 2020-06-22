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

#include "scene.h"

Scene::Scene() {
    this->camera_position = QVector3D(0.0f, 0.0f, 10.0f);
    this->camera_look_at = QVector3D(0.0f, 0.0f, 0.0f);
    this->update_view();

    this->build_transformation_matrices();
}

/**
 * @brief       calculate a ray originating based on mouse position and current view
 *
 * @param       mouse position
 * @param       pointer to vector holding ray origin
 * @param       pointer to vector holding ray direction
 * @return      void
 */
void Scene::calculate_ray(const QPoint& mouse_position, QVector3D* ray_origin, QVector3D* ray_direction) const {
    const float screen_width = (float)this->canvas_width;
    const float screen_height = (float)this->canvas_height;

    const QVector3D ray_nds = QVector3D((2.0f * (float)mouse_position.x()) / screen_width - 1.0f,
                                         1.0f - (2.0f * (float)mouse_position.y()) / screen_height,
                                         1.0);

    const QVector4D ray_clip(ray_nds[0], ray_nds[1], 0.0, 1.0);

    // the position on the 'camera screen' determines the origin of the
    // ray vector in orthographic projection
    QVector4D ray_eye = this->projection.inverted() * ray_clip;
    ray_eye = QVector4D(ray_eye[0], ray_eye[1], 0.0, 0.0);
    *ray_origin = (this->camera_position + this->view.inverted() * ray_eye).toVector3D();

    // if the projection is orthographic, the ray vector is the same
    // as the view direction of the camera (in world space)
    *ray_direction = -this->camera_position.normalized();
}

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
QVector3D Scene::calculate_ray_plane_intersection(const QVector3D& ray_origin,
                                                  const QVector3D& ray_vector,
                                                  const QVector3D& plane_origin,
                                                  const QVector3D& plane_normal) const {

    float dotprod = QVector3D::dotProduct(ray_vector, plane_normal);

    if(std::fabs(dotprod) < 0.001) {
        return QVector3D(-1, -1, -1);
    } else {
        float t = QVector3D::dotProduct(plane_origin - ray_origin, plane_normal) / dotprod;
        return ray_origin + t * ray_vector;
    }
}

/**
 * @brief      Convert hexcube coordinates to Cartesian
 *
 * @param[in]  hexcoord  The hexcoord
 *
 * @return     The 3D vector.
 */
QVector3D Scene::hexcube_to_cartesian(const QVector3D& hexcoord) const {
    return hex2cart * hexcoord;
}

/**
 * @brief      Convert hexcube coordinates to Cartesian tilecenter
 *
 * @param[in]  scale  Tile scale
 *
 * @return     The 3D vector.
 */
QVector3D Scene::get_tile_offset(float scale) const {
    static const float offset = 0.5f * (1.0 - std::cos(qDegreesToRadians(this->blender_projection_angle)));
    return QVector3D(0.0f, offset * scale, 0.0f);
}

/**
 * @brief      Convert Cartesian coordinates to hexcube coordinates
 *
 * @param[in]  hexcoord  The hexcoord
 *
 * @return     The 3D vector.
 */
QVector3D Scene::cartesian_to_hexcube(const QVector3D& cartcoord) const {
    return cart2hex * cartcoord;
}

/**
 * @brief      Set mouse position in world space to determine which tile to highlight
 */
void Scene::set_mouse_pos(const QVector3D& worldpos) {
    auto new_tile_highlight = this->cartesian_to_hexcube(worldpos);
    new_tile_highlight[0] = std::round(new_tile_highlight[0]);
    new_tile_highlight[1] = std::round(new_tile_highlight[1]);
    new_tile_highlight[2] = std::round(new_tile_highlight[2]);

    if(new_tile_highlight != this->tile_highlight) {
        this->tile_highlight = new_tile_highlight;
        emit(signal_update_screen());
    }
}

/**
 * @brief      Gets the hexpos given mouse position.
 *
 * @param[in]  mouse_position  The mouse position
 *
 * @return     The hexpos at mouse position.
 */
QVector3D Scene::get_hexpos_at_mousepos(const QPoint& mouse_position) const {
    QVector3D ray_origin, ray_direction;
    this->calculate_ray(mouse_position, &ray_origin, &ray_direction);
    auto worldpos = this->calculate_ray_plane_intersection(ray_origin, ray_direction, QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 0.0f, 1.0f));
    worldpos += this->camera_look_at;
    auto hexpos = this->cartesian_to_hexcube(worldpos);
    hexpos[0] = std::round(hexpos[0]);
    hexpos[1] = std::round(hexpos[1]);
    hexpos[2] = std::round(hexpos[2]);

    return hexpos;
}

/**
 * @brief      Update view matrix
 */
void Scene::update_view() {
    this->view.setToIdentity();
    this->view.lookAt(this->camera_position, this->camera_look_at, QVector3D(0.0, -1.0, 0.0));
}

/**
 * @brief      Builds transformation matrices.
 *
 * @param[in]  dist  The distance
 */
void Scene::build_transformation_matrices() {
    // hexcoord to cartesian
    // cubic coordinates to cartesian
    static const float t = std::sqrt(3.0f) / (2.0 * std::sqrt(2.0));
    const QMatrix4x4 basetransform(
        1.50,  0.75,     0.75,   0.00,
        0.00,  0.50*t,  -0.50*t, 0.00,
        1.00,  1.00,     1.00,   0.00,  // note that this line will always give 0 for z :-)
        0.00,  0.00,     0.00,   1.00
    );

    // fix coordinate transformations
    this->hex2cart = basetransform;
    this->cart2hex = basetransform.inverted();
}
