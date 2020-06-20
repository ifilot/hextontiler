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

#include "tile_selector.h"

/**
 * @brief      Constructs a new instance.
 *
 * @param      parent  The parent
 */
TileSelector::TileSelector(QWidget *parent) :
    QWidget(parent) {

    this->setMaximumWidth(350);

    // set layout
    QVBoxLayout *scroll_layout = new QVBoxLayout();
    this->setLayout(scroll_layout);

    this->label_selection = new QLabel("<i>Please select a tile from the list below.</i>");
    this->label_selection->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    scroll_layout->addWidget(this->label_selection);

    this->label_selection_image = new QLabel();
    this->label_selection_image->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    scroll_layout->addWidget(this->label_selection_image);

    QScrollArea *scrollarea = new QScrollArea(this);
    scrollarea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollarea->setWidgetResizable(true);
    scroll_layout->addWidget(scrollarea);

    QFrame *inner = new QFrame(scrollarea);
    scrollarea->setWidget(inner);
    this->layout = new QGridLayout();
    inner->setLayout(this->layout);

    this->long_names.emplace("AS", "Settements");
    this->long_names.emplace("AF", "Forts");
    this->long_names.emplace("AL", "Legendaries");
    this->long_names.emplace("AH", "Hills");
    this->long_names.emplace("AM", "Mountains");
    this->long_names.emplace("AP", "Plains");
    this->long_names.emplace("AV", "Rivers");
    this->long_names.emplace("AW", "Woodlands");
    this->long_names.emplace("AR", "Roads");

    this->populate_tile_list();
}

/**
 * @brief      Populate list of tiles
 */
void TileSelector::populate_tile_list() {
    QString filename = "tiledata.json";
    QTemporaryDir tmp_dir;
    QString path = tmp_dir.path() + "/" + filename;
    QFile::copy(":/assets/fragments/" + filename, path);

    std::unordered_map<std::string, unsigned int> tiles;
    std::vector<std::string> tilenames;

    try {
        boost::property_tree::ptree root;
        QTemporaryDir tmp_dir;
        QFile::copy(":/assets/configuration/tiledata.json", tmp_dir.path() + "/tiledata.json");
        boost::property_tree::read_json(tmp_dir.path().toStdString() + "/tiledata.json", root);

        for(auto iter = root.begin(); iter != root.end(); iter++) {
            std::string name = iter->first.substr(0,4);
            auto got = tiles.find(name);
            if(got != tiles.end()) {
                got->second++;
            } else {
                tiles.emplace(name, 1);
                tilenames.push_back(name);
            }
        }

    } catch(std::exception const& ex) {
        throw std::runtime_error("Could not open or parse file: " + path.toStdString());
    }

    std::sort(tilenames.begin(), tilenames.end());

    std::string oldname = "";
    QVector3D color;
    int rownr = -1;
    int colnr = 0;
    for(const auto& tilename : tilenames) {
        if(tilename.substr(0,2) == "ST") {
            continue;
        }

        if(tilename.substr(0,2) != oldname) {
            rownr++;
            auto got = this->long_names.find(tilename.substr(0,2));
            this->layout->addWidget(new QLabel(got->second.c_str()), rownr, 0);
            rownr++;
            colnr = 0;
            color = this->get_tile_color(tilename.substr(0,2)) * 200;
        }

        this->buttons.push_back(new QPushButton(tilename.c_str()));
        this->layout->addWidget(this->buttons.back(), rownr, colnr);
        QString path = tr(":/assets/tiles/icons/") + tr(tilename.c_str()) + tr("_000.png");
        this->buttons.back()->setIcon(QIcon(path));
        this->buttons.back()->setIconSize(QSize(32, 32));
        this->buttons.back()->setStyleSheet(tr("background-color: rgb(%1,%2,%3); font-weight: bold; color: #CCC;").arg(color[0]).arg(color[1]).arg(color[2]));

        colnr++;
        if(colnr == 3) {
            colnr = 0;
            rownr++;
        }
        oldname = tilename.substr(0,2);
    }

    for(QPushButton *button : this->buttons) {
        connect(button, SIGNAL(clicked()), this, SLOT(select_new_tile()));
    }
}

/**
 * @brief      Select new tile
 */
void TileSelector::select_new_tile() {
    QObject* obj = sender();
    QPushButton* but = qobject_cast<QPushButton*>(obj);

    // set name
    std::string tilename = but->text().toStdString();
    auto got = this->long_names.find(tilename.substr(0,2));
    std::string tile_id = but->text().toStdString().substr(tilename.size()-2);
    this->label_selection->setText(tr("<b>") + tr(got->second.c_str()) + tr("</b> Tile #") + tr(tile_id.c_str()));

    // set pixmap
    QString path = tr(":/assets/tiles/tiles/") + but->text() + tr("_000.png");
    QPixmap pixmap(path);
    this->label_selection_image->setPixmap(pixmap);

    // set pixmap color
    auto color = this->get_tile_color(tilename.substr(0,2)) * 200;
    this->label_selection_image->setStyleSheet(tr("background-color: rgb(%1,%2,%3); border-radius: 10px;").arg(color[0]).arg(color[1]).arg(color[2]));

    emit(signal_tile_selected(but->text()));
}

/**
 * @brief      Gets the tile color.
 *
 * @param[in]  tilecode  The tilecode
 *
 * @return     The tile color.
 */
QVector3D TileSelector::get_tile_color(const std::string& tilecode) const {
    if(tilecode == "AS") { // settlements
        return QVector3D(0.477, 0.352, 0.262);
    }

    if(tilecode == "AF") { // forts
        return QVector3D(0.694, 0.294, 0.400);
    }

    if(tilecode == "AH") { // hills
        return QVector3D(0.494, 0.537, 0.271);
    }

    if(tilecode == "AL") { // legendary
        return QVector3D(0.773, 0.624, 0.000);
    }

    if(tilecode == "AR") { // roads
        return QVector3D(0.471, 0.369, 0.231);
    }

    if(tilecode == "AM") { // mountains
        return QVector3D(0.471, 0.369, 0.545);
    }

    if(tilecode == "AM") { // mountains
        return QVector3D(0.471, 0.369, 0.545);
    }

    if(tilecode == "AP") { // plains
        return QVector3D(0.251, 0.475, 0.259);
    }

    if(tilecode == "AV") { // river
        return QVector3D(0.000, 0.447, 0.733);
    }

    if(tilecode == "AW") { // woodlands
        return QVector3D(0.114, 0.306, 0.090);
    }

    return QVector3D(1.0, 1.0, 1.0);
}
