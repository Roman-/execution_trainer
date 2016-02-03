#ifndef MYLABEL_H
#define MYLABEL_H

#include <QObject>
#include <QDebug>
#include <QLabel>

class MyLabel : public QLabel
{
    Q_OBJECT
public:
    MyLabel(const QString& text, QWidget * parent, int fontSize = 15, int x = 0, int y = 0);
    short state(); // getter for state
    void setText(const QString &text);
    void change_state();
    void underline();
    void deunderline();
    void setPos(int x, int y); // set position
signals:
    void clicked();

public slots:
    void slotClicked();

protected:
    void mousePressEvent ( QMouseEvent * event ) ;
    short state_; // 0 = nothing, 1 = hard, 2 = ez
};

#endif // MYLABEL_H
