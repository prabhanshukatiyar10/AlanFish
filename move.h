#ifndef MOVE_H
#define MOVE_H
#include "defs.h"
struct Move
{
    int move;
    int score;

    bool operator<(const Move &m);
};

// #define GetFromSQ120(m) ((m) & 0x7F)
// #define GetToSQ120(m) (((m)>>7) & 0x7F)
// #define GetCapture(m) (((m)>>14) & 0xF)
// #define GetPromoted(m) (((m)>>20) & 0xF)

// #define EP_FLAG 0x40000
// #define PS_FLAG 0x80000
// #define CAS_FLAG 0x1000000

// #define CAS_FLAGP 0x7C000
// #define MFLAGPROM 0xF00000

// #define MOVE(f,t,cap,pro,fl) ( (f) | ((t) << 7) | ( (cap) << 14 ) | ( (pro) << 20 ) | (fl))

extern int EP_FLAG;
extern int PS_FLAG;
extern int CAS_FLAG;

extern int GetFromSQ120(int move);
extern int GetToSQ120(int move);
extern int GetCapture(int move);
extern int GetPromoted(int move);

extern int MOVE(int from, int to, int capt, int prom, int flag);

void PrintMove(int move);

#endif