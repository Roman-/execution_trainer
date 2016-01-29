#include "tables_loader.h"

vector<vector<QString> > load_speffz(QString file_path) {
    vector<vector<QString> > result;
    QFile file(file_path);
    assert(file.open(QIODevice::ReadOnly | QIODevice::Text) && "open file");
    while (!file.atEnd()) {
        vector<QString> current_row;
        QString line = file.readLine();
        QTextStream stream(&line);
        while (!stream.atEnd()) {
            QString current_letter;
            stream >> current_letter;
            if (current_letter != "")
                current_row.push_back(current_letter);
        }
        result.push_back(current_row);
    }
    return result;
}

vector<QString> load_letters(QString file_path) {
    vector<QString> result;
    QFile file(file_path);
    assert(file.open(QIODevice::ReadOnly | QIODevice::Text) && "open file");
    while (!file.atEnd()) {
        QString line = file.readLine();
        QTextStream stream(&line);
        while (!stream.atEnd()) {
            QString current_letter;
            stream >> current_letter;
            if (current_letter != "")
                result.push_back(current_letter);
        }
    }
    return result;
}

map<QString, map<QString, QString> > load_images(QString file_path) {
    map<QString, map<QString, QString>> result;
    vector<QString> letters_vec;
    QFile file(file_path);
    assert(file.open(QIODevice::ReadOnly | QIODevice::Text) && "open file");
    /* first line = letters */
    QString letters = file.readLine();
    QTextStream letters_stream(&letters);
    while (!letters_stream.atEnd()) {
        QString current_letter;
        letters_stream >> current_letter;
        if (current_letter != "") {
            result[current_letter] = map<QString, QString>();
            letters_vec.push_back(current_letter);
        }
    }
    /* the rest of them */
    QString current_letter;
    int i = 0; // letter number
    while (!file.atEnd()) {
        i = 0;
        QString line = file.readLine(), current_word;
        QTextStream stream(&line);
        stream >> current_letter; // row
        while (!stream.atEnd()) {
            stream >> current_word;
            if (current_word != "")
                result[current_letter][letters_vec[i++]] = current_word;
        }
    }
    return result;
}

map<QString, map<QString, QString> > load_algs_table(QString file_path) {
    map<QString, map<QString, QString>> result;
    QVector<QString> letters_vec;
    QFile file(file_path);
    assert(file.open(QIODevice::ReadOnly | QIODevice::Text) && "cant open file");
    /* first line = letters */
    QString letters = file.readLine();
    QTextStream letters_stream(&letters);
    while (!letters_stream.atEnd()) {
        QString current_letter;
        letters_stream >> current_letter;
        if (current_letter.length() == 1) {
            result[current_letter] = map<QString, QString>();
            letters_vec.push_back(current_letter);
        }
    }
    letters_vec.push_back("??");
    /* the rest of them */
    QString current_letter;
    int i = 0; // letter number
    while (!file.atEnd()) {
        i = 0;
        LineWithTabs line = QString(file.readLine());
        current_letter = line.next().mid(0, 1); // letter
        //qDebug() << "at line: " << current_letter;
        while (line.hasNext()) {
            QString alg = line.next();
            if (alg != "") {
                result[current_letter][letters_vec[i]] = alg;
                //qDebug() << current_letter << ", " << letters_vec[i] << ": " << alg;
            }
            i++;
        }
    }
    return result;
}
