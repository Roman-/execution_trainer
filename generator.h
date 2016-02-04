/*!
 * @file
 * @author Roman Strakhov <fpmk2@ya.ru>
 * There are 2 types of elements:
 * type_1: there's one sticker on each element.
 * type_2: there are more than one sticker on each
 * element.
 * type_1 elements: x-centers, t-centers, obliques
 * type_2 elements: corners, edges
 * Wings are special. Unlike centers, each wing piece
 * is unique. Nontheless, there's only one letter
 * denoting a wing. I prefer generating wings chain
 * the way I generate centers chain.
 *
*/
#ifndef GENERATOR_H
#define GENERATOR_H
#include <vector>
#include <map>
#include <cassert>
#include <QDebug>
#include <QString>
#include <algorithm>
#include "probs.h"
#include "globals.h"

using namespace std;

QString vector_to_qstring(vector<QString> r); // vector of letters to string: {A,B,C,D,E,F} -> "AB CD EF"
QString vector_to_words(vector<QString> r, map<QString, map<QString, QString>> images); // {A,B,C,F} -> "arbalet coffee"

// shift and shuffle initial corners / edges chain to get a new one with same properties
vector<QString> generate_chain_by_shuffling(vector<pair<int, int>> chain, vector<vector<QString>> speffz);
// centers (x, t, o)
vector<QString> generate_initial_chain_type1(vector<vector<QString>> speffz, uint length); // X and T centers
// wings (easiest chain to generate)
vector<QString> generate_initial_chain_wings(vector<QString> letters, uint length);
// using probs
vector<QString> generate_initial_chain_type1(vector<vector<QString>> speffz, uint length, Probs p);
vector<QString> generate_initial_chain_wings(vector<QString> letters, uint length, Probs p);
vector<QString> generate_chain_by_shuffling(vector<pair<int, int>> chain, vector<vector<QString>> speffz, Probs p);

// helper funcs
// removes given letter from vector of vector of letters (speffz that is currently used in chain generating func)
void remove_letter_from_speffz(vector<vector<QString>> &speffz, QString letter);
// creates a vector of available letters based on available sides and remaining lettres (speffz)
vector<QString> make_available_letters_vector(const vector<vector<QString> > &speffz, const vector<int> &available_sides);
// calculates total chain difficulty based on probs
double chain_difficulty(vector<QString> &chain, Probs &p);




//QString to_string(vector<QString> r);
//QString to_string(vector<int> r);
#endif // GENERATOR_H
