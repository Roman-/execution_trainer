#include "linewithtabs.h"

QString LineWithTabs::next() {
    QString result;
    while(hasNext() && line_[index_] != '\t')
        result += line_[index_++];
    index_++; // tab
    return result;
}

bool LineWithTabs::hasNext() {
    return (index_ < line_.length()
            && line_[index_] != '\n');
}

QString LineWithTabs::get_initial_line() const {
    return line_;
}

int LineWithTabs::get_index() const {
    return index_;
}
