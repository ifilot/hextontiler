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

#include "bom_widget.h"

/**
 * @brief      Constructs a new instance.
 *
 * @param      parent  The parent
 */
BomWidget::BomWidget(QWidget *parent) : QDialog(parent, Qt::Popup | Qt:: Dialog) {
    QHBoxLayout *top_layout = new QHBoxLayout();
    this->setLayout(top_layout);

    // set BOM

    QWidget *left_container = new QWidget(this);
    top_layout->addWidget(left_container);
    QVBoxLayout *layout_main = new QVBoxLayout();
    left_container->setLayout(layout_main);

    this->text_edit = new QPlainTextEdit();
    layout_main->addWidget(text_edit);
    text_edit->setReadOnly(true);

    QWidget *widget_buttons = new QWidget();
    QHBoxLayout *layout_buttons = new QHBoxLayout();
    widget_buttons->setLayout(layout_buttons);
    layout_main->addWidget(widget_buttons);

    QPushButton *button_copy_to_clipboard = new QPushButton("&Copy to clipboard", this);
    QPushButton *button_confirm = new QPushButton("&Ok", this);

    layout_buttons->addWidget(button_confirm);
    layout_buttons->addWidget(button_copy_to_clipboard);

    // set shop buttons

    QWidget *right_container = new QWidget(this);
    top_layout->addWidget(right_container);
    QVBoxLayout *shop_layout = new QVBoxLayout();
    right_container->setLayout(shop_layout);
    shop_layout->addWidget(new QLabel("Order direct at these webshops"));
    QPushButton *button_mtechcave = new QPushButton("mtechcave.com");
    shop_layout->addWidget(button_mtechcave);
    shop_layout->addWidget(new QFrame());

    connect(button_copy_to_clipboard, SIGNAL(released()), this, SLOT(action_copy_to_clipboard()));
    connect(button_confirm, SIGNAL(released()), this, SLOT(confirm()));
    connect(button_mtechcave, SIGNAL(released()), this, SLOT(action_order()));

    this->setWindowModality(Qt::WindowModal);
    this->show();
}

/**
 * @brief      Close the window
 */
void BomWidget::action_copy_to_clipboard() {
    this->text_edit->selectAll();
    this->text_edit->copy();
    auto text_cursor = this->text_edit->textCursor();
    text_cursor.clearSelection();
    this->text_edit->setTextCursor(text_cursor);
}

/**
 * @brief      Confirm
 */
void BomWidget::confirm() {
    this->close();
}

/**
 * @brief      Destroys the object.
 */
BomWidget::~BomWidget() {

}

std::unordered_map<std::string, unsigned int> BomWidget::read_conversion_table(const QString& filename) {
    QTemporaryDir tmp_dir;
    QFile::copy(":/assets/shops/" + filename, tmp_dir.path() + filename);

    std::ifstream infile((tmp_dir.path() + filename).toStdString());
    std::string line;

    std::unordered_map<std::string, unsigned int> convtable;

    while(std::getline(infile, line)) {
        std::vector<std::string> pieces;
        boost::split(pieces, line, boost::is_any_of("\t "), boost::token_compress_on);
        if(pieces.size() == 2) {
            convtable.emplace(pieces[0], boost::lexical_cast<unsigned int>(pieces[1]));
        }
    }

    infile.close();

    return convtable;
}

/**
 * @brief      Launch browser to order tiles
 */
void BomWidget::action_order() {
    auto convtable = this->read_conversion_table("mtechcave_com.dat");

    QString url = "https://mtechcave.com/cart/?add-to-cart=";

    for(const auto& tile : this->bom) {
        for(unsigned int i=0; i<tile.second; i++) {
            url += QString::number(convtable.find(tile.first)->second) + ",";
        }
    }

    if(url.size() > 2048) {
        int ret = QMessageBox::critical(this, tr("Cannot submit this order"),
                               tr("The url to submit this order is longer than 2048 characters. Please cut up your order in chunks."),
                               QMessageBox::Ok,
                               QMessageBox::Save);
    } else {
        QDesktopServices::openUrl(QUrl(url));
    }
}
