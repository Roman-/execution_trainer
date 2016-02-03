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
    QFile file(file_path);
    assert(file.open(QIODevice::ReadOnly | QIODevice::Text) && "open file");
    while (!file.atEnd()) {
        QString line = ((QString)(file.readLine())).toUtf8();
        QTextStream stream(&line);
        QString l1, l2;
        double p;
        stream >> l1 >> l2 >> p;
        probs_[l1][l2] = p;
    }
}

QString to_string(vector<QString> r) {
    QString result;
    for (uint i = 0; i < r.size(); ++i)
        result.append(r[i]);
    return result;
}

QString Probs::define_next_letter(QString current_letter, vector<QString> available_letters) {
    //qDebug() << "define_next_letter for " << current_letter << " from " << to_string(available_letters);
    double sum_of_available = 0;
    // calculating local sum
    for (QString l: available_letters)
        sum_of_available += operator ()(current_letter, l);
    //qDebug() << "sum_of_available = " << sum_of_available;
    // generating random value
    double r = ((double)rand() / (double)RAND_MAX) * sum_of_available;
    // obtaining letter
    int i = 0; // index of available_letters vector
    while (true) {
        r -= operator ()(current_letter, available_letters[i]);
        //qDebug() << available_letters[i] << ": r -= " << operator ()(current_letter, available_letters[i]) << " = " << r;
        if (r <= 0) {
            //qDebug() << "next_letter for " << current_letter << " is " << available_letters[i];
            return available_letters[i];
        }
        ++i;
    }
}

bool pair_cmp_greater(const pair<double, pair<QString, QString>>& lhs, const pair<double, pair<QString, QString>>& rhs) {
    return lhs.first > rhs.first;
}

std::pair<QString, QString> Probs::define_first_letterpair(int n) {
    vector< pair<double, pair<QString, QString>> > v;
    // temp: first letter-pair is always the hardest one
    std::pair<QString, QString> max_pair;
    double max_prob = -1;
    for (auto it1 = probs_.begin(); it1 != probs_.end(); ++it1) {
        for (auto it2 = it1->second.begin(); it2 != it1->second.end(); ++it2) {
            v.push_back(make_pair(probs_[it1->first][it2->first], make_pair(it1->first, it2->first)));
        }
    }
    std::nth_element(v.begin(), v.begin() + n, v.end(), pair_cmp_greater);
    std::pair<QString, QString> result_pair = v[rand() % n].second; // random of first n elements
    if (rand() % 2 == 0) // 50% times we swap first and second letters
        return result_pair;
    return make_pair(result_pair.second, result_pair.first);
}

void Probs::save_to_file(QString filepath) {
    QFile out(filepath);
    if (!out.open(QIODevice::WriteOnly)) {
        qDebug() << "Probs::save_to_file(): cant open " << filepath;
        return;
    }
    QTextStream outstream( &out );
    outstream.setCodec("UTF-8");
    map<QString, map<QString, double> >::iterator it1;
    for (it1 = probs_.begin(); it1 != probs_.end(); ++it1) {
        map<QString, double>::iterator it2;
        for (it2 = it1->second.begin(); it2 != it1->second.end(); ++it2) {
            QString line = it1->first + " " + it2->first + " " + QString::number(it2->second) + "\n";
            outstream << line;
        }
    }
    out.close();
}
