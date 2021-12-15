#include "board.h"
#include "move.h"
bool History::operator<(const History &h)
{
	return posKey < h.posKey;
}

bool Board::OnBoard(int sq)
{
	if(get64from120[sq]==OFFBOARD)
		return false;
	if(getFilefrom120[sq]<0 || getFilefrom120[sq]>7)
		return false;
	if(getRankfrom120[sq]<0 || getRankfrom120[sq]>7)
		return false;
	return true;
}
bool Board::NonEmpty(int sq)
{
	if(!OnBoard(sq))
		return false;
	return (pieces[sq]!=EMPTY);
}

void Board::HashPc(int pc, int sq)
{
	posKey ^= pieceKeys[pc][sq];
}
void Board::HashCas()
{
	posKey ^= casKeys[casInfo];
}
void Board::HashEP()
{
	posKey ^= pieceKeys[EMPTY][enPas];
}
void Board::HashSide()
{
	posKey ^= sideKey;
}

U64 Board::SetHash()
{
	posKey=0;
	for(int sq=0; sq<120; sq++)
	{
		int pc = pieces[sq];
		if(pc==EMPTY || pc==NO_SQ || pc==OUT)
			continue;
		ASSERT(pc>=wP && pc<=bK);
		posKey ^= pieceKeys[pc][sq];
	}
	if(sideToMove==WHITE)
		posKey ^= sideKey;
	if(enPas!=NO_SQ)
	{
		ASSERT(OnBoard(enPas));
		posKey ^= pieceKeys[0][enPas];
	}
	ASSERT(casInfo>=0 && casInfo<=15);
	posKey ^= casKeys[casInfo];

	return posKey;
	// posKey ^= fiftyKeys[fiftyMove];
}

void Board::ResetBoard()
{
	for(int i=0; i<120; i++)
		pieces[i]=OUT;
	// for(int i=0; i<3; i++)
	// 	pawns[i].Reset();
	pawns.Reset();
	
	sideToMove = BOTH;
	enPas = NO_SQ;
	casInfo = 0;
	fiftyMove = 0;

	posKey = 0;

	for(int i=0; i<13; i++)
		pcCount[i]=0;
	for(int i=0; i<2; i++)
	{
		bigCount[i]=0;
		majCount[i]=0;
		minCount[i]=0;
		material[i]=0;
	}

	for(int i=0; i<13; i++)
		pcList[i].Reset();
	
	hist = vector<History>(0);
	ply=0;
}

void Board::LoadPosition(string fen)
{
	ResetBoard();
	int r=7;
	int f=0;
	int idx=0;
	int piece=EMPTY;
	while(r>=0 && idx<fen.size())
	{
		int cnt=1;
		switch(fen[idx])
		{
			case 'p': piece = bP; break;
			case 'r': piece = bR; break;
			case 'n': piece = bN; break;
			case 'b': piece = bB; break;
			case 'k': piece = bK; break;
			case 'q': piece = bQ; break;
			case 'P': piece = wP; break;
			case 'R': piece = wR; break;
			case 'N': piece = wN; break;
			case 'B': piece = wB; break;
			case 'K': piece = wK; break;
			case 'Q': piece = wQ; break;

			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
				piece = EMPTY;
				cnt = fen[idx]-'0';
				break;
			
			case '/':
			case ' ':
				r--;
				f=0;
				idx++;
				continue;
			
			default:
				cout << "fen error at index " << idx << endl;
				return;

		}

		while(cnt--)
		{
			int sq = 8*r+f;
			int sq120 = get120from64[sq];
			pieces[sq120]=piece;
			f++;
		}
		idx++;
		
	}
	ASSERT(fen[idx]=='w' || fen[idx]=='b');
	if(fen[idx]=='w')
		sideToMove = WHITE;
	else
		sideToMove = BLACK;
	
	idx+=2;
	for(int i=0; i<4; i++)
	{
		if(fen[idx]==' ')
			break;
		switch(fen[idx])
		{
			case 'K': casInfo |= WKCA; break;
			case 'Q': casInfo |= WQCA; break;
			case 'k': casInfo |= BKCA; break;
			case 'q': casInfo |= BQCA; break;
			default:	     break;
		}
		idx++;
	}
	idx++;

	ASSERT(casInfo>=0 || casInfo<=15);

	if(fen[idx]!='-')
	{
		int epFile = fen[idx]-'a';
		int epRank = fen[idx+1]-'1';

		enPas = get120fromFR[epFile][epRank];
	}
	idx+=3;
	while(fen[idx]<='9' && fen[idx]>='0')
	{
		fiftyMove = 10*fiftyMove + fen[idx]-'0';
		idx++;
	}
	SetHash();
	UpdateCount();
}

void Board::UpdateCount()
{
	for(int i=0; i<120; i++)
	{
		if(pieces[i]==EMPTY || pieces[i]==OUT)
			continue;
		int pc = pieces[i];
		pcCount[pc]++;
		pcList[pc].SetBit(get64from120[i]);
		int col = pcCol[pc];
		if(isBig[pc])
			bigCount[col]++;
		if(isMajor[pc])
			majCount[col]++;
		if(isMinor[pc])
			minCount[col]++;

		material[col] += pcVal[pc];

		if(isPawn[pc])
		{
			int sq64 = get64from120[i];
			pawns.SetBit(sq64);
		}
	}
}

void Board::Print(bool onlyboard)
{
	for(int r=7; r>=0; r--)
	{
		for(int f=0; f<8; f++)
		{
			int sq = get120fromFR[f][r];
			cout << pcName[pieces[sq]] << " ";
		}
		cout << endl;
	}
	cout << endl;
	if(onlyboard)
		return;
	pawns.Print();
	
	cout << endl;
	for(int i=1; i<13; i++)
	{
		cout << pcName[i] << ": " << pcCount[i] << "[ ";
		// for(int x:pieceList[i])
		// 	cout << getNamefrom120[x] << " ";
		U64 t_info = pcList[i].info;
		while(pcList[i].info)
		{
			int sq = pcList[i].PopBit();
			cout << getNamefrom120[get120from64[sq]] << ' ';
		}
		pcList[i].info = t_info;
		cout << "]" << endl;
	}
	cout << endl;
	for(int i=0; i<2; i++)
	{
		cout << bigCount[i] << " " << majCount[i] << " " << minCount[i] << " " << material[i] << endl;
	}
	cout << "To Play: " << sideToMove << endl;
	cout << "en Pas: " << getNamefrom120[enPas] << endl;
	cout << "casInfo: " << bitset<4> (casInfo) << endl;
	cout << "fiftyMove: " << fiftyMove << endl;
	cout << "posKey: " << posKey << endl;

}

bool Board::Verify()
{
	// cout << "called";
	BitBoard t_pawns[3];
	int t_pcCount[13], t_bigCount[2], t_majCount[2], t_minCount[2], t_material[2];

	for(int i=0; i<13; i++)
		t_pcCount[i]=0;
	for(int i=0; i<2; i++)
	{
		t_bigCount[i]=0;
		t_majCount[i]=0;
		t_minCount[i]=0;
		t_material[i]=0;
	}
	for(int i=0; i<120; i++)
	{
		int pc = pieces[i];
		if(pc==EMPTY||pc==OUT)
			continue;
		int col = pcCol[pc];
		t_pcCount[pc]++;
		if(isBig[pc])
			t_bigCount[col]++;
		if(isMajor[pc])
			t_majCount[col]++;
		if(isMinor[pc])
			t_minCount[col]++;
		t_material[col] += pcVal[pc];
		if(isPawn[pc])
		{
			int sq64 = get64from120[i];
			t_pawns[col].SetBit(sq64);
			t_pawns[BOTH].SetBit(sq64);
			
		}
		//ASSERT(pieceList[pc].find(i)!=pieceList[pc].end());
	}
	//check piece list
	for(int i=1; i<13; i++)
	{
		// for(int sq:pieceList[i])
		// 	ASSERT(i==pieces[sq]);
		U64 t_info = pcList[i].info;
		while(pcList[i].info)
		{
			int sq = get120from64[pcList[i].PopBit()];
			ASSERT(i==pieces[sq]);
		}
		pcList[i].info = t_info;
	}
	//check bitboard
	// for(int i=0; i<3; i++)
	// 	ASSERT(t_pawns[i].info==pawns[i].info);

	// check count
	for(int i=1; i<13; i++)
	{
		ASSERT(t_pcCount[i]==pcCount[i]);
		// ASSERT(t_pcCount[i]==pieceList[i].size());
	}
	
	for(int i=0; i<2; i++)
	{
		ASSERT(t_bigCount[i]==bigCount[i]);
		ASSERT(t_majCount[i]==majCount[i]);
		ASSERT(t_minCount[i]==minCount[i]);
		ASSERT(t_material[i]==material[i]);
	}
	// ASSERT(t_pcCount[bP]==pawns[BLACK].CountBits());
	// ASSERT(t_pcCount[wP]==pawns[WHITE].CountBits());

	//exactly 1 king
	// ASSERT(pieceList[wK].size()==1 && pieceList[bK].size()==1);
	U64 curKey = posKey;
	SetHash();
	if(curKey!=posKey)
		cout << "curkey: " << curKey << " poskey: " << posKey << endl;
	ASSERT(curKey==posKey);
	return true;
}

int Board::IsAttacked(int sq120, int side)
{
	//pawns
	if(side==WHITE)
	{
		if(pieces[sq120-11]==wP || pieces[sq120-9]==wP)
			return wP;
	}
	if(side==BLACK)
	{
		if(pieces[sq120+11]==bP || pieces[sq120+9]==bP)
			return bP;
	}
	// knights
	for(int dir:directions[Knight])
	{
		int cur = dir+sq120;
		if(pieces[cur]==OUT)
			continue;
		if(isKnight[pieces[cur]] && pcCol[pieces[cur]]==side)
			return pieces[cur];
	}
	
	// rook + queen
	for(int dir:directions[Rook])
	{
		int cur = sq120;
		while(pieces[cur+dir]!=OUT)
		{
			cur+=dir;
			int pc = pieces[cur];
			if(pc==EMPTY)
				continue;
			if(pcCol[pc]!=side)
				break;
			if(isRookQueen[pc])
				return pieces[cur];
			else
				break;
		}
		
	}

	// bishop + queen
	for(int dir:directions[Bishop])
	{
		int cur = sq120;
		while(pieces[cur+dir]!=OUT)
		{
			cur+=dir;
			int pc = pieces[cur];
			if(pc==EMPTY)
				continue;
			if(pcCol[pc]!=side)
				break;
			if(isBishopQueen[pc])
				return pc;
			else
				break;
		}
		
	}

	// king
	for(int dir:directions[King])
	{
		int cur = sq120+dir;
		if(pieces[cur]==OUT)
			continue;
		if(isKing[pieces[cur]] && pcCol[pieces[cur]]==side)
			return pieces[cur];
	}
	return EMPTY;	
	
}

void Board::PrintMap()
{
	for(int side=0; side<2; side++)
	{
		cout << side << endl;
		for(int r=7; r>=0; r--)
		{
			for(int f=0; f<8; f++)
			{
				int sq120 = get120fromFR[f][r];
				int attacker = IsAttacked(sq120, side);
				if(attacker)
					cout << pcName[attacker] << ' ';
				else
					cout << ". ";
			}
			cout << endl;
		}
		cout << endl;
	}
}

void Board::ClearPiece(int sq)
{
	ASSERT(OnBoard(sq));
	ASSERT(NonEmpty(sq));

	int pc = pieces[sq];
	int col = pcCol[pc];

	HashPc(pc,sq);
	pieces[sq]=EMPTY;
	// pieceList[pc].erase(sq);
	pcList[pc].ClearBit(get64from120[sq]);
	material[col] -= pcVal[pc];
	pcCount[pc]--;

	if(isBig[pc])
		bigCount[col]--;
	if(isMajor[pc])
		majCount[col]--;
	if(isMinor[pc])
		minCount[col]--;

	if(isPawn[pc])
	{
		pawns.ClearBit(get64from120[sq]);
		
	}
	U64 cur = posKey;
	ASSERT(cur==SetHash());

}
void Board::AddPiece(int pc, int sq)
{
	if(!OnBoard(sq))
		cout << sq << endl;
	ASSERT(OnBoard(sq));
	HashPc(pc, sq);
	pieces[sq]=pc;
	pcList[pc].SetBit(get64from120[sq]);

	int col = pcCol[pc];
	pcCount[pc]++;

	if(isBig[pc])
		bigCount[col]++;
	if(isMajor[pc])
		majCount[col]++;
	if(isMinor[pc])
		minCount[col]++;

	material[col] += pcVal[pc];

	if(isPawn[pc])
	{
		pawns.SetBit(get64from120[sq]);
	}
	U64 cur = posKey;
	ASSERT(cur==SetHash());
}
void Board::MovePiece(int from, int to)
{
	ASSERT(OnBoard(from));
	ASSERT(OnBoard(to));
	ASSERT(NonEmpty(from));
	
	int pc = pieces[from];

	//ASSERT(pieceList[pc].find(from)!=pieceList[pc].end());

	pieces[from] = EMPTY;
	pieces[to] = pc;
	pcList[pc].ClearBit(get64from120[from]);
	pcList[pc].SetBit(get64from120[to]);

	if(isPawn[pc])
	{
		int col = pcCol[pc];
		pawns.ClearBit(get64from120[from]);

		pawns.SetBit(get64from120[to]);
	}
	HashPc(pc, from);
	HashPc(pc, to);

	U64 cur = posKey;
	ASSERT(posKey==SetHash());
}

void Board::Undo()
{
	History h = hist[hist.size()-1];
	hist.pop_back();
	int move = h.move;

	int from = FROMSQ(move);
	int to = TOSQ(move);
	int capt = CAPTURED(move);
	int prom = PROMOTED(move);
	int casflag = move & MFLAGCA;
	int epflag = move & MFLAGEP;
	int psflag = move & MFLAGPS;

	sideToMove ^= 1;
	HashSide();
	ply--;

	ASSERT(Verify());
	if(prom!=EMPTY)
	{
		ClearPiece(to);
		AddPiece(6*sideToMove+wP, to);
	}
	MovePiece(to, from);
	
	if(capt!=EMPTY && !epflag)
		AddPiece(capt, to);
	
	if(casflag)
	{
		switch(to)
		{
			case C1:
				MovePiece(D1, A1); break;
			case G1:
				MovePiece(F1, H1); break;
			case C8:
				MovePiece(D8, A8); break;
			case G8:
				MovePiece(F8, H8); break;
			default:
				ASSERT(false); break;
		}
	}
	else if(epflag)
	{
		if(sideToMove==WHITE)
			AddPiece(bP, to-10);
		else
			AddPiece(wP, to+10);
	}
	if(enPas!=NO_SQ)
		HashEP();
	HashCas();
	enPas = h.enPas;
	if(enPas != NO_SQ)
		HashEP();
	fiftyMove = h.fiftyMove;
	casInfo = h.casInfo;
	HashCas();

	ASSERT(posKey == h.posKey);
	ASSERT(Verify());
}

bool Board::MakeMove(int move)
{
	ASSERT(Verify());
	//cout << "verified" << endl;
	//PrintMove(move);
	int from = FROMSQ(move);
	int to = TOSQ(move);
	int capt = CAPTURED(move);
	int prom = PROMOTED(move);
	int casflag = move & MFLAGCA;
	int epflag = move & MFLAGEP;
	int psflag = move & MFLAGPS;

	ASSERT(OnBoard(from));
	ASSERT(OnBoard(to));
	ASSERT(NonEmpty(from));

	History h;
	h.posKey = posKey;
	h.fiftyMove = fiftyMove;
	h.enPas = enPas;
	h.casInfo = casInfo;
	h.move = move;
	hist.push_back(h);

	fiftyMove++;
	ply++;

	if(enPas != NO_SQ)
		{HashEP(); enPas = NO_SQ;}
	HashCas();

	if(casflag)
	{
		switch(to)
		{
			case C1:
				MovePiece(A1, D1); break;
			case G1:
				MovePiece(H1, F1); break;
			case C8:
				MovePiece(A8, D8); break;
			case G8:
				MovePiece(H8, F8); break;
			default:
				ASSERT(false); break;
		}
	}
	else if(epflag)
	{
		if(sideToMove==WHITE)
			ClearPiece(to-10);
		else
			ClearPiece(to+10);
	}
	else if(psflag)
	{
		if(sideToMove==WHITE)
		{
			
			enPas = to-10; 
			ASSERT(getRankfrom120[to]==RANK_4);
		}
		else
		{
			enPas = to+10; 
			ASSERT(getRankfrom120[to]==RANK_5);
		}
		HashEP();
	}

	casInfo &= casPerm[from];
	casInfo &= casPerm[to];
	HashCas();

	int pc = pieces[from];
	ASSERT(pcCol[pc]==sideToMove);

	if(capt!=EMPTY && !epflag)
	{
		ClearPiece(to);
		fiftyMove = 0;
	}
	if(isPawn[pc])
		fiftyMove=0;

	MovePiece(from, to);
	
	if(prom!=EMPTY)
	{
		ClearPiece(to);
		AddPiece(prom, to);
	}
	sideToMove ^= 1;
	HashSide();
	
	ASSERT(Verify());
	int kingsq = pcList[(1^sideToMove)*6 + wK].PopBit();
	pcList[(1^sideToMove)*6 + wK].SetBit(kingsq);
	kingsq = get120from64[kingsq];
	//cout << "ended, next move is of " << sideToMove << endl;
	if(IsAttacked(kingsq, sideToMove))
	{
		Undo();
		return false;
	}
	return true;
}


