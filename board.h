#include "bitboard.h"
struct History
{
    int move;
    int casInfo;
    int enPas;
    int fiftyMove;
    U64 posKey;

    bool operator<(const History &h);

};

class Board
{
    
public:
    int pieces[120];
    BitBoard pawns;
    
    int sideToMove;
    int enPas;
    int casInfo;
    int fiftyMove;

    U64 posKey;

    int pcCount[13];
    int bigCount[2];
    int majCount[2];
    int minCount[2];
    int material[2];

    BitBoard pcList[13];


    vector<History> hist;
    int ply;

    bool OnBoard(int sq);
    bool NonEmpty(int sq);

    void HashPc(int pc, int sq);
    void HashCas();
    void HashEP();
    void HashSide();

    U64 SetHash();

    void ResetBoard();
    void UpdateCount();

    void LoadPosition(string fen);
    void Print(bool onlyboard);
    bool Verify();

    int IsAttacked(int sq120, int side);
    void PrintMap();

    void AddPiece(int pc, int sq);
    void ClearPiece(int sq);
    void MovePiece(int from, int to);

    void Undo();
    bool MakeMove(int move);
      
};

