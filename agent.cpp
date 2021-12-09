#include "agent.h"

Agent::Agent(Board* _b)
{
	b = _b;
	moveList = vector<Move>(0);
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
	m.score = 0;
	moveList.push_back(m);
}
void Agent::AddEnPassantMove(int move)
{
	Move m;
	m.move = move;
	m.score = 0;
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

/*----------------------------------------------------WHITE PAWN--------------------------------------------*/
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