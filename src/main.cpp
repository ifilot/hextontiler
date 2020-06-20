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

#include <QApplication>
#include <QDesktopWidget>
#include <QSurfaceFormat>
#include <QStatusBar>
#include <QDebug>

#include <iostream>
#include <iomanip>
#include <ctime>

#include "gui/mainwindow.h"
#include "config.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // write boot of program
    std::ofstream lf("execution.log");
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    lf << "Start application: " << std::put_time(&tm, "%d-%m-%Y %H-%M-%S") << std::endl;

    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    // set OpenGL surface settings
    QSurfaceFormat fmt;
    fmt.setDepthBufferSize(24);
    fmt.setSamples(4);
    fmt.setVersion(3, 3);
    fmt.setProfile(QSurfaceFormat::CoreProfile);
    fmt.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    QSurfaceFormat::setDefaultFormat(fmt);

    std::unique_ptr<MainWindow> mainWindow;

    try {
        // build main window
        mainWindow = std::make_unique<MainWindow>();
        mainWindow->setWindowTitle(QString(PROGRAM_NAME) + " " + QString(PROGRAM_VERSION));
        mainWindow->resize(1280,640);

    } catch(const std::exception& e) {
        // if any errors are caught in the process of starting up the application,
        // they will be printed in the execution.log file
        lf << "Error detected!" << std::endl;
        lf << e.what() << std::endl;
        auto texc = std::time(nullptr);
        auto tmexc = *std::localtime(&texc);
        lf << "Abnormal closing of program: " << std::put_time(&tmexc, "%d-%m-%Y %H-%M-%S") << std::endl;
        lf.close();
    }

    // show main window
    int desktopArea = QApplication::desktop()->width() *
                      QApplication::desktop()->height();
    int widgetArea = mainWindow->width() * mainWindow->height();

    if (((float)widgetArea / (float)desktopArea) < 0.75f) {
        mainWindow->show();
    } else {
        mainWindow->showMaximized();
    }

    lf.close();

    return app.exec();
}
