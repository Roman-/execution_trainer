#include "generator.h"

QString vector_to_qstring(vector<QString> r) {
    QString result;
    for (uint i = 0; i < r.size(); ++i) {
        result.append(r[i]);
        if (i%2)
            result.append(" ");
    }
    return result;
}

QString vector_to_words(vector<QString> r, map<QString, map<QString, QString>> images) {
    assert(r.size() % 2 == 0 && "odd");
    QString result;
    for (uint i = 0; i < r.size(); i += 2) {
        result.append(images[r[i]][r[i+1]]);
        result.append(" ");
    }
    return result;
}

//QString to_string(vector<QString> r) {
//    QString result;
//    for (uint i = 0; i < r.size(); ++i)
//        result.append(r[i]);
//    return result;
//}

//QString to_string(vector<int> r) {
//    QString result;
//    for (uint i = 0; i < r.size(); ++i)
//        result.append(QString::number(r[i]));
//    return result;
//}



vector<QString> generate_chain_by_shuffling(vector<pair<int, int>> chain, vector<vector<QString>> speffz) {
    rotate(chain.begin(),
           chain.begin() + rand() %  chain.size(),
           chain.end());
    for (auto& sp_arr: speffz)
        rotate(sp_arr.begin(),
               sp_arr.begin() + rand() %  sp_arr.size(),
               sp_arr.end());
    random_shuffle(speffz.begin(), speffz.end());

    vector<QString> r; // result
    for (auto c_el: chain)
        r.push_back( speffz[c_el.first][c_el.second] );
    return r;
}

int side_that_contain(const vector<vector<QString>>& speffz, const QString& letter) {
    for (uint i = 0; i < speffz.size(); ++i)
        if (std::find(speffz[i].begin(), speffz[i].end(), letter) != speffz[i].end()) {
            return i;
    }
    assert(false && "not qool");
}

vector<int> get_available_sides(const vector<vector<QString>>& original_speffz,
                                const vector<vector<QString>>& speffz, // in use
                             const vector<QString>& letters_perm,
                             int& i, int& m, int& n) {
        vector<int> available_sides = {0, 1, 2, 3, 4, 5}; // 6 sides, corresponding to speffz
        //remove empty
        for (int i = 0; i < 6; ++i) {
            if (speffz[i].size() == 0) {
                available_sides.erase(std::remove(available_sides.begin(),
                                                  available_sides.end(),
                                                  i));
            }
        }
        // remove prev
        if (i != 0) {
            int side_to_remove = side_that_contain(
                    original_speffz, // sides with letters
                    letters_perm[i - 1]);

            available_sides.erase(
                std::remove(available_sides.begin(),
                            available_sides.end(),
                            side_to_remove),
                available_sides.end());
        }
        if (i == n) {
            int side_to_remove = side_that_contain(
                    original_speffz, // sides with letters
                    letters_perm[m]);
            available_sides.erase(std::remove(available_sides.begin(),
                                              available_sides.end(),
                                              side_to_remove),
                                  available_sides.end());
        }
        if (i == n - 1) {
            int side_to_remove = side_that_contain(
                    original_speffz, // sides with letters
                    letters_perm[0]);
            available_sides.erase(std::remove(available_sides.begin(),
                                              available_sides.end(),
                                              side_to_remove),
                                  available_sides.end());
        }
        return available_sides;
}

vector<QString> generate_initial_chain_type1(vector<vector<QString>> speffz, uint length) { // X and T centers
    length -= length % 4;
    if (length < 12)
        length = 12;
    /* initial permutation */
    vector<vector<QString>> original_speffz(speffz);
    int n = length / 2 - 2;
    int m = (rand() % (n/2-1)) * 2 + 1;
    vector<QString> letters_perm;
    for (int i = 0; i <= n; ++i) {
        vector<int> available_sides = get_available_sides(original_speffz,
                                                          speffz,
                                                        letters_perm,
                                                        i, m, n);
        if (available_sides.size() == 0) // no available sides
            return generate_initial_chain_type1(original_speffz, length - 4); // usually means that length is too big
        int side_number = available_sides[rand() % available_sides.size()];
        int sticker_number = rand() % speffz[side_number].size();
        letters_perm.push_back( speffz[side_number][sticker_number] );
        speffz[side_number].erase(speffz[side_number].begin() + sticker_number); // remove current
    }
    /* finish the chain */
    for (int i = m; i >= 0; --i)
        letters_perm.push_back(letters_perm[i]);
    for (int i = n - 1; i >= m; --i)
        letters_perm.push_back(letters_perm[i]);
    letters_perm.push_back(letters_perm[n]);
    letters_perm.push_back(letters_perm[n - 1]);
    std::rotate(letters_perm.begin(),
                letters_perm.begin() + 1,//letters_perm.begin() + rand() % letters_perm.size(),
                letters_perm.end());
    return letters_perm;
}

vector<QString> generate_initial_chain_type1(vector<vector<QString>> speffz, uint length, Probs p) { // X and T centers
    length -= length % 4;
    if (length < 12)
        length = 12;
    vector<vector<QString>> original_speffz(speffz); // initial permutation
    int n = length / 2 - 2;
    int m = (rand() % (n/2-1)) * 2 + 1;
    vector<QString> letters_perm;
    // first letter-pair based on topX biggest probs
    std::pair<QString, QString> fp = p.define_first_letterpair();
    letters_perm.push_back(fp.first);
    letters_perm.push_back(fp.second);
    remove_letter_from_speffz(speffz, fp.first);
    remove_letter_from_speffz(speffz, fp.second);
    for (int i = 2; i <= n; ++i) {
        vector<int> available_sides = get_available_sides(original_speffz,
                                                          speffz,
                                                        letters_perm,
                                                        i, m, n);
        assert(available_sides.size() != 0 && "no available sides, size is 2 big");

        QString letter = p.define_next_letter(letters_perm[i-1],
                make_available_letters_vector(speffz, available_sides));
        letters_perm.push_back( letter );
        remove_letter_from_speffz(speffz, letter);
    }
    /* finish the chain */
    for (int i = m; i >= 0; --i)
        letters_perm.push_back(letters_perm[i]);
    for (int i = n - 1; i >= m; --i)
        letters_perm.push_back(letters_perm[i]);
    letters_perm.push_back(letters_perm[n]);
    letters_perm.push_back(letters_perm[n - 1]);
    /*std::rotate(letters_perm.begin(),
                letters_perm.begin() + 1,//letters_perm.begin() + rand() % letters_perm.size(),
                letters_perm.end()); */
    return letters_perm;
}

vector<QString> generate_initial_chain_wings(vector<QString> letters, uint length) {
    vector<QString> initial_letters(letters);
    length -= length % 4;
    if (length < 12)
        length = 12;
    /* initial permutation */
    int n = length / 2 - 2;
    int m = (rand() % (n/2-1)) * 2 + 1;
    vector<QString> letters_perm;
    for (int i = 0; i <= n; ++i) {
        if (letters.size() == 0)
            return generate_initial_chain_wings(initial_letters, length - 4);
        int letter_number = rand() % letters.size();
        letters_perm.push_back( letters[letter_number] );
        letters.erase(letters.begin() + letter_number); // remove current
    }
    /* finish the chain */
    for (int i = m; i >= 0; --i)
        letters_perm.push_back(letters_perm[i]);
    for (int i = n - 1; i >= m; --i)
        letters_perm.push_back(letters_perm[i]);
    letters_perm.push_back(letters_perm[n]);
    letters_perm.push_back(letters_perm[n - 1]);
    std::rotate(letters_perm.begin(),
                letters_perm.begin() + 2,//letters_perm.begin() + rand() % letters_perm.size(),
                letters_perm.end());
    return letters_perm;
}

vector<QString> generate_initial_chain_wings(vector<QString> letters, uint length, Probs p) {
    length -= length % 4;
    if (length < 12)
        length = 12;
    /* initial permutation */
    int n = length / 2 - 2;
    int m = (rand() % (n/2-1)) * 2 + 1;
    vector<QString> letters_perm;
    // first letter-pair based on topX biggest probs
    std::pair<QString, QString> fp = p.define_first_letterpair();
    letters_perm.push_back(fp.first);
    letters_perm.push_back(fp.second);
    letters.erase(std::find(letters.begin(), letters.end(), fp.first)); // remove current;
    letters.erase(std::find(letters.begin(), letters.end(), fp.second)); // remove current;
    for (int i = 2; i <= n; ++i) {
        assert(letters.size() != 0);
        QString letter = p.define_next_letter(letters_perm[i-1], letters);
        letters_perm.push_back( letter );
        letters.erase(std::find(letters.begin(), letters.end(), letter)); // remove current
    }
    /* finish the chain */
    for (int i = m; i >= 0; --i)
        letters_perm.push_back(letters_perm[i]);
    for (int i = n - 1; i >= m; --i)
        letters_perm.push_back(letters_perm[i]);
    letters_perm.push_back(letters_perm[n]);
    letters_perm.push_back(letters_perm[n - 1]);
    std::rotate(letters_perm.begin(),
                letters_perm.begin() + 2,//letters_perm.begin() + rand() % letters_perm.size(),
                letters_perm.end());
    return letters_perm;
}

void remove_letter_from_speffz(vector<vector<QString>> &speffz, QString letter) {
    for (vector<QString>& v: speffz)
        v.erase( std::remove( v.begin(), v.end(), letter ), v.end() );
}

vector<QString> make_available_letters_vector(const vector<vector<QString>>& speffz, const vector<int> &available_sides) {
    vector<QString> result;
    for (int side: available_sides) {
        for (QString letter: speffz[side])
            result.push_back(letter);
    }
    return result;
}
