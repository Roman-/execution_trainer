#include "probs.h"

Probs::Probs(map<QString, map<QString, QString> >& algorithms_table) {
    map<QString, map<QString, QString> >::iterator it1;
    for (it1 = algorithms_table.begin(); it1 != algorithms_table.end(); ++it1) {
        it1->first;
        map<QString, QString>::iterator it2;
        for (it2 = it1->second.begin(); it2 != it1->second.end(); ++it2) {
            if (it1->first != it2->first) // algs like AA are not available
                probs_[it1->first][it2->first] = 1;
        }
    }
}

uint Probs::size() const {
    uint result = 0;
    for (auto it1 = probs_.begin(); it1 != probs_.end(); ++it1)
        result += it1->second.size();
    return result;
}
double Probs::sum() {
    double result = 0;
    for (auto it1 = probs_.begin(); it1 != probs_.end(); ++it1) {
        for (auto it2 = it1->second.begin(); it2 != it1->second.end(); ++it2)
            result += probs_[it1->first][it2->first];
    }
    return result;
}

void Probs::normalize() {
    double amount = (double)size(); // how many algs
    double start_sum = sum(); // sum before normalization
    map<QString, map<QString, double> >::iterator it1;
    for (it1 = probs_.begin(); it1 != probs_.end(); ++it1) {
        map<QString, double>::iterator it2;
        for (it2 = it1->second.begin(); it2 != it1->second.end(); ++it2)
            probs_[it1->first][it2->first] = (probs_[it1->first][it2->first]) * amount / start_sum;
    }
}

void Probs::output() {
    map<QString, map<QString, double> >::iterator it1;
    for (it1 = probs_.begin(); it1 != probs_.end(); ++it1) {
        map<QString, double>::iterator it2;
        for (it2 = it1->second.begin(); it2 != it1->second.end(); ++it2)
            qDebug() << it1->first << it2->first << ": " << probs_[it1->first][it2->first];
    }
}

double& Probs::operator() (QString letter1, QString letter2) {
    auto it1 = probs_.find(letter1);
    if (it1 == probs_.cend()) {
        return operator()(letter2, letter1);
    }
    auto it2 = it1->second.find(letter2);
    if (it2 == it1->second.cend()) {
        return operator()(letter2, letter1);
    }
    return probs_[letter1][letter2];
}

Probs::Probs(QString file_path) {
    // stopped there
    /*
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
    */
}

QString to_string(vector<QString> r) {
    QString result;
    for (uint i = 0; i < r.size(); ++i)
        result.append(r[i]);
    return result;
}

QString Probs::define_next_letter(QString current_letter, vector<QString> available_letters) {
    qDebug() << "define_next_letter for " << current_letter << " from " << to_string(available_letters);
    double sum_of_available = 0;
    // calculating local sum
    for (QString l: available_letters)
        sum_of_available += operator ()(current_letter, l);
    qDebug() << "sum_of_available = " << sum_of_available;
    // generating random value
    double r = ((double)rand() / (double)RAND_MAX) * sum_of_available;
    // obtaining letter
    int i = 0; // index of available_letters vector
    while (true) {
        r -= operator ()(current_letter, available_letters[i]);
        qDebug() << available_letters[i] << ": r -= " << operator ()(current_letter, available_letters[i]) << " = " << r;
        if (r <= 0) {
            qDebug() << "next_letter for " << current_letter << " is " << available_letters[i];
            return available_letters[i];
        }
        ++i;
    }
}

std::pair<QString, QString> Probs::define_first_letterpair() {
    // temp: first letter-pair is always the hardest one
    std::pair<QString, QString> max_pair;
    double max_prob = -1;
    for (auto it1 = probs_.begin(); it1 != probs_.end(); ++it1) {
        for (auto it2 = it1->second.begin(); it2 != it1->second.end(); ++it2) {
            if (probs_[it1->first][it2->first] > max_prob) {
                max_prob = probs_[it1->first][it2->first];
                max_pair = make_pair(it1->first, it2->first);
            }
        }
    }
    //if (rand() % 2 == 0) // 50% times we swap first and second letters
        return max_pair;
    //return make_pair(max_pair.second, max_pair.first);
}

void Probs::save_to_file(QString filepath) {
    QFile out(filepath);
    if (!out.open(QIODevice::WriteOnly)) {
        qDebug() << "Probs::save_to_file(): cant open " << filepath;
        return;
    }
    QTextStream outstream( &out );
    outstream.setCodec("UTF-8");

    QString letter1, letter2;
    for (map<QString, map<QString, double> >::iterator row = probs_.begin(); row != probs_.end(); ++ row) {
        letter1 = row->first;
        for (map<QString, double>::const_iterator p = row->second.begin(); p != row->second.end(); ++ p) {
            letter2 = p->first;
            QString ankiLine = letter1 + " " + letter2 + ": " + p->second + "\n";
            outstream << ankiLine;
        }
    }
    out.close();
}

