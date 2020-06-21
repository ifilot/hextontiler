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

#include "mainwindow.h"

/**
 * @brief      Class for main window.
 */
MainWindow::MainWindow() {
    QMenuBar *menuBar = new QMenuBar;

    setMenuBar(menuBar);
    this->interface_window = new InterfaceWindow(this);
    setCentralWidget(this->interface_window);

    // add drop-down menus
    QMenu *menu_file = menuBar->addMenu(tr("&File"));
    QMenu *menu_view = menuBar->addMenu(tr("&View"));
    QMenu *menu_tools = menuBar->addMenu(tr("&Tools"));
    QMenu *menu_help = menuBar->addMenu(tr("&Help"));

    // actions for file menu
    QAction *action_open = new QAction(menu_file);
    QAction *action_save = new QAction(menu_file);
    QAction *action_quit = new QAction(menu_file);

    // actions for view menu
    QAction *action_center_map = new QAction(menu_view);

    // actions for tools menu
    QAction *action_construct_bom = new QAction(menu_tools);

    // actions for help menu
    QAction *action_about = new QAction(menu_help);

    // create actions for file menu
    action_open->setText(tr("Open"));
    action_open->setShortcuts(QKeySequence::Open);
    action_open->setIcon(QIcon(":/assets/icons/open.png"));
    action_save->setText(tr("Save"));
    action_save->setShortcuts(QKeySequence::Save);
    action_save->setIcon(QIcon(":/assets/icons/save.png"));
    action_quit->setText(tr("Quit"));
    action_quit->setShortcut(Qt::CTRL + Qt::Key_Q);
    action_quit->setIcon(QIcon(":/assets/icons/close.png"));

    // create actions for view menu
    action_center_map->setText(tr("Center map"));
    action_center_map->setShortcut(Qt::Key_0);
    action_center_map->setIcon(QIcon(":/assets/icons/center.png"));

    // create actions for tools menu
    action_construct_bom->setText(tr("Construct Bill of Materials"));
    action_construct_bom->setShortcut(Qt::CTRL + Qt::Key_B);
    action_construct_bom->setIcon(QIcon(":/assets/icons/list.png"));

    // create actions for about menu
    action_about->setText(tr("About"));
    action_about->setShortcut(Qt::CTRL + Qt::Key_Question);
    action_about->setIcon(QIcon(":/assets/icons/info.png"));

    // add actions to file menu
    menu_file->addAction(action_open);
    menu_file->addAction(action_save);
    menu_file->addAction(action_quit);

    // add actions to view menu
    menu_view->addAction(action_center_map);

    // add actions to tools menu
    menu_tools->addAction(action_construct_bom);

    // add actions to help menu
    menu_help->addAction(action_about);

    // connect actions file menu
    connect(action_open, &QAction::triggered, this, &MainWindow::open);
    connect(action_save, &QAction::triggered, this, &MainWindow::save);
    connect(action_quit, &QAction::triggered, this, &MainWindow::exit);

    // connect actions view menu
    connect(action_center_map, SIGNAL(triggered()), this->interface_window, SLOT(action_center_map()));

    // connect actions tools menu
    connect(action_construct_bom, SIGNAL(triggered()), this->interface_window, SLOT(action_build_bom()));

    // connect actions about menu
    connect(action_about, &QAction::triggered, this, &MainWindow::about);

    // set icon
    setWindowIcon(QIcon(":/assets/icons/hextontiler_logo_256.png"));

    // allow dropping of media files
    setAcceptDrops(true);

    // load dark theme
    this->load_theme();
}

/**
 * @brief      Open a new object file
 */
void MainWindow::open() {
    QString filename = QFileDialog::getOpenFileName(this, tr("Open file"), "", tr("Hexton tile map (*.htm);;"));

    if(filename.isEmpty()) {
        return;
    }

    // display load time
    this->interface_window->open_file(filename);
    statusBar()->showMessage("Loaded " + filename + ".");

    // set main window title
    this->setWindowTitle(QFileInfo(filename).fileName() + " - " + QString(PROGRAM_NAME));
}

/**
 * @brief      Open a new object file
 */
void MainWindow::save() {
    QString filename = QFileDialog::getSaveFileName(this, tr("Save file"), "", tr("Hexon tile map (*.htm);;"));

    if(filename.isEmpty()) {
        return;
    }

    // display load time
    this->interface_window->save_file(filename);
    statusBar()->showMessage("Saved to " + filename + ".");

    // set main window title
    this->setWindowTitle(QFileInfo(filename).fileName() + " - " + QString(PROGRAM_NAME));
}

/**
 * @brief      Close the application
 */
void MainWindow::exit() {
    QMessageBox msgBox;
    msgBox.setText("Exit program.");
    msgBox.setInformativeText("Are you sure you want to quit?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    int ret = msgBox.exec();

    switch (ret) {
      case QMessageBox::Ok:
          QApplication::quit();
          return;
      case QMessageBox::Cancel:
          return;
      default:
          // should never be reached
          return;
    }
}

/**
 * @brief      Display about menu
 */
void MainWindow::about() {
    QMessageBox message_box;
    message_box.setStyleSheet("font-weight: normal;");
    message_box.setText(PROGRAM_NAME
                        " "
                        PROGRAM_VERSION
                        ".\n\nAuthor: Ivo Filot <ivo@ivofilot.nl>\n\n"
                        "HEXTONTILER is free software:\n"
                        "you can redistribute it and/or modify it under the terms of the\n"
                        "GNU General Public License as published by the Free Software\n"
                        "Foundation, either version 3 of the License, or (at your option)\n"
                        "any later version.\n"
                        "\n"
                        "HEXTONTILER is distributed in the hope that it will be useful,\n"
                        "but WITHOUT ANY WARRANTY; without even the implied warranty\n"
                        "of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n"
                        "See the GNU General Public License for more details.\n"
                        "\n"
                        "You should have received a copy of the GNU General Public License\n"
                        "along with this program.  If not, see http://www.gnu.org/licenses/.\n\n"
                        "The actual tiles on which the renderings as shown in this program \n"
                        "are based are intellectual property of Graven Guild Ltd. \n"
                        "For more information, see https://www.gravenguild.com/.\n"
                        "The author of this program is not affiliated with Graven Guild.\n"
                        );
    message_box.setIcon(QMessageBox::Information);
    message_box.setWindowTitle("About Hextontiler");
    message_box.setWindowIcon(QIcon(":/assets/icons/hextontiler_logo_256.png"));
    message_box.setIconPixmap(QPixmap(":/assets/icons/hextontiler_logo_256.png"));
    message_box.exec();
}

/**
 * @brief      Handle windows move event
 *
 * Updates anaglyph window
 *
 * @param      event  MoveEvent
 */
void MainWindow::moveEvent(QMoveEvent* event) {
    this->interface_window->get_anaglyph_widget()->window_move_event();

    // this is just to silence a warning message, we are not using event here, but this is the callback form
    (void)event;
}

/**
 * @brief      Handles drag Enter event
 *
 * @param      event  The event
 */
void MainWindow::dragEnterEvent(QDragEnterEvent *event) {
    event->acceptProposedAction();
}

/**
 * @brief      Handles file drop event
 *
 * @param      event  The event
 */
void MainWindow::dropEvent(QDropEvent *event) {
    const QMimeData *mimeData = event->mimeData();

    if (mimeData->hasUrls()) {
        QList<QUrl> urlList = mimeData->urls();
        QString text;
        for (int i = 0; i < urlList.size() && i < 32; ++i) {
            QString url = urlList.at(i).path();

            // check if file exists, else show error message
            if(boost::filesystem::exists(url.toStdString())) {
                this->interface_window->open_file(url);
            } else {
                QMessageBox::critical(this, tr("Failed to load file"), tr("Could not load file. Did you try to load this file from a network drive? This is not supported.") );
                statusBar()->showMessage("Error loading file.");
                return;
            }
        }

        // show message after loading
        if(urlList.size() == 1) {
            statusBar()->showMessage("Loaded " + urlList.at(0).path() + ".");
        } else {
            statusBar()->showMessage("Loaded files.");
        }
    } else {
        statusBar()->showMessage("Could not identify dropped format. Ignoring...");
    }
}

/**
 * @brief      Handles drag move event
 *
 * @param      event  The event
 */
void MainWindow::dragMoveEvent(QDragMoveEvent *event) {
    event->acceptProposedAction();
}

/**
 * @brief      Handles event when object is dragged outside window
 *
 * @param      event  The event
 */
void MainWindow::dragLeaveEvent(QDragLeaveEvent *event) {
    event->accept();
}

/**
 * @brief      Loads a theme.
 */
void MainWindow::load_theme() {
    // load theme
    QFile f(":/assets/themes/darkorange/darkorange.qss");
    if (!f.exists())   {
        throw std::runtime_error("Cannot open theme file.");
    } else {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        qApp->setStyleSheet(ts.readAll());
    }
}
