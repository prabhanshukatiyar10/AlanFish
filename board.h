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

struct Board
{
private:
    void HashPc(int pc, int sq);
    void HashCas();
    void HashEP();
    void HashSide();
    void UpdateCount();
    void AddPiece(int pc, int sq);
    void ClearPiece(int sq);
    void MovePiece(int from, int to);

public:
    int pieces[120];
    BitBoard pawns;
    
    int sideToMove;
    int enPas;
    int casInfo;
    int fiftyMove;

    U64 posKey;

    int material[2];

    BitBoard pcList[13];


    vector<History> hist;
    int ply;

    bool OnBoard(int sq);
    bool NonEmpty(int sq);

    

    U64 SetHash();

    void ResetBoard();
    

    void LoadPosition(string fen);
    void Print(bool onlyboard);
    bool Verify();

    int IsAttacked(int sq120, int side);
    void PrintMap();

    void Undo();
    bool MakeMove(int move);
      
};

