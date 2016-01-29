#include "my_chains.h"

vector<pair<int, int> > my_corners_chain() {
    return {
        make_pair(1, 0),
        make_pair(2, 0),
        make_pair(3, 0),
        make_pair(4, 0),
        make_pair(3, 1),
        make_pair(2, 1),
        make_pair(1, 1),
        make_pair(3, 2),
        make_pair(4, 1),
        make_pair(3, 1)
    };
}

vector<pair<int, int> > my_edges_chain() {
    return {
        make_pair(1, 0),
        make_pair(2, 0),
        make_pair(3, 0),
        make_pair(4, 0),
        make_pair(3, 1),
        make_pair(2, 1),
        make_pair(1, 1),
        make_pair(3, 0),
        make_pair(4, 1),
        make_pair(3, 1)
    };
}

vector<pair<int, int> > my_short_corners_chain() {
    return {
        make_pair(0, 1),
        make_pair(1, 1),
        make_pair(0, 0),
        make_pair(1, 0),
        make_pair(0, 0),
        make_pair(1, 1)
    };
}
