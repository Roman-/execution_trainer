#ifndef TABLES_LOADER_H
#define TABLES_LOADER_H

#include <QFile>
#include <map>
#include <cassert>
#include "linewithtabs.h"

using namespace std;

map<QString, map<QString, QString>> load_images(QString file_path);
vector<vector<QString> > load_speffz(QString file_path);
vector<QString> load_letters(QString file_path);
map<QString, map<QString, QString> > load_algs_table(QString file_path);
QString get_alg(const QString& l1, const QString& letter2, map<QString, map<QString, QString> >& algs); // <l1, l2> or <l2, l1>

#endif // TABLES_LOADER_H
