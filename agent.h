#include "move.h"
#include "board.h"

#define MAXDEPTH 64

struct PV_Entry
{
	U64 posKey;
	int move;

};
struct PV_Table
{
	static const int SIZE = 1e5;
	PV_Entry data[SIZE];

	void Reset();
	void Insert(U64 key, int mv);
	int GetMove(U64 key);
};

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
public:
	vector<Move> moveList;
	Board *b;
	PV_Table table;
	vector<int> priVar;

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

	void AddBlackPawnMove(int from, int to);
	void AddBlackPawnCapMove(int from, int to, int capt);
	void AddAllBlackPawnMoves();

	void AddSlidingMoves();
	void AddNonSlidingMoves();
	
	void AddWhiteCastles();
	void AddBlackCastles();

	vector<Move> FindMoves();

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