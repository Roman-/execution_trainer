#ifndef GLOBALS_H
#define GLOBALS_H

// difficulty coefficients
const double k_neutral = 0.9;
const double k_hard = 2;
const double k_easy = 0.4;

// when generating chain by shufling, try N times and pick best variation
const unsigned int kShufflingChainAttempts = 30;
// with some low probability, better chain is rejected (makes generation a bit more random)
const double kBetterChainRejectedCoeff = 0.05;

// images dir
#define kDirImages "files/images.txt"

// probabilities dirs
#define kDirXProbs "files/x_centers_probs.txt"
#define kDirTProbs "files/t_centers_probs.txt"
#define kDirWProbs "files/wings_probs.txt"
#define kDirCProbs "files/corners_probs.txt"
#define kDirEProbs "files/edges_probs.txt"

// speffz dirs
#define kDirXSpeffz "files/x_centers_speffz.txt"
#define kDirTSpeffz "files/t_centers_speffz.txt"
#define kDirWSpeffz "files/wings_speffz.txt"
#define kDirCSpeffz "files/corners_speffz.txt"
#define kDirESpeffz "files/edges_speffz.txt"

// algs dirs
#define kDirXAlgs "files/x_centers_algs.txt"
#define kDirTAlgs "files/t_centers_algs.txt"
#define kDirWAlgs "files/wings_algs.txt"
#define kDirCAlgs "files/corners_algs.txt"
#define kDirEAlgs "files/edges_algs.txt"

// settings file
#define kDirSettings "files/settings.conf"

//font
#define kLabelsFontFamily "Times"
#define kChainInfoLabelFontFamily "Times"
#define kAlgInfoLabelFontFamily "Times"

#endif // GLOBALS_H
