#include "move.h"
#include "board.h"

class Agent
{
public:
	vector<Move> moveList;
	Board *b;
	


	Agent(Board* _b);

	void PrintList();

	void AddQuietMove(int move);
	void AddCaptureMove(int move);
	void AddEnPassantMove(int move);


	void AddWhitePawnMove(int from, int to);
	void AddWhitePawnCapMove(int from, int to, int capt);
	void AddAllWhitePawnMoves();

	void AddBlackPawnMove(int from, int to);
	void AddBlackPawnCapMove(int from, int to, int capt);
	void AddAllBlackPawnMoves();

	void AddSlidingMoves();
	void AddNonSlidingMoves();
	
	void AddWhiteCastles();
	void AddBlackCastles();

	vector<Move> FindMoves();

	void TotalMoves(int depth, U64 &ans);
};