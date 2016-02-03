#ifndef LINEWITHTABS_H
#define LINEWITHTABS_H

#include <QMap>
#include <QDebug>
#include <QString>
#include <QFile>
#include <cassert>

class LineWithTabs {
    int index_;
    QString line_;
public:
    LineWithTabs(QString line): line_(line) {index_ = 0;}
    QString next();
    bool hasNext();
    QString get_initial_line() const;
    int get_index() const;
};
#endif // LINEWITHTABS_H
