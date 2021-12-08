#include "move.h"

bool Move::operator<(const Move &m)
{
    return (move < m.move);
}

void PrintMove(int move)
{
    string from = getNamefrom120[FROMSQ(move)];
    string to = getNamefrom120[TOSQ(move)];
    char capt = pcName[CAPTURED(move)];
    char prom = pcName[PROMOTED(move)];

    bool eps = move & MFLAGEP;
    bool cas = move & MFLAGCA;
    bool pr = move & MFLAGPROM;
    bool ps = move & MFLAGPS;
    bool cp = move & MFLAGCAP;

    cout << "From " << from << " to " << to << " capturing " << capt << " prom to " << prom << " en pas " << eps
            << " prom " << pr << " pawn start " << ps << " castle " << cas << " capt " << cp << endl;
}