#include "bitboard.h"

BitBoard::BitBoard()
{
    info = 0ULL;
    for(int i=0; i<64; i++)
    {
        setMask[i]=0;
        clearMask[i]=0;
    }
    for(int i=0; i<64; i++)
    {
        setMask[i] = (1ULL<<i);
        clearMask[i] = ~setMask[i];
    }
}
void BitBoard::Reset()
{
    info=0;
}
void BitBoard::SetBit(int pos)
{
    info |= setMask[pos];
    return;
}
void BitBoard::ClearBit(int pos)
{
    info &= clearMask[pos];
    return;
}
int BitBoard::PopBit()
{
    U64 b = info ^ (info-1);
    unsigned int fold = (unsigned) ((b & 0xffffffff) ^ (b >> 32));
    info &= (info-1);
    return BitTable[(fold * 0x783a9b23) >> 26];
}
int BitBoard::CountBits()
{
    U64 b = info;
    int r;
    for(r = 0; b; r++, b &= b - 1);
    return r;
}
void BitBoard::Print()
{
    cout << CountBits() << " " << bitset<64>(info) << endl;

    for(int r=7; r>=0; r--)
    {
        for(int f=0; f<8; f++)
        {
            int sq64 = get64fromFR[f][r];
            if((info & (1ULL << sq64))>0)
                cout << "X" << " ";
            else
                cout << "." << " ";
        }
        cout << endl;
    }
    cout << endl;
}