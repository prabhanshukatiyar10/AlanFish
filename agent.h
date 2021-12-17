#include "move.h"
#include "board.h"
#include "lru.h"

#define MAXDEPTH 64


struct SearchData
{
	clock_t startTime;
	int stopTime;
	int depth;
	int depthSet;
	bool timeSet;

	int movesToGo;
	int inf;

	U64 nodes;

	bool quit;
	bool stopped;

	float fh;
	float fhf;
};
#define MAXINPUTSIZE 2048

class Agent
{

	int cache_size = 1e5;
	// vector<Move> moveList;
	Board *b;
	LRU cache;
	// vector<int> priVar;
public:
	int searchHistory[13][120];
	int searchKillers[2][MAXDEPTH];

	SearchData sInfo;

	Agent(Board* _b);

	void PrintList(const vector<Move> &moveList);

	void AddQuietMove(int move, vector<Move> &moveList);
	void AddCaptureMove(int move, vector<Move> &moveList);
	// void AddEnPassantMove(int move);


	// void AddWhitePawnMove(int from, int to);
	// void AddWhitePawnCapMove(int from, int to, int capt);
	// void AddAllWhitePawnMoves();
	// void AddAllWhitePawnCaps();

	// void AddBlackPawnMove(int from, int to);
	// void AddBlackPawnCapMove(int from, int to, int capt);
	// void AddAllBlackPawnMoves();
	// void AddAllBlackPawnCaps();

	void AddPawnMoves(vector<Move> &moveList ,bool OnlyCaps);

	void AddSlidingMoves(vector<Move> &moveList ,bool OnlyCaps);
	void AddNonSlidingMoves(vector<Move> &moveList ,bool OnlyCaps);
	
	// void AddWhiteCastles();
	// void AddBlackCastles();
	void AddCastles(vector<Move> &moveList);

	void FindMoves(vector<Move> &moveList,bool OnlyCaps);
	// vector<Move> FindCaptures();

	void TotalMoves(int depth, U64 &ans);
	int ParseMove(string s);

	bool Repeated();
	bool IsValidMove(int move);

	vector<int> SetPV(int depth);
	void PrintPV(const vector<int> &priVar);

	int EvalPos();
	void InitSearch();
	void ShouldStop();
	int Quiescence(int alpha, int beta);
	int AlphaBeta(int alpha, int beta, int depth, int doNull);
	void SearchPos();

	void UCIloop();
	void ParseGO(string inp);
	void ParsePos(string inp);
	
};