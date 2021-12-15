#include "move.h"
#include "board.h"
#include "lru.h"

#define MAXDEPTH 64


struct SearchData
{
	clock_t startTime;
	clock_t stopTime;
	int depth;
	int depthSet;
	int timeSet;

	int movesToGo;
	int inf;

	U64 nodes;

	int quit;
	int stopped;

	float fh;
	float fhf;
};
class Agent
{

	int cache_size = 1e5;
	vector<Move> moveList;
	Board *b;
	LRU cache;
	vector<int> priVar;
public:
	int searchHistory[13][120];
	int searchKillers[2][MAXDEPTH];

	SearchData sInfo;

	Agent(Board* _b);

	void PrintList();

	void AddQuietMove(int move);
	void AddCaptureMove(int move);
	void AddEnPassantMove(int move);


	void AddWhitePawnMove(int from, int to);
	void AddWhitePawnCapMove(int from, int to, int capt);
	void AddAllWhitePawnMoves();
	void AddAllWhitePawnCaps();

	void AddBlackPawnMove(int from, int to);
	void AddBlackPawnCapMove(int from, int to, int capt);
	void AddAllBlackPawnMoves();
	void AddAllBlackPawnCaps();

	void AddSlidingMoves();
	void AddNonSlidingMoves();
	
	void AddWhiteCastles();
	void AddBlackCastles();

	vector<Move> FindMoves();
	vector<Move> FindCaptures();

	void TotalMoves(int depth, U64 &ans);
	int ParseMove(string s);

	bool Repeated();
	bool IsValidMove(int move);

	int SetPV(int depth);
	void PrintPV();

	int EvalPos();
	void InitSearch();
	void ShouldStop();
	int Quiescence(int alpha, int beta);
	int AlphaBeta(int alpha, int beta, int depth, int doNull);
	void SearchPos();

	
	
};