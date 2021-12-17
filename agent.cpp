#include "agent.h"

Agent::Agent(Board* _b)
{
	b = _b;
	cache = LRU(cache_size);
}

void Agent::PrintList(const vector<Move> &moveList)
{
	int i=1;
	for(auto m:moveList)
	{
		cout << i++ << ": ";
		PrintMove(m.move);
	}
}

// General Functions
void Agent::AddQuietMove(int move, vector<Move> &moveList)
{
	Move m;
	m.move = move;
	m.score = 0;
	moveList.push_back(m);
}
void Agent::AddCaptureMove(int move, vector<Move> &moveList)
{
	Move m;
	m.move = move;
	m.score = pcVal[b->pieces[GetToSQ120(move)]] - pcVal[b->pieces[GetFromSQ120(move)]]/10;
	moveList.push_back(m);
}
// void Agent::AddEnPassantMove(int move)
// {
// 	Move m;
// 	m.move = move;
// 	m.score = 90;
// 	moveList.push_back(m);
// }

void Agent::AddPawnMoves(vector<Move> &moveList, bool OnlyCaps)
{
	int pc = wP;
	int left = +9;
	int right = +11;
	int fwd = 10;

	int promRank = RANK_7;
	int startRank = RANK_2;

	vector<int> promPieces = {wN, wB, wR, wQ};

	if(b->sideToMove==BLACK)
	{
		pc = bP;
		left = -9;
		right = -11;
		fwd = -10;
		swap(promRank, startRank);
		promPieces = {bN, bB, bR, bQ};
	}
	U64 t_info = b->pcList[pc].info;
	while(b->pcList[pc].info)
	{
		int sq64 = b->pcList[pc].PopBit();
		int sq = get120from64[sq64];
		vector<int> tars = {sq+left, sq+right};
		
		for(int tar:tars)
		{
			if(b->pieces[tar]==OUT)
				continue;
			if(b->pieces[tar]!=EMPTY && pcCol[b->pieces[tar]]!=b->sideToMove)
			{
				//cout << "capture detected at " << getNamefrom120[tar] << endl;
				if(getRankfrom120[sq]==promRank)
				{
					for(int ppc:promPieces)
						AddCaptureMove(MOVE(sq, tar, b->pieces[tar], ppc, 0), moveList);
				}
				else
					AddCaptureMove(MOVE(sq, tar, b->pieces[tar], EMPTY, 0), moveList);
			}
			if(tar==b->enPas)
				AddCaptureMove(MOVE(sq, tar, (bP - b->sideToMove*6), EMPTY, EP_FLAG), moveList);
		}

		if(OnlyCaps)
			continue;

		int tar = sq+fwd;
		if(b->pieces[tar]==EMPTY)
		{
			if(getRankfrom120[sq]==promRank)
			{
				for(int ppc:promPieces)
					AddQuietMove(MOVE(sq, tar, EMPTY, ppc, 0), moveList);
			}
			else
				AddQuietMove(MOVE(sq, tar, EMPTY, EMPTY, 0), moveList);
			
			if(getRankfrom120[sq]==startRank && b->pieces[tar+fwd]==EMPTY)
				AddQuietMove(MOVE(sq, tar+fwd, EMPTY, EMPTY, PS_FLAG), moveList);
		}
	}
	b->pcList[pc].info = t_info;
	
}

/*----------------------------------------------------WHITE PAWN--------------------------------------------*/
// void Agent::AddWhitePawnMove(int from, int to)
// {
// 	if(getRankfrom120[from]==RANK_7)
// 	{
// 		AddQuietMove(MOVE(from, to, EMPTY, wN, 0));
// 		AddQuietMove(MOVE(from, to, EMPTY, wB, 0));
// 		AddQuietMove(MOVE(from, to, EMPTY, wR, 0));
// 		AddQuietMove(MOVE(from, to, EMPTY, wQ, 0));
// 	}
// 	else if(to-from!=20)
// 		AddQuietMove(MOVE(from, to, EMPTY, EMPTY, 0));
// 	else
// 		AddQuietMove(MOVE(from, to, EMPTY, EMPTY, PS_FLAG));
// }
// void Agent::AddWhitePawnCapMove(int from, int to, int cap)
// {
// 	if(getRankfrom120[from]==RANK_7)
// 	{
// 		AddCaptureMove(MOVE(from, to, cap, wN, 0));
// 		AddCaptureMove(MOVE(from, to, cap, wB, 0));
// 		AddCaptureMove(MOVE(from, to, cap, wR, 0));
// 		AddCaptureMove(MOVE(from, to, cap, wQ, 0));
// 	}
// 	else
// 		AddCaptureMove(MOVE(from, to, cap, EMPTY, 0));
// }
// void Agent::AddAllWhitePawnMoves()
// {
// 	U64 t_info = b->pcList[wP].info;
// 	while(b->pcList[wP].info)
// 	{
// 		int sq120 = get120from64[b->pcList[wP].PopBit()];
// 		if(b->pieces[sq120+10]==EMPTY)
// 			AddWhitePawnMove(sq120, sq120+10);
// 		if(pcCol[b->pieces[sq120+9]]==BLACK)
// 			AddWhitePawnCapMove(sq120, sq120+9, b->pieces[sq120+9]);
// 		if(pcCol[b->pieces[sq120+11]]==BLACK)
// 			AddWhitePawnCapMove(sq120, sq120+11, b->pieces[sq120+11]);

// 		if(getRankfrom120[sq120]==RANK_2)
// 			if(b->pieces[sq120+10]==EMPTY && b->pieces[sq120+20]==EMPTY)
// 				AddWhitePawnMove(sq120, sq120+20);

// 		if((sq120+9)==b->enPas || (sq120+11)==b->enPas)
// 			AddEnPassantMove(MOVE(sq120, b->enPas, bP, EMPTY, EP_FLAG));
// 		}
// 	b->pcList[wP].info = t_info;

// 	// for(int sq120:b->pieceList[wP])
// 	// {
		
// 	// }
// }

// void Agent::AddAllWhitePawnCaps()
// {
// 	U64 t_info = b->pcList[wP].info;
// 	while(b->pcList[wP].info)
// 	{
// 		int sq120 = get120from64[b->pcList[wP].PopBit()];

// 		if(pcCol[b->pieces[sq120+9]]==BLACK)
// 			AddWhitePawnCapMove(sq120, sq120+9, b->pieces[sq120+9]);
// 		if(pcCol[b->pieces[sq120+11]]==BLACK)
// 			AddWhitePawnCapMove(sq120, sq120+11, b->pieces[sq120+11]);

// 		if((sq120+9)==b->enPas || (sq120+11)==b->enPas)
// 			AddEnPassantMove(MOVE(sq120, b->enPas, bP, EMPTY, EP_FLAG));
// 		}
// 	b->pcList[wP].info = t_info;
// }

/*----------------------------------------------------BLACK PAWN--------------------------------------------*/
// void Agent::AddBlackPawnMove(int from, int to)
// {
// 	if(getRankfrom120[from]==RANK_2)
// 	{
// 		AddQuietMove(MOVE(from, to, EMPTY, bN, 0));
// 		AddQuietMove(MOVE(from, to, EMPTY, bB, 0));
// 		AddQuietMove(MOVE(from, to, EMPTY, bR, 0));
// 		AddQuietMove(MOVE(from, to, EMPTY, bQ, 0));
// 	}
// 	else if(to-from!=-20)
// 		AddQuietMove(MOVE(from, to, EMPTY, EMPTY, 0));
// 	else
// 		AddQuietMove(MOVE(from, to, EMPTY, EMPTY, PS_FLAG));
// }
// void Agent::AddBlackPawnCapMove(int from, int to, int cap)
// {
// 	if(getRankfrom120[from]==RANK_2)
// 	{
// 		AddCaptureMove(MOVE(from, to, cap, bN, 0));
// 		AddCaptureMove(MOVE(from, to, cap, bB, 0));
// 		AddCaptureMove(MOVE(from, to, cap, bR, 0));
// 		AddCaptureMove(MOVE(from, to, cap, bQ, 0));
// 	}
// 	else
// 		AddCaptureMove(MOVE(from, to, cap, EMPTY, 0));
// }
// void Agent::AddAllBlackPawnMoves()
// {
// 	U64 t_info = b->pcList[bP].info;
// 	while(b->pcList[bP].info)
// 	{
// 		int sq120 = get120from64[b->pcList[bP].PopBit()];
// 		if(b->pieces[sq120-10]==EMPTY)
// 			AddBlackPawnMove(sq120, sq120-10);

// 		if(pcCol[b->pieces[sq120-9]]==WHITE)
// 			AddBlackPawnCapMove(sq120, sq120-9, b->pieces[sq120-9]);
// 		if(pcCol[b->pieces[sq120-11]]==WHITE)
// 			AddBlackPawnCapMove(sq120, sq120-11, b->pieces[sq120-11]);

// 		if(getRankfrom120[sq120]==RANK_7)
// 			if(b->pieces[sq120-10]==EMPTY && b->pieces[sq120-20]==EMPTY)
// 				AddBlackPawnMove(sq120, sq120-20);

// 		if((sq120-9)==b->enPas || (sq120-11)==b->enPas)
// 			AddEnPassantMove(MOVE(sq120, b->enPas, wP, EMPTY, EP_FLAG));
// 		}
// 	b->pcList[bP].info = t_info;
// 	// for(int sq120:b->pieceList[bP])
// 	// {
		
// 	// }
// }



// void Agent::AddAllBlackPawnCaps()
// {
// 	U64 t_info = b->pcList[bP].info;
// 	while(b->pcList[bP].info)
// 	{
// 		int sq120 = get120from64[b->pcList[bP].PopBit()];


// 		if(pcCol[b->pieces[sq120-9]]==WHITE)
// 			AddBlackPawnCapMove(sq120, sq120-9, b->pieces[sq120-9]);
// 		if(pcCol[b->pieces[sq120-11]]==WHITE)
// 			AddBlackPawnCapMove(sq120, sq120-11, b->pieces[sq120-11]);


// 		if((sq120-9)==b->enPas || (sq120-11)==b->enPas)
// 			AddEnPassantMove(MOVE(sq120, b->enPas, wP, EMPTY, EP_FLAG));
// 		}
// 	b->pcList[bP].info = t_info;
// }


/*----------------------------------------------------SLIDING--------------------------------------------*/
/*----------------------------------------------------SLIDING--------------------------------------------*/
/*----------------------------------------------------SLIDING--------------------------------------------*/
/*----------------------------------------------------SLIDING--------------------------------------------*/


void Agent::AddSlidingMoves(vector<Move> &moveList, bool OnlyCaps)
{
	vector<int> slp = {wB, wR, wQ};
	for(int i=0; i<3; i++)
		slp[i] += 6*b->sideToMove;

	for(int pc:slp)
	{
		vector<int> dirList;
		if(isBishopQueen[pc])
			dirList.push_back(Bishop);
		if(isRookQueen[pc])
			dirList.push_back(Rook);
		U64 t_info = b->pcList[pc].info;
		while(b->pcList[pc].info)
		{
			int sq = get120from64[b->pcList[pc].PopBit()];
			for(int dL:dirList)
			{
				for(int dir:directions[dL])
				{
					int cursq = sq;
					while(b->pieces[cursq+dir]!=OUT)
					{
						cursq+=dir;
						if(b->pieces[cursq]==EMPTY)
							{if(!OnlyCaps)
								AddQuietMove(MOVE(sq,cursq,EMPTY,EMPTY, 0), moveList);}
						else if(pcCol[b->pieces[cursq]]==b->sideToMove)
							break;
						else
						{
							AddCaptureMove(MOVE(sq, cursq, b->pieces[cursq], EMPTY, 0), moveList);
							break;
						}
					}
				}
			}
		}
		b->pcList[pc].info = t_info;
	}
}

/*--------------------------------------------------NON-SLIDING------------------------------------------*/
/*--------------------------------------------------NON-SLIDING------------------------------------------*/
/*--------------------------------------------------NON-SLIDING------------------------------------------*/
/*--------------------------------------------------NON-SLIDING------------------------------------------*/

void Agent::AddNonSlidingMoves(vector<Move> &moveList, bool OnlyCaps)
{
	vector<int> nsp = {wN, wK};
	for(int i=0; i<2; i++)
		nsp[i]+= b->sideToMove*6;

	for(int pc:nsp)
	{
		int dir;
		if(isKnight[pc])
			dir = Knight;
		else
			dir = King;

		U64 t_info = b->pcList[pc].info;
		while(b->pcList[pc].info)
		{
			int sq = get120from64[b->pcList[pc].PopBit()];
			for(int d:directions[dir])
			{
				int curpc = b->pieces[sq+d];
				if(curpc==OUT)
					continue;
				if(curpc==EMPTY)
					{if(!OnlyCaps)
						AddQuietMove(MOVE(sq, sq+d,EMPTY,EMPTY,0), moveList);}
				else if(pcCol[curpc]==b->sideToMove)
					continue;
				else
					AddCaptureMove(MOVE(sq,sq+d,b->pieces[sq+d],EMPTY,0), moveList);

			}
		}
		b->pcList[pc].info = t_info;
	}
}


/*----------------------------------------------------CASTLES--------------------------------------------*/
/*----------------------------------------------------CASTLES--------------------------------------------*/
/*----------------------------------------------------CASTLES--------------------------------------------*/
/*----------------------------------------------------CASTLES--------------------------------------------*/

// void Agent::AddWhiteCastles()
// {
// 	if((b->casInfo & WKCA) && b->pieces[E1]==wK && b->pieces[H1]==wR && b->pieces[F1]==EMPTY && b->pieces[G1]==EMPTY)
// 	{
// 		if( !( b->IsAttacked(E1, BLACK) || b->IsAttacked(F1, BLACK) || b->IsAttacked(G1, BLACK) ) )
// 		{
// 			Move m;
// 			m.move = MOVE(E1, G1, EMPTY, EMPTY, CAS_FLAG);
// 			m.score = 0;
// 			moveList.push_back(m);
// 		}
// 	}
// 	if((b->casInfo & WQCA) && b->pieces[E1]==wK && b->pieces[A1]==wR && b->pieces[B1]==EMPTY && b->pieces[C1]==EMPTY && b->pieces[D1]==EMPTY)
// 	{
// 		if( !( b->IsAttacked(E1, BLACK) || b->IsAttacked(D1, BLACK) || b->IsAttacked(C1, BLACK) ) )
// 		{
// 			Move m;
// 			m.move = MOVE(E1, C1, EMPTY, EMPTY, CAS_FLAG);
// 			m.score = 0;
// 			moveList.push_back(m);
// 		}
// 	}
// }
// void Agent::AddBlackCastles()
// {
// 	if((b->casInfo & BKCA) && b->pieces[E8]==bK && b->pieces[H8]==bR && b->pieces[F8]==EMPTY && b->pieces[G8]==EMPTY)
// 	{
// 		if( !( b->IsAttacked(E8, WHITE) || b->IsAttacked(F8, WHITE) || b->IsAttacked(G8, WHITE) ) )
// 		{
// 			Move m;
// 			m.move = MOVE(E8, G8, EMPTY, EMPTY, CAS_FLAG);
// 			m.score = 0;
// 			moveList.push_back(m);
// 		}
// 	}
// 	if((b->casInfo & BQCA) && b->pieces[E8]==bK && b->pieces[A8]==bR && b->pieces[B8]==EMPTY && b->pieces[C8]==EMPTY && b->pieces[D8]==EMPTY)
// 	{
// 		if( !( b->IsAttacked(E8, WHITE) || b->IsAttacked(D8, WHITE) || b->IsAttacked(C8, WHITE) ) )
// 		{
// 			Move m;
// 			m.move = MOVE(E8, C8, EMPTY, EMPTY, CAS_FLAG);
// 			m.score = 0;
// 			moveList.push_back(m);
// 		}
// 	}
// }

void Agent::AddCastles(vector<Move> &moveList)
{
	if(b->sideToMove==BLACK)
	{
		if((b->casInfo & BKCA) && b->pieces[E8]==bK && b->pieces[H8]==bR && b->pieces[F8]==EMPTY && b->pieces[G8]==EMPTY)
		{
			if( !( b->IsAttacked(E8, WHITE) || b->IsAttacked(F8, WHITE) || b->IsAttacked(G8, WHITE) ) )
			{
				Move m;
				m.move = MOVE(E8, G8, EMPTY, EMPTY, CAS_FLAG);
				m.score = 0;
				moveList.push_back(m);
			}
		}
		if((b->casInfo & BQCA) && b->pieces[E8]==bK && b->pieces[A8]==bR && b->pieces[B8]==EMPTY && b->pieces[C8]==EMPTY && b->pieces[D8]==EMPTY)
		{
			if( !( b->IsAttacked(E8, WHITE) || b->IsAttacked(D8, WHITE) || b->IsAttacked(C8, WHITE) ) )
			{
				Move m;
				m.move = MOVE(E8, C8, EMPTY, EMPTY, CAS_FLAG);
				m.score = 0;
				moveList.push_back(m);
			}
		}
	}
	else
	{
		if((b->casInfo & WKCA) && b->pieces[E1]==wK && b->pieces[H1]==wR && b->pieces[F1]==EMPTY && b->pieces[G1]==EMPTY)
		{
			if( !( b->IsAttacked(E1, BLACK) || b->IsAttacked(F1, BLACK) || b->IsAttacked(G1, BLACK) ) )
			{
				Move m;
				m.move = MOVE(E1, G1, EMPTY, EMPTY, CAS_FLAG);
				m.score = 0;
				moveList.push_back(m);
			}
		}
		if((b->casInfo & WQCA) && b->pieces[E1]==wK && b->pieces[A1]==wR && b->pieces[B1]==EMPTY && b->pieces[C1]==EMPTY && b->pieces[D1]==EMPTY)
		{
			if( !( b->IsAttacked(E1, BLACK) || b->IsAttacked(D1, BLACK) || b->IsAttacked(C1, BLACK) ) )
			{
				Move m;
				m.move = MOVE(E1, C1, EMPTY, EMPTY, CAS_FLAG);
				m.score = 0;
				moveList.push_back(m);
			}
		}
	}
}

void Agent::FindMoves(vector<Move> &moveList, bool OnlyCaps)
{

	moveList = vector<Move>(0);
	// if(b->sideToMove==WHITE)
	// {
	// 	AddAllWhitePawnMoves();
	// 	AddWhiteCastles();
	// }
	// else
	// {
	// 	AddAllBlackPawnMoves();
	// 	AddBlackCastles();
	// }
	AddPawnMoves(moveList, OnlyCaps);
	AddSlidingMoves(moveList, OnlyCaps);
	AddNonSlidingMoves(moveList, OnlyCaps);
	AddCastles(moveList);
	sort(moveList.begin(), moveList.end());
}

// vector<Move> Agent::FindCaptures()
// {
// 	moveList = vector<Move>(0);
// 	if(b->sideToMove==WHITE)
// 		AddAllWhitePawnCaps();
// 	else
// 		AddAllBlackPawnCaps();

// 	vector<int> slp = {wB, wR, wQ};
// 	for(int i=0; i<3; i++)
// 		slp[i] += 6*b->sideToMove;

// 	for(int pc:slp)
// 	{
// 		vector<int> dirList;
// 		if(isBishopQueen[pc])
// 			dirList.push_back(Bishop);
// 		if(isRookQueen[pc])
// 			dirList.push_back(Rook);
// 		U64 t_info = b->pcList[pc].info;
// 		while(b->pcList[pc].info)
// 		{
// 			int sq = get120from64[b->pcList[pc].PopBit()];
// 			for(int dL:dirList)
// 			{
// 				for(int dir:directions[dL])
// 				{
// 					int cursq = sq;
// 					while(b->pieces[cursq+dir]!=OUT)
// 					{
// 						cursq+=dir;
// 						if(b->pieces[cursq]==EMPTY)
// 							{}
// 						else if(pcCol[b->pieces[cursq]]==b->sideToMove)
// 							break;
// 						else
// 						{
// 							AddCaptureMove(MOVE(sq, cursq, b->pieces[cursq], EMPTY, 0));
// 							break;
// 						}
// 					}
// 				}
// 			}
// 		}
// 		b->pcList[pc].info = t_info;
// 	}

// 	vector<int> nsp = {wN, wK};
// 	for(int i=0; i<2; i++)
// 		nsp[i]+= b->sideToMove*6;

// 	for(int pc:nsp)
// 	{
// 		int dir;
// 		if(isKnight[pc])
// 			dir = Knight;
// 		else
// 			dir = King;

// 		U64 t_info = b->pcList[pc].info;
// 		while(b->pcList[pc].info)
// 		{
// 			int sq = get120from64[b->pcList[pc].PopBit()];
// 			for(int d:directions[dir])
// 			{
// 				int curpc = b->pieces[sq+d];
// 				if(curpc==OUT)
// 					continue;
// 				if(curpc==EMPTY)
// 					{}
// 				else if(pcCol[curpc]==b->sideToMove)
// 					continue;
// 				else
// 					AddCaptureMove(MOVE(sq,sq+d,b->pieces[sq+d],EMPTY,0));

// 			}
// 		}
// 		b->pcList[pc].info = t_info;
// 	}
// 	return moveList;
	
// }

void Agent::TotalMoves(int depth, U64 &ans)
{
	if(depth==0)
	{
		ans++;
		return;
	}
	vector<Move> mL;
	FindMoves(mL, false);
	for(Move &m:mL)
	{
		int move = m.move;
		//PrintMove(move);
		int ret = b->MakeMove(move);
		if(ret)
		{
			
			//b->Print(false);
			TotalMoves(depth-1,ans);
			//cout << "Undoing now" << endl;
			b->Undo();
			//b->Print(false);

		}
		// else
		// 	cout << "move bad" << endl;
	}
	
}

int Agent::ParseMove(string s)
{
	ASSERT(s.length()>=4);
	int fr, ff, tr, tf;
	ff = s[0]-'a';
	fr = s[1]-'1';
	tf = s[2] - 'a';
	tr = s[3]-'1';

	int from = get120fromFR[ff][fr];
	int to = get120fromFR[tf][tr];
	char prom = 0;
	if(s.length()==5)
		prom = s[4];
	vector<Move> moveList;
	FindMoves(moveList, false);
	for(Move m:moveList)
	{
		int move = m.move;
		if(GetFromSQ120(move)==from && GetToSQ120(move)==to)
		{
			if(prom==0)
				return move;
			if(tolower(pcName[GetPromoted(move)])==prom)
				return move;
		}
	}
	return 0;
}

/*---------------------------------------------------RL-BEGINS-------------------------------------------*/
/*---------------------------------------------------RL-BEGINS-------------------------------------------*/
/*---------------------------------------------------RL-BEGINS-------------------------------------------*/
/*---------------------------------------------------RL-BEGINS-------------------------------------------*/

bool Agent::Repeated()
{
	for(int i=b->hist.size()-b->fiftyMove; i<b->hist.size(); i++)
	{
		if(b->posKey==b->hist[i].posKey)
			return true;
	}
	return false;
}

bool Agent::IsValidMove(int move)
{
	vector<Move> moveList;
	FindMoves(moveList, false);
	for(Move m:moveList)
	{
		if(m.move==move)
		{
			int ret = b->MakeMove(move);
			if(ret)
			{
				b->Undo();
				return true;
			}
		}
	}
	return false;
}

vector<int> Agent::SetPV(int depth)
{
	vector<int> priVar(0);
	int mv = cache.get(b->posKey);
	
	while(mv && priVar.size()<depth)
	{
		
		if(!IsValidMove(mv))
			break;
		priVar.push_back(mv);
		
		b->MakeMove(mv);

		mv = cache.get(b->posKey);
		
	}

	while(b->ply)
		b->Undo();
	// cout << "got size of " << priVar.size() << endl;
	return priVar;
}

void Agent::PrintPV(const vector<int> &priVar)
{
	// cout << "@@@@ " << table.data.size() << endl;
	for(int mv:priVar)
	{
		PrintMove(mv);
		cout << ' ';
	}
}

int Agent::EvalPos()
{
	int score = b->material[WHITE]-b->material[BLACK];
	for(int i=0; i<64; i++)
	{
		int sq120 = get120from64[i];
		int pc = b->pieces[sq120];
		//ASSERT(pc>=EMPTY && pc <=bK);
		if(pc==EMPTY)
			continue;
		if(pcCol[pc]==WHITE)
			score += sqScore[pc][i];
		else
			score -= sqScore[pc][i];
	}
	if(b->sideToMove==WHITE)
		return score;
	return -score;
}

void Agent::InitSearch()
{
	for(int i=0; i<13; i++)
		for(int j=0; j<120; j++)
			searchHistory[i][j]=0;
	
	for(int i=0; i<2; i++)
		for(int j=0; j<MAXDEPTH; j++)
			searchKillers[i][j]=0;
	
	cache = LRU(cache_size);
	b->ply=0;



	sInfo.stopped=0;
	sInfo.nodes = 0;


	sInfo.fh = 0;
	sInfo.fhf = 0;

}

void Agent::SearchPos()
{
	int bestMove = 0;
	int bestScore = INT_MIN+1;
	InitSearch();
	for(int cd =1; cd<=sInfo.depth; cd++)
	{
		if(bestScore<MateScore-cd)
			bestScore = AlphaBeta(INT_MIN+1, INT_MAX, cd, 1);
		
		if(sInfo.stopped)
			break;
		
		auto priVar = SetPV(cd);
		bestMove = priVar[0];

		// cout << "Nodes: " << sInfo.nodes <<  " Depth: " << cd << " Score: " << bestScore << " Move: ";
		// PrintMove(bestMove);
		cout << "info score cp " << bestScore << " depth " << cd << " nodes " << sInfo.nodes
				<< " time " << float((clock()-sInfo.startTime)*1000/CLOCKS_PER_SEC) << " pv ";
		PrintPV(priVar);
		cout << endl;
		//cout << "Ordering: " << sInfo.fhf/sInfo.fh << endl;
	}
	cout << "bestmove "; PrintMove(bestMove);
	cout << endl;
}

int Agent::AlphaBeta(int alpha, int beta, int depth, int doNull)
{
	ASSERT(b->Verify());
	if(depth==0)
	{
		return Quiescence(alpha, beta);
	}
	if((sInfo.nodes & 4096) == 0)
		ShouldStop();

	if(Repeated()||b->fiftyMove>=100)
		return 0;
		
	if(b->ply>=MAXDEPTH)
		return EvalPos();
	
	vector<Move> mL;
	FindMoves(mL, false);
	int legal=0;
	int alpha0 = alpha;
	int score = INT_MIN+1;
	int bestmove = 0;

	for(Move m:mL)
	{
		int mv = m.move;
		if(!b->MakeMove(mv))
			continue;
		legal++;
		score = -AlphaBeta(-beta, -alpha, depth-1, doNull);
		b->Undo();

		if(sInfo.stopped)
			return 0;

		if(score>alpha)
		{
			if(score>=beta)
			{
				if(legal==1)
					sInfo.fhf++;
				sInfo.fh++;
				return beta;
			}
			// cout << "$$$$ ";
			// PrintMove(mv);
			alpha=score;
			bestmove = mv;
		}
	}
	if(legal==0)
	{
		int kingsq = b->pcList[(b->sideToMove)*6 + wK].PopBit();
		b->pcList[(b->sideToMove)*6 + wK].SetBit(kingsq);
		kingsq = get120from64[kingsq];

		if(b->IsAttacked(kingsq, 1^b->sideToMove))
			return -MateScore + b->ply;
		return 0;
	}

	if(alpha!=alpha0)
	{
		cache.put(b->posKey, bestmove);
	}
		

	return alpha;
}

int Agent::Quiescence(int alpha, int beta)
{
	ASSERT(b->Verify());

	if((sInfo.nodes & 4096) == 0)
		ShouldStop();

	sInfo.nodes++;
	if(Repeated()||b->fiftyMove>=100)
		return 0;
	
	if(b->ply>=MAXDEPTH)
		return EvalPos();

	int score = EvalPos();

	if(score>=beta)
		return beta;
	if(score>alpha)
		alpha = score;

	


	vector<Move> mL;
	FindMoves(mL, true);
	int legal=0;
	int alpha0 = alpha;
	int bestmove = 0;
	score = INT_MIN+1;

	for(Move m:mL)
	{
		int mv = m.move;
		if(!b->MakeMove(mv))
			continue;
		legal++;
		score = -Quiescence(-beta, -alpha);
		b->Undo();

		if(sInfo.stopped)
			return 0;

		if(score>alpha)
		{
			if(score>=beta)
			{
				if(legal==1)
					sInfo.fhf++;
				sInfo.fh++;
				return beta;
			}
			// cout << "$$$$ ";
			// PrintMove(mv);
			alpha=score;
			bestmove = mv;
		}
	}


	if(alpha!=alpha0)
	{
		cache.put(b->posKey, bestmove);
	}
		

	return alpha;

}

void Agent::ShouldStop()
{
	auto curtime = clock();

	double elapsed = int(curtime - sInfo.startTime);
	elapsed/=(CLOCKS_PER_SEC/1000);
	if(sInfo.timeSet && elapsed>sInfo.stopTime)
		sInfo.stopped = true;
	if(InputWaiting())
	{
		string s;
		cin >> s;
		sInfo.stopped = true;
		if(s=="quit")
			sInfo.quit = true;
	}

}

void Agent::ParseGO(string inp)
{
	int depth = MAXDEPTH;
	int movestogo = 30;
	int movetime = -1;
	int time = -1;
	int inc = 0;

	sInfo.timeSet = false;
	if(b->sideToMove==WHITE)
	{
		auto ip1 = inp.find("winc");
		auto ip2 = inp.find("wtime");
		if(ip1!=string::npos)
			inc = stoi(inp.substr(ip1+5, MAXINPUTSIZE));
		if(ip2!=string::npos)
			time = stoi(inp.substr(ip2+6, MAXINPUTSIZE));
	}
	else
	{
		auto ip1 = inp.find("binc");
		auto ip2 = inp.find("btime");
		if(ip1!=string::npos)
			inc = stoi(inp.substr(ip1+5, MAXINPUTSIZE));
		if(ip2!=string::npos)
			time = stoi(inp.substr(ip2+6, MAXINPUTSIZE));
	}
	auto ip = inp.find("movestogo");
	if(ip!=string::npos)
		movestogo = stoi(inp.substr(ip+10, MAXINPUTSIZE));
	ip = inp.find("movetime");
	if(ip!=string::npos)
		movetime = stoi(inp.substr(ip+9, MAXINPUTSIZE));

	ip = inp.find("depth");
	if(ip!=string::npos)
		depth = min(MAXDEPTH, stoi(inp.substr(ip+6, MAXINPUTSIZE)));

	if(movetime!=-1)
	{
		time = movetime;
		movestogo = 1;
	}
	sInfo.startTime = clock();
	sInfo.depth = depth;

	if(time != -1)
	{
		sInfo.timeSet = true;
		
		time /= movestogo;
		time -= 50;
		
		sInfo.stopTime = time + inc;
	}

	cout << "time: " << time << " stoptime: " << sInfo.stopTime << " depth: " << sInfo.depth << endl;
	SearchPos(); 

	

}
void Agent::ParsePos(string inp)
{
	int idx=0;
	if(inp.substr(idx, 8)!="position")
	{
		cout << "invalid input " << endl;
		ASSERT(false);
	}
	idx+=9;
	if(inp.substr(idx, 8)=="startpos")
	{
		b->LoadPosition(START);	
	}
	else
	{
		auto ip = inp.find("fen");
		if(ip==string::npos)
			b->LoadPosition(START);
		else
			b->LoadPosition(inp.substr(ip+4, MAXINPUTSIZE));
	}

	auto ip = inp.find("moves");
	if(ip!=string::npos)
	{
		ip+=6;
		
		while(ip<inp.size())
		{
			string mv;
			while(ip<inp.size() && inp[ip]!=' ')
				mv+=inp[ip++];
			ip++;
			int move = ParseMove(mv);
			b->MakeMove(move);
			b->ply=0;
		}
			
	}
}


void Agent::UCIloop()
{
	setbuf(stdin, NULL);
	setbuf(stdout, NULL);

	cout << "id name " << NAME << endl;
	cout << "id author " << AUTHOR << endl;

	cout << "uciok" << endl;

	while(!sInfo.quit)
	{
		fflush(stdout);
		string inp;
		getline(cin, inp);
		if(inp=="\n")
			continue;
		if(inp=="isready")
			cout << "readyok" << endl;
		else if(inp.substr(0,8)=="position")
			ParsePos(inp);
		else if (inp=="ucinewgame")
			ParsePos("position startpos");
		else if(inp.substr(0,2)=="go")
			ParseGO(inp);
		else if(inp=="quit")
			sInfo.quit = true;
		else if(inp=="uci")
		{
			cout << "id name " << NAME << '\n';
			cout << "id author " << AUTHOR << '\n';
			cout << "uciok" << '\n';
		}
		

			
		

	}
}

