#include "defs.h"
struct Move
{
    int move;
    int score;

    bool operator<(const Move &m);
};

#define FROMSQ(m) ((m) & 0x7F)
#define TOSQ(m) (((m)>>7) & 0x7F)
#define CAPTURED(m) (((m)>>14) & 0xF)
#define PROMOTED(m) (((m)>>20) & 0xF)

#define MFLAGEP 0x40000
#define MFLAGPS 0x80000
#define MFLAGCA 0x1000000

#define MFLAGCAP 0x7C000
#define MFLAGPROM 0xF00000

#define MOVE(f,t,cap,pro,fl) ( (f) | ((t) << 7) | ( (cap) << 14 ) | ( (pro) << 20 ) | (fl))

void PrintMove(int move);