#include "move.h"

bool Move::operator<(const Move &m)
{
    return (score > m.score);
}

int EP_FLAG = 0x40000;
int PS_FLAG = 0x80000;
int CAS_FLAG = 0x1000000;

int GetFromSQ120(int move)
{
    return (move & 127);
}
int GetToSQ120(int move)
{
    return ((move>>7) & 127);
}
int GetCapture(int move)
{
    return ((move>>14) & 15);
}
int GetPromoted(int move)
{
    return ((move>>20) & 15);
}

int MOVE(int from, int to, int capt, int prom, int flag)
{
    int ans=0;
    ans |= from;
    ans |= (to<<7);
    ans |= (capt<<14);
    ans |= (prom<<20);
    ans |= flag;
    return ans;
}

void PrintMove(int move)
{
    string from = getNamefrom120[GetFromSQ120(move)];
    string to = getNamefrom120[GetToSQ120(move)];
    char capt = pcName[GetCapture(move)];
    char prom = pcName[GetPromoted(move)];

    bool eps = move & EP_FLAG;
    bool cas = move & CAS_FLAG;
    bool pr = (prom!='.');
    bool ps = move & PS_FLAG;
    bool cp = capt;

    cout << from << to;
    if(prom!='.')
        cout << char(tolower(prom));
    
}