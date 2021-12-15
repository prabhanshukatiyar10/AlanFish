#include "agent.h"

Agent::Agent(Board* _b)
{
	b = _b;
	moveList = vector<Move>(0);
	cache = LRU(cache_size);
	priVar = vector<int>(0);
}

void Agent::PrintList()
{
	int i=1;
	for(auto m:moveList)
	{
		cout << i++ << ": ";
		PrintMove(m.move);
	}
}

// General Functions
void Agent::AddQuietMove(int move)
{
	Move m;
	m.move = move;
	m.score = 0;
	moveList.push_back(m);
}
void Agent::AddCaptureMove(int move)
{
	Move m;
	m.move = move;
	m.score = pcVal[b->pieces[TOSQ(move)]] - pcVal[b->pieces[FROMSQ(move)]]/10;
	moveList.push_back(m);
}
void Agent::AddEnPassantMove(int move)
{
	Move m;
	m.move = move;
	m.score = 90;
	moveList.push_back(m);
}



/*----------------------------------------------------WHITE PAWN--------------------------------------------*/
void Agent::AddWhitePawnMove(int from, int to)
{
	if(getRankfrom120[from]==RANK_7)
	{
		AddQuietMove(MOVE(from, to, EMPTY, wN, 0));
		AddQuietMove(MOVE(from, to, EMPTY, wB, 0));
		AddQuietMove(MOVE(from, to, EMPTY, wR, 0));
		AddQuietMove(MOVE(from, to, EMPTY, wQ, 0));
	}
	else if(to-from!=20)
		AddQuietMove(MOVE(from, to, EMPTY, EMPTY, 0));
	else
		AddQuietMove(MOVE(from, to, EMPTY, EMPTY, MFLAGPS));
}
void Agent::AddWhitePawnCapMove(int from, int to, int cap)
{
	if(getRankfrom120[from]==RANK_7)
	{
		AddCaptureMove(MOVE(from, to, cap, wN, 0));
		AddCaptureMove(MOVE(from, to, cap, wB, 0));
		AddCaptureMove(MOVE(from, to, cap, wR, 0));
		AddCaptureMove(MOVE(from, to, cap, wQ, 0));
	}
	else
		AddCaptureMove(MOVE(from, to, cap, EMPTY, 0));
}
void Agent::AddAllWhitePawnMoves()
{
	U64 t_info = b->pcList[wP].info;
	while(b->pcList[wP].info)
	{
		int sq120 = get120from64[b->pcList[wP].PopBit()];
		if(b->pieces[sq120+10]==EMPTY)
			AddWhitePawnMove(sq120, sq120+10);
		if(pcCol[b->pieces[sq120+9]]==BLACK)
			AddWhitePawnCapMove(sq120, sq120+9, b->pieces[sq120+9]);
		if(pcCol[b->pieces[sq120+11]]==BLACK)
			AddWhitePawnCapMove(sq120, sq120+11, b->pieces[sq120+11]);

		if(getRankfrom120[sq120]==RANK_2)
			if(b->pieces[sq120+10]==EMPTY && b->pieces[sq120+20]==EMPTY)
				AddWhitePawnMove(sq120, sq120+20);

		if((sq120+9)==b->enPas || (sq120+11)==b->enPas)
			AddEnPassantMove(MOVE(sq120, b->enPas, bP, EMPTY, MFLAGEP));
		}
	b->pcList[wP].info = t_info;

	// for(int sq120:b->pieceList[wP])
	// {
		
	// }
}

void Agent::AddAllWhitePawnCaps()
{
	U64 t_info = b->pcList[wP].info;
	while(b->pcList[wP].info)
	{
		int sq120 = get120from64[b->pcList[wP].PopBit()];

		if(pcCol[b->pieces[sq120+9]]==BLACK)
			AddWhitePawnCapMove(sq120, sq120+9, b->pieces[sq120+9]);
		if(pcCol[b->pieces[sq120+11]]==BLACK)
			AddWhitePawnCapMove(sq120, sq120+11, b->pieces[sq120+11]);

		if((sq120+9)==b->enPas || (sq120+11)==b->enPas)
			AddEnPassantMove(MOVE(sq120, b->enPas, bP, EMPTY, MFLAGEP));
		}
	b->pcList[wP].info = t_info;
}

/*----------------------------------------------------BLACK PAWN--------------------------------------------*/
void Agent::AddBlackPawnMove(int from, int to)
{
	if(getRankfrom120[from]==RANK_2)
	{
		AddQuietMove(MOVE(from, to, EMPTY, bN, 0));
		AddQuietMove(MOVE(from, to, EMPTY, bB, 0));
		AddQuietMove(MOVE(from, to, EMPTY, bR, 0));
		AddQuietMove(MOVE(from, to, EMPTY, bQ, 0));
	}
	else if(to-from!=-20)
		AddQuietMove(MOVE(from, to, EMPTY, EMPTY, 0));
	else
		AddQuietMove(MOVE(from, to, EMPTY, EMPTY, MFLAGPS));
}
void Agent::AddBlackPawnCapMove(int from, int to, int cap)
{
	if(getRankfrom120[from]==RANK_2)
	{
		AddCaptureMove(MOVE(from, to, cap, bN, 0));
		AddCaptureMove(MOVE(from, to, cap, bB, 0));
		AddCaptureMove(MOVE(from, to, cap, bR, 0));
		AddCaptureMove(MOVE(from, to, cap, bQ, 0));
	}
	else
		AddCaptureMove(MOVE(from, to, cap, EMPTY, 0));
}
void Agent::AddAllBlackPawnMoves()
{
	U64 t_info = b->pcList[bP].info;
	while(b->pcList[bP].info)
	{
		int sq120 = get120from64[b->pcList[bP].PopBit()];
		if(b->pieces[sq120-10]==EMPTY)
			AddBlackPawnMove(sq120, sq120-10);

		if(pcCol[b->pieces[sq120-9]]==WHITE)
			AddBlackPawnCapMove(sq120, sq120-9, b->pieces[sq120-9]);
		if(pcCol[b->pieces[sq120-11]]==WHITE)
			AddBlackPawnCapMove(sq120, sq120-11, b->pieces[sq120-11]);

		if(getRankfrom120[sq120]==RANK_7)
			if(b->pieces[sq120-10]==EMPTY && b->pieces[sq120-20]==EMPTY)
				AddBlackPawnMove(sq120, sq120-20);

		if((sq120-9)==b->enPas || (sq120-11)==b->enPas)
			AddEnPassantMove(MOVE(sq120, b->enPas, wP, EMPTY, MFLAGEP));
		}
	b->pcList[bP].info = t_info;
	// for(int sq120:b->pieceList[bP])
	// {
		
	// }
}



void Agent::AddAllBlackPawnCaps()
{
	U64 t_info = b->pcList[bP].info;
	while(b->pcList[bP].info)
	{
		int sq120 = get120from64[b->pcList[bP].PopBit()];


		if(pcCol[b->pieces[sq120-9]]==WHITE)
			AddBlackPawnCapMove(sq120, sq120-9, b->pieces[sq120-9]);
		if(pcCol[b->pieces[sq120-11]]==WHITE)
			AddBlackPawnCapMove(sq120, sq120-11, b->pieces[sq120-11]);


		if((sq120-9)==b->enPas || (sq120-11)==b->enPas)
			AddEnPassantMove(MOVE(sq120, b->enPas, wP, EMPTY, MFLAGEP));
		}
	b->pcList[bP].info = t_info;
}


/*----------------------------------------------------SLIDING--------------------------------------------*/
/*----------------------------------------------------SLIDING--------------------------------------------*/
/*----------------------------------------------------SLIDING--------------------------------------------*/
/*----------------------------------------------------SLIDING--------------------------------------------*/


void Agent::AddSlidingMoves()
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
							AddQuietMove(MOVE(sq,cursq,EMPTY,EMPTY, 0));
						else if(pcCol[b->pieces[cursq]]==b->sideToMove)
							break;
						else
						{
							AddCaptureMove(MOVE(sq, cursq, b->pieces[cursq], EMPTY, 0));
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

void Agent::AddNonSlidingMoves()
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
					AddQuietMove(MOVE(sq, sq+d,EMPTY,EMPTY,0));
				else if(pcCol[curpc]==b->sideToMove)
					continue;
				else
					AddCaptureMove(MOVE(sq,sq+d,b->pieces[sq+d],EMPTY,0));

			}
		}
		b->pcList[pc].info = t_info;
	}
}


/*----------------------------------------------------CASTLES--------------------------------------------*/
/*----------------------------------------------------CASTLES--------------------------------------------*/
/*----------------------------------------------------CASTLES--------------------------------------------*/
/*----------------------------------------------------CASTLES--------------------------------------------*/

void Agent::AddWhiteCastles()
{
	if((b->casInfo & WKCA) && b->pieces[E1]==wK && b->pieces[H1]==wR && b->pieces[F1]==EMPTY && b->pieces[G1]==EMPTY)
	{
		if( !( b->IsAttacked(E1, BLACK) || b->IsAttacked(F1, BLACK) || b->IsAttacked(G1, BLACK) ) )
		{
			Move m;
			m.move = MOVE(E1, G1, EMPTY, EMPTY, MFLAGCA);
			m.score = 0;
			moveList.push_back(m);
		}
	}
	if((b->casInfo & WQCA) && b->pieces[E1]==wK && b->pieces[A1]==wR && b->pieces[B1]==EMPTY && b->pieces[C1]==EMPTY && b->pieces[D1]==EMPTY)
	{
		if( !( b->IsAttacked(E1, BLACK) || b->IsAttacked(D1, BLACK) || b->IsAttacked(C1, BLACK) ) )
		{
			Move m;
			m.move = MOVE(E1, C1, EMPTY, EMPTY, MFLAGCA);
			m.score = 0;
			moveList.push_back(m);
		}
	}
}
void Agent::AddBlackCastles()
{
	if((b->casInfo & BKCA) && b->pieces[E8]==bK && b->pieces[H8]==bR && b->pieces[F8]==EMPTY && b->pieces[G8]==EMPTY)
	{
		if( !( b->IsAttacked(E8, WHITE) || b->IsAttacked(F8, WHITE) || b->IsAttacked(G8, WHITE) ) )
		{
			Move m;
			m.move = MOVE(E8, G8, EMPTY, EMPTY, MFLAGCA);
			m.score = 0;
			moveList.push_back(m);
		}
	}
	if((b->casInfo & BQCA) && b->pieces[E8]==bK && b->pieces[A8]==bR && b->pieces[B8]==EMPTY && b->pieces[C8]==EMPTY && b->pieces[D8]==EMPTY)
	{
		if( !( b->IsAttacked(E8, WHITE) || b->IsAttacked(D8, WHITE) || b->IsAttacked(C8, WHITE) ) )
		{
			Move m;
			m.move = MOVE(E8, C8, EMPTY, EMPTY, MFLAGCA);
			m.score = 0;
			moveList.push_back(m);
		}
	}
}

vector<Move> Agent::FindMoves()
{

	moveList = vector<Move>(0);
	if(b->sideToMove==WHITE)
	{
		AddAllWhitePawnMoves();
		AddWhiteCastles();
	}
	else
	{
		AddAllBlackPawnMoves();
		AddBlackCastles();
	}
	AddSlidingMoves();
	AddNonSlidingMoves();
	sort(moveList.begin(), moveList.end());
	return moveList;
}

vector<Move> Agent::FindCaptures()
{
	moveList = vector<Move>(0);
	if(b->sideToMove==WHITE)
		AddAllWhitePawnCaps();
	else
		AddAllBlackPawnCaps();

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
							{}
						else if(pcCol[b->pieces[cursq]]==b->sideToMove)
							break;
						else
						{
							AddCaptureMove(MOVE(sq, cursq, b->pieces[cursq], EMPTY, 0));
							break;
						}
					}
				}
			}
		}
		b->pcList[pc].info = t_info;
	}

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
					{}
				else if(pcCol[curpc]==b->sideToMove)
					continue;
				else
					AddCaptureMove(MOVE(sq,sq+d,b->pieces[sq+d],EMPTY,0));

			}
		}
		b->pcList[pc].info = t_info;
	}
	return moveList;
	
}

void Agent::TotalMoves(int depth, U64 &ans)
{
	if(depth==0)
	{
		ans++;
		return;
	}
	auto mL = FindMoves();
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
	FindMoves();
	for(Move m:moveList)
	{
		int move = m.move;
		if(FROMSQ(move)==from && TOSQ(move)==to)
		{
			if(prom==0)
				return move;
			if(pcName[PROMOTED(move)]==prom)
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
	FindMoves();
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

int Agent::SetPV(int depth)
{
	priVar = vector<int>(0);
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
	return priVar.size();
}

void Agent::PrintPV()
{
	// cout << "@@@@ " << table.data.size() << endl;
	for(int mv:priVar)
		PrintMove(mv);
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

	sInfo.startTime=clock();
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
		

		
		SetPV(cd);
		bestMove = priVar[0];

		cout << "Nodes: " << sInfo.nodes <<  " Depth: " << cd << " Score: " << bestScore << " Move: ";
		PrintMove(bestMove);
		PrintPV();
		cout << "Ordering: " << sInfo.fhf/sInfo.fh << endl;
	}
}

int Agent::AlphaBeta(int alpha, int beta, int depth, int doNull)
{
	//ASSERT(b->Verify());
	if(depth==0)
	{
		return Quiescence(alpha, beta);
	}
	if(Repeated()||b->fiftyMove>=100)
		return 0;
		
	if(b->ply>=MAXDEPTH)
		return EvalPos();
	
	vector<Move> mL = FindMoves();
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

	


	vector<Move> mL = FindCaptures();
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



