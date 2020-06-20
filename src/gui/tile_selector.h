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
#include <QLabel>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QListWidget>
#include <QTemporaryDir>
#include <QPushButton>
#include <QScrollArea>
#include <QDebug>
#include <QVector3D>
#include <QGridLayout>

// boost headers
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

#include <unordered_map>

class TileSelector : public QWidget {
    Q_OBJECT

private:
    QLabel *label_selection;
    QGridLayout *layout;
    QLabel *label_selection_image;

    std::unordered_map<std::string, std::string> long_names;
    std::vector<QPushButton*> buttons;

public:
    /**
     * @brief      Constructs a new instance.
     *
     * @param      parent  The parent
     */
    TileSelector(QWidget *parent = nullptr);

private:
    /**
     * @brief      Populate list of tiles
     */
    void populate_tile_list();

    /**
     * @brief      Gets the tile color.
     *
     * @param[in]  tilecode  The tilecode
     *
     * @return     The tile color.
     */
    QVector3D get_tile_color(const std::string& tilecode) const;

private slots:
    /**
     * @brief      Select new tile
     */
    void select_new_tile();

signals:
	/**
     * @brief   Signal that new tile is selected
     *
     * @param[in]  tilecode  tilecode
     */
    void signal_tile_selected(const QString& tilecode);
};
