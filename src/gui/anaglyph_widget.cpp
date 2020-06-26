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

#include "anaglyph_widget.h"

/**
 * @brief      Constructs a new instance.
 *
 * @param      parent  The parent
 */
AnaglyphWidget::AnaglyphWidget(
    const std::shared_ptr<Scene>& _scene,
    const std::shared_ptr<TileManager>& _tile_manager,
    QWidget *parent)
    : QOpenGLWidget(parent),
      scene(_scene),
      tile_manager(_tile_manager) {
    const QString operatingsystem = QSysInfo::productType();

    this->shader_manager = std::make_shared<ShaderProgramManager>();

    auto pTimer = new QTimer(this);
    pTimer->start(1000 / 60.0);

    this->setMouseTracking(true);

    connect(this, SIGNAL(frameSwapped()), this, SLOT(action_frame_swapped()));
}

/**
 * @brief      Destroys the object.
 */
AnaglyphWidget::~AnaglyphWidget() {
    cleanup();
}

/**
 * @brief      Set the minimum window size
 *
 * @return     Minimimum window size
 */
QSize AnaglyphWidget::minimumSizeHint() const {
    return QSize(50, 50);
}

/**
 * @brief      Provide default window size
 *
 * @return     Default window size
 */
QSize AnaglyphWidget::sizeHint() const {
    return QSize(400, 400);
}

/**
 * @brief      Clean the widget
 */
void AnaglyphWidget::cleanup() {
    makeCurrent();
    doneCurrent();
}

/**
 * @brief      Initialize OpenGL environment
 */
void AnaglyphWidget::initializeGL() {
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &AnaglyphWidget::cleanup);

    this->initializeOpenGLFunctions();

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    this->map_renderer = std::make_unique<MapRenderer>(this->shader_manager, this->scene, this->tile_manager);

    this->load_shaders();

    this->blitter.create();
    this->fbo = new QOpenGLFramebufferObject(this->size() * this->aa);

    this->set_projection_matrix();
    emit(opengl_ready());
}

/**
 * @brief      Render scene
 */
void AnaglyphWidget::paintGL() {
    this->fbo->bind();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_BLEND);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
    glBlendEquation(GL_FUNC_ADD);
    glViewport(0, 0, this->width() * this->aa, this->height() * this->aa);
    this->map_renderer->draw();
    this->fbo->release();

    glViewport(0, 0, this->width(), this->height());

    blitter.bind();
    const QRect targetRect(QPoint(0, 0), this->fbo->size());
    const QMatrix4x4 target = QOpenGLTextureBlitter::targetTransform(targetRect, QRect(QPoint(0, 0), this->fbo->size()));
    blitter.blit(this->fbo->texture(), target, QOpenGLTextureBlitter::OriginBottomLeft);
    blitter.release();

    this->map_renderer->draw_debug();
}

/**
 * @brief      Resize window
 *
 * @param[in]  width   screen width
 * @param[in]  height  screen height
 */
void AnaglyphWidget::resizeGL(int w, int h) {
    // store sizes
    this->scene->canvas_width = w;
    this->scene->canvas_height = h;
    this->set_projection_matrix();

    delete this->fbo;
    this->fbo = new QOpenGLFramebufferObject(this->size() * this->aa);
}

/**
 * @brief      Parse mouse press event
 *
 * @param      event  The event
 */
void AnaglyphWidget::mousePressEvent(QMouseEvent *event) {
    // handle dragging
    if (event->buttons() & Qt::RightButton && !this->flag_drag_map) {
        this->setCursor(Qt::SizeAllCursor);
        this->mouse_drag_center = event->pos();
        this->flag_drag_map = true;
    }
}

/**
 * @brief      Parse mouse release event
 *
 * @param      event  The event
 */
void AnaglyphWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (this->flag_drag_map && !(event->buttons() & Qt::RightButton) ) {
        this->setCursor(Qt::ArrowCursor);
        this->flag_drag_map = false;
        this->scene->set_dragging(false);
    }
}

/**
 * @brief      Parse mouse move event
 *
 * @param      event  The event
 */
void AnaglyphWidget::mouseMoveEvent(QMouseEvent *event) {
    this->setFocus(Qt::MouseFocusReason);

    this->mouse_lastpos = event->pos();

    QVector3D ray_origin;
    QVector3D ray_direction;

    this->scene->calculate_ray(event->pos(), &ray_origin, &ray_direction);
    auto pos = this->scene->calculate_ray_plane_intersection(ray_origin, ray_direction, QVector3D(0.0, 0.0, 0.0), QVector3D(0.0, 0.0, 1.0));
    pos += this->scene->camera_look_at;
    this->scene->set_mouse_pos(pos);
}

/**
 * @brief      Parse mouse wheel event
 *
 * @param      event  The event
 */
void AnaglyphWidget::wheelEvent(QWheelEvent *event) {
    this->scene->camera_position += event->delta() * 0.01f * QVector3D(0, 0, 1);
    this->scene->camera_position[2] = glm::clamp(this->scene->camera_position[2], 4.0f, 40.0f);

    float w = (float)this->scene->canvas_width;
    float h = (float)this->scene->canvas_height;
    float ratio = w/h;
    float zoom = -this->scene->camera_position[2];
    this->scene->update_view();

    this->scene->projection.setToIdentity();
    this->scene->projection.ortho(-zoom/2.0f, zoom/2.0f, -zoom / ratio /2.0f, zoom / ratio / 2.0f, 0.01f, 1000.0f);

    this->update();
}

void AnaglyphWidget::window_move_event() {
    this->top_left = mapToGlobal(QPoint(0, 0));
    this->update();
}

/**
 * PRIVATE FUNCTIONS
 */

/**
 * @brief      Load OpenGL shaders
 */
void AnaglyphWidget::load_shaders() {
    // create regular shaders
    shader_manager->create_shader_program("sprite_shader", ShaderProgramType::SpriteShader, ":/assets/shaders/sprite.vs", ":/assets/shaders/sprite.fs");
    shader_manager->create_shader_program("background_shader", ShaderProgramType::SpriteShader, ":/assets/shaders/background.vs", ":/assets/shaders/background.fs");
    shader_manager->create_shader_program("line_shader", ShaderProgramType::SpriteShader, ":/assets/shaders/line.vs", ":/assets/shaders/line.fs");
}

/**
 * @brief      Sets the projection matrix.
 */
void AnaglyphWidget::set_projection_matrix() {
    this->scene->canvas_width = this->width();
    this->scene->canvas_height = this->height();
    float w = (float)this->scene->canvas_width;
    float h = (float)this->scene->canvas_height;
    float ratio = w/h;
    float zoom = -this->scene->camera_position[2];
    this->scene->projection.setToIdentity();
    this->scene->projection.ortho(-zoom/2.0f, zoom/2.0f, -zoom / ratio /2.0f, zoom / ratio / 2.0f, 0.01f, 1000.0f);
}

/**
 * @brief      Action to conduct when a frame is swapped
 */
void AnaglyphWidget::action_frame_swapped() {
    static const float drag_intensity = 0.0005f;

    if (this->flag_drag_map) {
        QPoint position_difference = this->mouse_drag_center - this->mouse_lastpos;
        this->scene->set_dragging(true);
        this->scene->camera_position += QVector3D(-position_difference.x(), position_difference.y(), 0.0) * drag_intensity;
        this->scene->camera_look_at += QVector3D(-position_difference.x(), position_difference.y(), 0.0) * drag_intensity;
        this->scene->update_view();
        this->update();
        return;
    }
}
