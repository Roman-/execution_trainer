#include "mylabel.h"

MyLabel::MyLabel(const QString& text, QWidget *parent, int fontSize, int x, int y): QLabel(parent)
{
    QLabel::setText(text);
    setFont(QFont("Times", fontSize));
    setStyleSheet("QLabel { background-color : transparent; color : white; }");
    setGeometry(x, y, 0, 0);
    state_ = 1;
    adjustSize(); // call this last
    //parent->layout()->addWidget(*this);
}

void MyLabel::slotClicked()
{
    qDebug() << " MyLabel::slotClicked()";
}

void MyLabel::setPos(int x, int y) {
    QLabel::setGeometry(x, y, size().width(), size().height());
}

void MyLabel::mousePressEvent ( QMouseEvent * event )
{
    emit clicked();
}

short MyLabel::state() {
    return state_;
}

void MyLabel::setText(const QString & text) {
    QLabel::setText(text);
    adjustSize();
}

void MyLabel::change_state() {
    state_ = (state_ + 1)%3;
    switch(state_) {
    case 0: setStyleSheet("QLabel { background-color : transparent; color : green; }"); break;
    case 1: setStyleSheet("QLabel { background-color : transparent; color : white; }"); break;
    case 2: setStyleSheet("QLabel { background-color : transparent; color : red; }"); break;
    }
}

void MyLabel::underline() {
    QFont f = QLabel::font();
    f.setUnderline(true);
    setFont(f);
}

void MyLabel::deunderline() {
    QFont f = QLabel::font();
    f.setUnderline(false);
    setFont(f);
}
