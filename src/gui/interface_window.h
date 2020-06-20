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

#include <QWidget>
#include <QSlider>
#include <QBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QStyle>
#include <QKeyEvent>
#include <QPushButton>
#include <QDesktopWidget>
#include <QApplication>
#include <QMessageBox>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QTimer>
#include <QSplitter>

#include "anaglyph_widget.h"
#include "tile_selector.h"
#include "mainwindow.h"
#include "user_action.h"
#include "../data/map_io.h"

QT_BEGIN_NAMESPACE
class QSlider;
class QPushButton;
QT_END_NAMESPACE

class MainWindow; // forward declaration to avoid circular dependencies

class InterfaceWindow : public QWidget {
    Q_OBJECT

private:
    MainWindow *mainWindow;

    AnaglyphWidget *anaglyph_widget;
    TileSelector *tile_selector;

    std::shared_ptr<UserAction> user_action;
    std::shared_ptr<Map> map;
    std::shared_ptr<Scene> scene;
    std::shared_ptr<TileManager> tile_manager;
    std::unique_ptr<MapIO> map_io;

public:
    /**
     * @brief      Constructs the object.
     *
     * @param      mw    pointer to MainWindow object
     */
    InterfaceWindow(MainWindow *mw);

    /**
     * @brief      Get pointer to anaglyph widget
     *
     * @return     The anaglyph widget.
     */
    inline AnaglyphWidget* get_anaglyph_widget() {
        return this->anaglyph_widget;
    }

private:


protected:
    /**
     * @brief      Button press event
     *
     * @param      event  The event
     */
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

public slots:
    /**
     * @brief      Opens a file.
     *
     * @param[in]  filename  The filename
     */
    void open_file(const QString& filename);

    /**
     * @brief      Save to a file.
     *
     * @param[in]  filename  The filename
     */
    void save_file(const QString& filename);

private slots:
    /**
     * @brief      OpenGL ready function
     */
    void slot_opengl_ready();

    /**
     * @brief      Center the camera on the map
     */
    void action_center_map();

    /**
     * @brief      Build bill of materials
     */
    void action_build_bom();

signals:
    /**
     * @brief      Signal when new file is loaded
     */
    void new_file_loaded();
};
