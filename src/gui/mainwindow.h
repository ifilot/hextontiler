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

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QMessageBox>
#include <QFileDialog>
#include <QStatusBar>
#include <QString>
#include <QtWidgets/QApplication>
#include <QFileInfo>
#include <QMimeData>

#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>

#include "interface_window.h"
#include "../config.h"

class InterfaceWindow; // forward declaration to avoid circular dependencies

/**
 * @brief      Class for main window.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    InterfaceWindow* interface_window;
    QLabel* statusbar_projection_icon;

public:
    /**
     * @brief      Constructs the object.
     */
    MainWindow();

protected:
    void moveEvent(QMoveEvent*) Q_DECL_OVERRIDE;

private slots:
    /**
     * @brief      Open a new object file
     */
    void open();

    /**
     * @brief      Save map to file
     */
    void save();

    /**
     * @brief      Close the application
     */
    void exit();

    /**
     * @brief      Display about menu
     */
    void about();

    /**
     * @brief      Set stereo projection
     */
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dropEvent(QDropEvent *event);

private:
    /**
     * @brief      Loads a theme.
     */
    void load_theme();
};
