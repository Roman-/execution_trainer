#ifndef PROBS_H
#define PROBS_H

#include <QDebug>
#include <QString>
#include <QFile>
#include <map>
#include <ctime>
#include <cassert>
#include "linewithtabs.h"

using namespace std;

/* probabilities for letter-pair */
class Probs
{
public:
    Probs(QString file_path);
    Probs(map<QString, map<QString, QString> >& algorithms_table); // creating probs with all 1s
    double& operator() (QString letter1, QString letter2); // if P<a,b> not found then return P<b,a>
    uint size() const; // amount of existing algs
    double sum(); // sum of all probabilities
    void normalize(); // make sum be equal to 1*size
    void output(); // prints probs to console
    std::pair<QString, QString> define_first_letterpair();
    // based on probability of letter-pair <current, available[i]> defines next letter in chain
    QString define_next_letter(QString current_letter, vector<QString> available_letters);
    void save_to_file(QString filepath);
private:
    map<QString, map<QString, double>> probs_; // probabilities
};

QString to_string(vector<QString> r);


#endif // PROBS_H
