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

#include "map_renderer.h"


/**
 * @brief      Constructs a new instance.
 *
 * @param[in]  _shader_manager  The shader manager
 * @param[in]  _scene           The scene
 * @param[in]  _tile_manager    The tile manager
 */
MapRenderer::MapRenderer(const std::shared_ptr<ShaderProgramManager>& _shader_manager,
                         const std::shared_ptr<Scene>& _scene,
                         const std::shared_ptr<TileManager>& _tile_manager) :
    shader_manager(_shader_manager),
    scene(_scene),
    tile_manager(_tile_manager) {
    this->build_vao();

    this->tilespackage = new QOpenGLTexture(QImage(QString(":/assets/tiles/tilespackage.png")));
}

/**
 * @brief      Draw the actual tiles
 */
void MapRenderer::draw() {
    this->draw_template_map();

    ShaderProgram *model_shader = this->shader_manager->get_shader_program("sprite_shader");
    model_shader->bind();

    QMatrix4x4 model;
    model.setToIdentity();
    QMatrix4x4 mvp = this->scene->projection * this->scene->view * model;

    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    // draw tile
    this->vao.bind();
    this->tilespackage->bind();

    auto tilehighlight = this->scene->get_hexpos_highlight();
    bool highlight = false;

    for(const auto& tile : this->map->get_tiles()) {
        model.setToIdentity();
        auto tilepos = QVector3D(tile.second.x, tile.second.y, tile.second.z);
        model.translate(this->scene->hexcube_to_cartesian(tilepos));
        mvp = this->scene->projection * this->scene->view * model;
        model_shader->set_uniform("mvp", mvp);

        QVector3D color = this->tile_manager->get_color(tile.second.tile_id);
        if(tilehighlight == tilepos && !this->scene->flag_dragging) {
            color = 0.5 * color + 0.25 * QVector3D(1.0f, 1.0f, 1.0f);
            highlight = true;
        }

        model_shader->set_uniform("color", color);

        QVector4D uv = this->tile_manager->get_uv(tile.second.tile_id);
        std::vector<float> uvs = {uv[0], uv[3], uv[2], uv[3], uv[2], uv[1], uv[0], uv[1]};
        this->vbo[1].bind();
        this->vbo[1].allocate(&uvs[0], uvs.size() * sizeof(float));

        f->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    if(!highlight && QVector3D::dotProduct(QVector3D(1.0, 1.0, 1.0), tilehighlight) == 0 && !this->scene->flag_dragging) {
        model.setToIdentity();
        model.translate(this->scene->hexcube_to_cartesian(tilehighlight));
        mvp = this->scene->projection * this->scene->view * model;
        model_shader->set_uniform("mvp", mvp);

        QVector3D color = QVector3D(0.150, 0.150, 0.150);
        model_shader->set_uniform("color", color);

        QVector4D uv = this->tile_manager->get_uv(this->tile_manager->get_tile_id("ST00_000"));
        std::vector<float> uvs = {uv[0], uv[3], uv[2], uv[3], uv[2], uv[1], uv[0], uv[1]};
        this->vbo[1].bind();
        this->vbo[1].allocate(&uvs[0], uvs.size() * sizeof(float));

        f->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    this->vao.release();

    model_shader->release();
}

/**
 * @brief      Draw map background
 */
void MapRenderer::draw_template_map() {
    // determine hexpositions
    auto leftbottom = this->scene->get_hexpos_at_mousepos(QPoint(0,0));
    auto righttop = this->scene->get_hexpos_at_mousepos(QPoint(this->scene->canvas_width,this->scene->canvas_height));

    ShaderProgram *model_shader = this->shader_manager->get_shader_program("background_shader");
    model_shader->bind();

    QMatrix4x4 model;
    model.setToIdentity();
    QMatrix4x4 mvp = this->scene->projection * this->scene->view * model;

    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    // draw tile
    this->vao.bind();
    this->tilespackage->bind();

    for(int y = righttop.y(); y <= leftbottom.y(); y++) {
        for(int x = leftbottom.x(); x <= righttop.x(); x++) {
            model.setToIdentity();
            int z = -(x + y);
            QVector3D tilepos(x,y,z);
            model.translate(this->scene->hexcube_to_cartesian(tilepos));
            mvp = this->scene->projection * this->scene->view * model;
            model_shader->set_uniform("mvp", mvp);

            QVector3D color = QVector3D(0.058, 0.065, 0.070);
            model_shader->set_uniform("color", color);

            QVector4D uv = this->tile_manager->get_uv(this->tile_manager->get_tile_id("ST00_000"));
            std::vector<float> uvs = {uv[0], uv[3], uv[2], uv[3], uv[2], uv[1], uv[0], uv[1]};
            this->vbo[1].bind();
            this->vbo[1].allocate(&uvs[0], uvs.size() * sizeof(float));

            f->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }
    }

    this->vao.release();

    model_shader->release();
}

/**
 * @brief      Build vertex array objects
 */
void MapRenderer::build_vao() {
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    this->vao.create();
    this->vao.bind();

    std::vector<float> vertices = {-0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f};
    this->vbo[0].create();
    this->vbo[0].setUsagePattern(QOpenGLBuffer::StaticDraw);
    this->vbo[0].bind();
    this->vbo[0].allocate(&vertices[0], vertices.size() * sizeof(float));
    f->glEnableVertexAttribArray(0);
    f->glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    std::vector<float> uvs = {0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0};
    this->vbo[1].create();
    this->vbo[1].setUsagePattern(QOpenGLBuffer::DynamicDraw);
    this->vbo[1].bind();
    this->vbo[1].allocate(&uvs[0], uvs.size() * sizeof(float));
    f->glEnableVertexAttribArray(1);
    f->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    std::vector<unsigned int> indices = {0,1,3,1,2,3};
    this->vbo[2] = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    this->vbo[2].create();
    this->vbo[2].setUsagePattern(QOpenGLBuffer::StaticDraw);
    this->vbo[2].bind();
    this->vbo[2].allocate(&indices[0], 6 * sizeof(unsigned int));

    this->vao.release();
}
