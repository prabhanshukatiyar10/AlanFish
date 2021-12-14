// defs.h, bitboard.h, move.h, bitboard.h, board.h, agent.h

#ifndef DEFS_H
#define DEFS_H

// #define DEBUG 

#ifndef DEBUG
#define ASSERT(n)
#else
#define  ASSERT(n) \
if(!(n)) { \
printf("%s - Failed",#n); \
printf("On %s ",__DATE__); \
printf("At %s ",__TIME__); \
printf("In File %s ",__FILE__); \
printf("At Line %d\n",__LINE__); \
exit(1);\
}
#endif

#include <bits/stdc++.h>
#include <random>
#include<time.h>
using namespace std;

typedef unsigned long long U64;
enum { EMPTY, wP, wN, wB, wR, wQ, wK, bP, bN, bB, bR, bQ, bK, OUT  };
enum { FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, FILE_NONE };
enum { RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8, RANK_NONE };
enum { WHITE, BLACK, BOTH };
enum { WKCA = 1, WQCA = 2, BKCA = 4, BQCA = 8 };
enum {
  A1 = 21, B1, C1, D1, E1, F1, G1, H1,
  A2 = 31, B2, C2, D2, E2, F2, G2, H2,
  A3 = 41, B3, C3, D3, E3, F3, G3, H3,
  A4 = 51, B4, C4, D4, E4, F4, G4, H4,
  A5 = 61, B5, C5, D5, E5, F5, G5, H5,
  A6 = 71, B6, C6, D6, E6, F6, G6, H6,
  A7 = 81, B7, C7, D7, E7, F7, G7, H7,
  A8 = 91, B8, C8, D8, E8, F8, G8, H8, NO_SQ=111, OFFBOARD
};

extern bool isBig[14];
extern bool isMajor[14];
extern bool isMinor[14];

extern bool isPawn[14];
extern bool isKnight[14];
extern bool isBishopQueen[14];
extern bool isRookQueen[14];
extern bool isKing[14];
extern bool isSliding[14];

enum {Knight, Bishop, Rook, King};
extern vector<vector<int>> directions;

extern int casPerm[120];

extern int pcCol[14];
extern int pcVal[14];

extern string pcName;

extern int get64from120[120];
extern int get120from64[64];
extern int get64fromFR[8][8];
extern int get120fromFR[8][8];
extern int getFilefrom120[120];
extern int getRankfrom120[120];
extern string getNamefrom120[120];



extern void Init();

extern U64 pieceKeys[13][120];
extern U64 casKeys[16];
extern U64 sideKey;
// extern U64 fiftyKeys[100];

extern void HashInit();

extern vector<vector<int>> sqScore;
extern int MateScore;
extern void InitSqScore();

#endif