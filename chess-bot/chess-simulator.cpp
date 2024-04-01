#include "chess-simulator.h"
#include "chess.hpp"
#include <random>
using namespace ChessSimulator;

std::string ChessSimulator::Move(std::string fen) {
  // create your board based on the board string following the FEN notation
  // search for the best move using minimax / monte carlo tree search /
  // alpha-beta pruning / ... try to use nice heuristics to speed up the search
  // and have better results return the best move in UCI notation you will gain
  // extra points if you create your own board/move representation instead of
  // using the one provided by the library

  // here goes a random movement
  chess::Board board(fen);
  chess::Movelist moves;
  chess::movegen::legalmoves(moves, board);
  if(moves.size() == 0)
    return "";

  // get random move
  std::vector<std::string> moveUcis;
  for (int i = 0; i < moves.size(); i++) {
    std::string uci = chess::uci::moveToUci(moves[i]);
    std::cout << uci << std::endl;
    moveUcis.push_back(uci);
  }
  auto uciCompare = [](std::string a, std::string b) { return a < b; };
  std::sort(moveUcis.begin(), moveUcis.end(), uciCompare);

  return moveUcis.front();
}

float ChessSimulator::MeasureStateJuiciness(std::string fen, chess::Color side)
{
  chess::Board board(fen);
  float juiciness = 1.0f;

  if (board.inCheck()) {
    return 100.f;
  }

  if (board.enpassantSq().is_valid()) {
    return -100.f;
  }

  if (board.hasNonPawnMaterial(side)) {
    juiciness += 1.0f;
  }
  else {
    juiciness -= 1.0f;
  }

  chess::Movelist moves;
  chess::movegen::legalmoves(moves, board);

  return juiciness;
}
