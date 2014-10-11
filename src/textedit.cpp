#include "textedit.h"
#include <QApplication>
#include <QClipboard>
#include <QToolButton>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QDebug>
#include <QScrollBar>

TextEdit::TextEdit(QWidget *parent) :
    QTextEdit(parent)
{
}

void TextEdit::addAction(QAction *action)
{
    QToolButton *button = new QToolButton(this);
    button->setDefaultAction(action);
    button->hide();
    button->setAutoRaise(true);
    mBtnList.append(button);

    QTextEdit::addAction(action);
}

void TextEdit::enterEvent(QEvent *e)
{
    foreach(QToolButton *btn, mBtnList)
            btn->show();
    e->accept();
}

void TextEdit::leaveEvent(QEvent *e)
{
    foreach(QToolButton *btn, mBtnList)
            btn->hide();
        e->accept();
}

void TextEdit::paintEvent(QPaintEvent *e)
{
    QTextEdit::paintEvent(e);

    if (mBtnList.isEmpty())
        return;

    const int toolbutton_width = mBtnList.first()->sizeHint().height();
    const int toolbutton_height = mBtnList.first()->sizeHint().height();
    const int scrollbar_width = verticalScrollBar()->sizeHint().width();
    const QPoint offset(toolbutton_width + scrollbar_width + 6, toolbutton_height + 6);
    const QPoint btn_pos = mapFromParent(geometry().bottomRight() - offset);
    int padding = 0;

    foreach(QToolButton *btn, mBtnList) {
        btn->move(btn_pos.x() - padding , btn_pos.y());
        padding += btn->width() + 2;
    }
}

void TextEdit::copy()
{
    QApplication::clipboard()->setText(toPlainText());
}
