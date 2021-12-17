#include "defs.h"
#include "agent.h"


#define PAWNMOVESW "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1"
#define PAWNMOVESB "rnbqkbnr/p1p1p3/3p3p/1p1p4/2P1Pp2/8/PP1P1PpP/RNBQKB1R b KQkq e3 0 1"
#define KNIGHTSKINGS "5k2/1n6/4n3/6N1/8/3N4/8/5K2 b - - 0 1"
#define ROOKS "6k1/8/5r2/8/1nR5/5N2/8/6K1 w - - 0 1"
#define QUEENS "6k1/8/4nq2/8/1nQ5/5N2/1N6/6K1 b - - 0 1 "
#define BISHOPS "6k1/1b6/4n3/8/1n4B1/1B3N2/1N6/2b3K1 b - - 0 1 "
#define CASTLE1 "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"
#define CASTLE2 "3rk2r/8/8/8/8/8/6p1/R3K2R w KQk - 0 1"

#define m3_1 "2rr3k/pp3pp1/1nnqbN1p/3pN3/2pP4/2P3Q1/PPB4P/R4RK1 w - -"
#define m5_1 "r1b1kb1r/pppp1ppp/5q2/4n3/3KP3/2N3PN/PPP4P/R1BQ1B1R b kq - 0 1"
#define m7_1 "r5rk/2p1Nppp/3p3P/pp2p1P1/4P3/2qnPQK1/8/R6R w - - 1 0"

#define TEST "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"
#define MOTEST "r1b1k2r/ppppnppp/2n2q2/2b5/3NP3/2P1B3/PP3PPP/RN1QKB1R w KQkq - 0 1"
int main()
{
	Init();
	HashInit();
	InitSqScore();

	Board b;
	Agent a(&b);
	a.UCIloop();
	
}

// Board b;
	// Agent a(&b);
	// b.LoadPosition(m7_1);
	// while(true)
	// {
	// 	b.Print(true);
	// 	string s;
	// 	cin >> s;
	// 	if(s[0]=='q')
	// 		break;
	// 	if(s[0]=='t')
	// 		b.Undo();
	// 	else if(s[0]=='s')
	// 	{
	// 		a.sInfo.depth = 8;
	// 		a.SearchPos();
	// 	}

		
	// }





	// // string fen = "START";
	// // //cin >> fen;
	// // Board b;
	// // b.LoadPosition(TEST);
	// // b.Print(false);
	// // ASSERT((b.Verify()));
	// // // b.PrintMap();
	// // Agent a(&b);

	// // auto v = a.FindMoves(false);
	// // int i=1;
	// // for(auto m:v)
	// // {
	// // 	cout << i++ << ": ";
	// // 	PrintMove(m.move);
	// // }
	

	// // for(int d=4; d<5; d++)
	// // {
	// // 	//b.LoadPosition(fen);
	// // 	U64 ans=0;
	// // 	a.TotalMoves(d,ans);
	// // 	cout << d << ": " << ans << endl;
	// // }
	// // int m = MOVE(A2, A4, EMPTY, EMPTY, PS_FLAG);
	// // b.MakeMove(m);
	// // b.Verify();
	// // b.Print(false);
	// // for(string fen:fenlist)
	// // {
	// // 	Board b;
	// // 	Agent a(&b);
	// // 	b.LoadPosition(fen);
	// // 	ASSERT(b.Verify());

	// // Board b;
	// // Agent a(&b);
	// // ifstream file("fen.txt");
	// // string line;
	// // vector<string> fenlist;
	// // while(getline(file, line))
	// // 	fenlist.push_back(line);
	// // int i=0;
	// // for(string fen:fenlist)
	// // {

	// // 	b.LoadPosition(fen);
	// // 	for(int d=1; d<6; d++)
	// // 	{
	// // 		// cout << "running" << d << endl;
	// // 		U64 ans=0;
	// // 		a.TotalMoves(d,ans);
	// // 		cout << ans << " " << flush;
	// // 	}
	// // 	cout << endl;
	// // }