#include "defs.h"
#include <random>
bool isBig[14]   =     {0,0,1,1,1,1,1,0,1,1,1,1,1,0};
bool isMajor[14] =     {0,0,0,0,1,1,1,0,0,0,1,1,1,0};
bool isMinor[14] =     {0,0,1,1,0,0,0,0,1,1,0,0,0,0};

bool isPawn[14]  =     {0,1,0,0,0,0,0,1,0,0,0,0,0,0};
bool isKnight[14]=     {0,0,1,0,0,0,0,0,1,0,0,0,0,0};
bool isBishopQueen[14]={0,0,0,1,0,1,0,0,0,1,0,1,0,0};
bool isRookQueen[14] = {0,0,0,0,1,1,0,0,0,0,1,1,0,0};
bool isKing[14]      = {0,0,0,0,0,0,1,0,0,0,0,0,1,0};
bool isSliding[14] =   {0,0,0,1,1,1,0,0,0,1,1,1,0,0};

const vector<int> KingDir = { -1, -10,	1, 10, -9, -11, 11, 9 };
const vector<int> KnightDir = { -8, -19,	-21, -12, 8, 19, 21, 12 };
const vector<int> RookDir = {-10 , 10, -1, 1};
const vector<int> BishopDir = {-11, -9, 9, 11};

int pcCol[14] = {BOTH, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BOTH};
int pcVal[14] = {0,100,325,325,550,1000,50000,100,325,325,550,1000,50000,0};

string pcName = ".PNBRQKpnbrqk-";

int get64from120[120];
int get120from64[64];
int get64fromFR[8][8];
int get120fromFR[8][8];
int getFilefrom120[120];
int getRankfrom120[120];
string getNamefrom120[120];

int casPerm[120];
void Init()
{
    for(int i=0; i<120; i++)
    {
        getNamefrom120[i]="..";
        get64from120[i] = OFFBOARD;
        getFilefrom120[i]=getRankfrom120[i]=OFFBOARD;
        casPerm[i]=15;
    }
    for(int f=0; f<8; f++)
        for(int r=0; r<8; r++)
        {
            int sq = 8*r+f;
            get64fromFR[f][r]=sq;
            int sq120 = (2+r)*10 + (1+f);
            get120from64[sq] = sq120;
            get64from120[sq120] = sq;
            get120fromFR[f][r] = sq120;
            string name = "";
            name += char('A'+f);
            name += char('1'+r);
            getNamefrom120[sq120] = name;
            getFilefrom120[sq120] = f;
            getRankfrom120[sq120] = r;
        }
    casPerm[A1] = 13;
    casPerm[H1] = 14;
    casPerm[E1] = 12;
    casPerm[A8] = 7;
    casPerm[H8] = 11;
    casPerm[E8] = 3;
    
}

U64 sideKey;
U64 pieceKeys[13][120];
U64 casKeys[16];
void HashInit()
{
    random_device rd;
    default_random_engine gen(rd());
    gen.seed(69);
    uniform_int_distribution<U64> dist(0,0xFFFFFFFFFFFFFFFF);

    for(int i=0; i<13; i++)
        for(int j=0; j<120; j++)
            pieceKeys[i][j] = dist(gen);
    
    for(int i=0; i<16; i++)
        casKeys[i] = dist(gen);
    
    // for(int i=0; i<100; i++)
    //     fiftyKeys[i] = dist(gen);
    
    sideKey = dist(gen);

}