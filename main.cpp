#include "defs.h"
#include "agent.h"

#define START "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
#define PAWNMOVESW "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1"
#define PAWNMOVESB "rnbqkbnr/p1p1p3/3p3p/1p1p4/2P1Pp2/8/PP1P1PpP/RNBQKB1R b KQkq e3 0 1"
#define KNIGHTSKINGS "5k2/1n6/4n3/6N1/8/3N4/8/5K2 b - - 0 1"
#define ROOKS "6k1/8/5r2/8/1nR5/5N2/8/6K1 w - - 0 1"
#define QUEENS "6k1/8/4nq2/8/1nQ5/5N2/1N6/6K1 b - - 0 1 "
#define BISHOPS "6k1/1b6/4n3/8/1n4B1/1B3N2/1N6/2b3K1 b - - 0 1 "
#define CASTLE1 "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"
#define CASTLE2 "3rk2r/8/8/8/8/8/6p1/R3K2R w KQk - 0 1"

#define TEST "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"
int main()
{
	Init();
	HashInit();

	string fen = TEST;
	//cin >> fen;
	Board b;
	b.LoadPosition(fen);
	b.Print(false);
	ASSERT((b.Verify()));
	// b.PrintMap();
	Agent a(&b);
	// a.FindMoves();
	

	for(int d=1; d<6; d++)
	{
		b.LoadPosition(fen);
		U64 ans=0;
		a.TotalMoves(d,ans);
		cout << d << ": " << ans << endl;
	}
	// int m = MOVE(A2, A4, EMPTY, EMPTY, MFLAGPS);
	// b.MakeMove(m);
	// b.Verify();
	// b.Print(false);
}