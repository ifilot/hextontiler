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

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <QCoreApplication>
#include <QString>
#include <QGuiApplication>
#include <QScreen>
#include <QSysInfo>
#include <QDebug>
#include <QTimer>
#include <QCursor>

#include <QtCore/qmath.h>
#include <QtCore/qvariant.h>
#include <QOpenGLTextureBlitter>
#include <QOpenGLFramebufferObject>

#include <sstream>
#include <fstream>
#include <boost/format.hpp>
#include <math.h>
#include <string>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/norm.hpp>

#include "shader_program_manager.h"
#include "shader_program_types.h"
#include "map_renderer.h"
#include "scene.h"

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class AnaglyphWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

private:
    QPoint m_lastPos;

    QString root_path;
    QPoint top_left;

    QOpenGLTextureBlitter blitter;
    QOpenGLFramebufferObject *fbo;

    std::shared_ptr<ShaderProgramManager> shader_manager;
    std::shared_ptr<Scene> scene;
    std::shared_ptr<Map> map;
    std::shared_ptr<TileManager> tile_manager;
    std::unique_ptr<MapRenderer> map_renderer;

    QPoint mouse_lastpos;
    QPoint mouse_drag_center;
    bool flag_drag_map = false;

    // anti-aliasing
    int aa = 4;

public:
    AnaglyphWidget(const std::shared_ptr<Scene>& _scene,
                   const std::shared_ptr<TileManager>& _tile_manager,
                   QWidget *parent = 0);

    void window_move_event();

    QSize minimumSizeHint() const Q_DECL_OVERRIDE;

    QSize sizeHint() const Q_DECL_OVERRIDE;

    ~AnaglyphWidget();

    inline void set_map(const std::shared_ptr<Map>& _map) {
        this->map = _map;
        this->map_renderer->set_map(this->map);
    }

public slots:
    /**
     * @brief      Clean up this object
     */
    void cleanup();

protected:
    /**
     * @brief      Initialize OpenGL environment
     */
    void initializeGL() Q_DECL_OVERRIDE;

    /**
     * @brief      Render scene
     */
    void paintGL() Q_DECL_OVERRIDE;

    /**
     * @brief      Resize window
     *
     * @param[in]  width   screen width
     * @param[in]  height  screen height
     */
    void resizeGL(int width, int height) Q_DECL_OVERRIDE;

    /**
     * @brief      Parse mouse press event
     *
     * @param      event  The event
     */
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    /**
     * @brief      Parse mouse release event
     *
     * @param      event  The event
     */
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    /**
     * @brief      Parse mouse move event
     *
     * @param      event  The event
     */
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    /**
     * @brief      Parse mouse wheel event
     *
     * @param      event  The event
     */
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;

private:
    /**
     * @brief      Load OpenGL shaders
     */
    void load_shaders();

    /**
     * @brief      Sets the projection matrix.
     */
    void set_projection_matrix();

private slots:
    /**
     * @brief      Action to perform when a frame is swapped
     */
    void action_frame_swapped();

signals:
    /**
     * @brief      Send signal that opengl engine is ready
     */
    void opengl_ready();
};
