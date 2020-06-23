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
#include "interface_window.h"

/**
 * @brief      Constructs the object.
 *
 * @param      Pointer to MainWindow object
 */
InterfaceWindow::InterfaceWindow(MainWindow *mw)
    : mainWindow(mw) {

    // add Splitter
    QHBoxLayout *container = new QHBoxLayout;
    QSplitter *splitter = new QSplitter();

    this->scene = std::make_shared<Scene>();

    // add anaglyph widget
    this->tile_manager = std::make_shared<TileManager>();
    this->map_io = std::make_unique<MapIO>(this->tile_manager);
    this->anaglyph_widget = new AnaglyphWidget(this->scene, this->tile_manager, this);
    splitter->addWidget(this->anaglyph_widget);
    this->anaglyph_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // add tile selector widget
    this->tile_selector = new TileSelector(this);
    splitter->addWidget(this->tile_selector);

    // put everything as central widget
    container->addWidget(splitter);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QWidget *w = new QWidget;
    w->setLayout(container);
    mainLayout->addWidget(w);
    this->setLayout(mainLayout);

    this->map = std::make_shared<Map>();
    this->map->add_tile(this->tile_manager->get_tile_id("AF02_000"), 0, 0, 0); // default empty map tile
    this->user_action = std::make_shared<UserAction>(this->scene, this->map, this->tile_manager);

    connect(this->anaglyph_widget, SIGNAL(opengl_ready()), this, SLOT(slot_opengl_ready()));
    connect(this->tile_selector, SIGNAL(signal_tile_selected(const QString&)), this->user_action.get(), SLOT(slot_new_tile(const QString&)));
    connect(this->scene.get(), SIGNAL(signal_update_screen()), this->anaglyph_widget, SLOT(update()));
}

/**
 * @brief      Button press event
 *
 * @param      event  The event
 */
void InterfaceWindow::keyPressEvent(QKeyEvent *e) {
    static const float scroll_intensity = 0.25;

    if(e->key() == Qt::Key_A && e->modifiers() == Qt::ShiftModifier) {
        this->user_action->add_tile();
        this->anaglyph_widget->update();
    }
    else if(e->key() == Qt::Key_R && e->modifiers() == Qt::ShiftModifier) {
        this->user_action->rotate_tile();
        this->anaglyph_widget->update();
    }
    else if(e->key() == Qt::Key_S && e->modifiers() == Qt::ShiftModifier) {
        this->user_action->substitute_tile();
        this->anaglyph_widget->update();
    }
    else if(e->key() == Qt::Key_Delete && e->modifiers() == Qt::NoModifier) {
        this->user_action->remove_tile();
        this->anaglyph_widget->update();
    }
    else if(e->key() == Qt::Key_W && e->modifiers() == Qt::NoModifier) {
        this->scene->camera_position += QVector3D(0.0, scroll_intensity, 0.0);
        this->scene->camera_look_at += QVector3D(0.0, scroll_intensity, 0.0);
        this->scene->update_view();
        this->anaglyph_widget->update();
    }
    else if(e->key() == Qt::Key_A && e->modifiers() == Qt::NoModifier) {
        this->scene->camera_position += QVector3D(-scroll_intensity, 0.0, 0.0);
        this->scene->camera_look_at += QVector3D(-scroll_intensity, 0.0, 0.0);
        this->scene->update_view();
        this->anaglyph_widget->update();
    }
    else if(e->key() == Qt::Key_S && e->modifiers() == Qt::NoModifier) {
        this->scene->camera_position += QVector3D(0.0, -scroll_intensity, 0.0);
        this->scene->camera_look_at += QVector3D(0.0, -scroll_intensity, 0.0);
        this->scene->update_view();
        this->anaglyph_widget->update();
    }
    else if(e->key() == Qt::Key_D && e->modifiers() == Qt::NoModifier) {
        this->scene->camera_position += QVector3D(scroll_intensity, 0.0, 0.0);
        this->scene->camera_look_at += QVector3D(scroll_intensity, 0.0, 0.0);
        this->scene->update_view();
        this->anaglyph_widget->update();
    }
    else {
        QWidget::keyPressEvent(e);
    }
}

/**
 * @brief      Center the camera on the map
 */
void InterfaceWindow::action_center_map() {
    this->scene->camera_position = QVector3D(0.0f, 0.0f, 10.0f);
    this->scene->camera_look_at = QVector3D(0.0f, 0.0f, 0.0f);
    this->scene->update_view();
    this->anaglyph_widget->update();
}

/**
 * @brief      Build bill of materials
 */
void InterfaceWindow::action_build_bom() {
    BomWidget bomwidget(this);
    bomwidget.set_text(this->map_io->build_bom(this->map));
    bomwidget.exec();
}

/**
 * @brief      OpenGL ready function
 */
void InterfaceWindow::slot_opengl_ready() {
    this->anaglyph_widget->set_map(this->map);
}

/**
 * @brief      Opens a file.
 *
 * @param[in]  filename  The filename
 */
void InterfaceWindow::open_file(const QString& filename) {
    auto newmap = this->map_io->load(filename);
    this->map = newmap;
    this->anaglyph_widget->set_map(newmap);
    this->user_action->set_map(newmap);
    emit(new_file_loaded());
}

/**
 * @brief      Save to a file.
 *
 * @param[in]  filename  The filename
 */
void InterfaceWindow::save_file(const QString& filename) {
    this->map_io->save(this->map, filename);
}
