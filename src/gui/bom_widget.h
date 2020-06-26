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
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QCheckBox>
#include <QDialog>
#include <QVector>
#include <QLabel>
#include <QPlainTextEdit>
#include <QTemporaryDir>
#include <QDesktopServices>
#include <QMessageBox>

#include <boost/lexical_cast.hpp>
#include <unordered_map>
#include <boost/algorithm/string.hpp>

#include <fstream>

#include "../data/map.h"

class BomWidget : public QDialog {
    Q_OBJECT

private:
    QPlainTextEdit *text_edit;
    std::vector<std::unordered_map<std::string, unsigned int>> store_conversion_tables;
    std::unordered_map<std::string, unsigned int> bom;

public:
    /**
     * @brief      Constructs a new instance.
     *
     * @param      parent  The parent
     */
    BomWidget(QWidget *parent = 0);

    /**
     * @brief      Sets the text.
     *
     * @param[in]  text  The text
     */
    inline void set_text(const QString& text) {
        this->text_edit->setPlainText(text);
    }

    /**
     * @brief      Sets the map.
     *
     * @param[in]  _bom  The bom
     */
    inline void set_map(const std::unordered_map<std::string, unsigned int> _bom) {
        this->bom = _bom;
    }

    /**
     * @brief      Destroys the object.
     */
    ~BomWidget();

private:
    /**
     * @brief      Reads a store conversion table from a file
     *
     * @param[in]  filename  The filename
     *
     * @return     Return map
     */
    std::unordered_map<std::string, unsigned int> read_conversion_table(const QString& filename);

public slots:

private slots:
    /**
     * @brief      Confirm
     */
    void action_copy_to_clipboard();

    /**
     * @brief      Confirm
     */
    void confirm();

    /**
     * @brief      Launch browser to order tiles
     */
    void action_order();
};
