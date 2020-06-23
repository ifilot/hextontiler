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
    QVBoxLayout *layout_main = new QVBoxLayout();
    this->setLayout(layout_main);

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

    connect(button_copy_to_clipboard, SIGNAL(released()), this, SLOT(action_copy_to_clipboard()));
    connect(button_confirm, SIGNAL(released()), this, SLOT(confirm()));

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
