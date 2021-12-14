all:
	g++ main.cpp bitboard.cpp defs.cpp board.cpp agent.cpp move.cpp -pthread -std=c++17 -O2 -o main