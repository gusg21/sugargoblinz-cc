#include "chess-simulator.h"
// disservin's lib. drop a star on his hard work!
// https://github.com/Disservin/chess-library
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

ChessTreeNode *ChessTree::newNode(ChessTreeNode *parent, const chess::Move& move) {
    ChessTreeNode node;

    chess::Board board;

    node.setParent(parent);
    node.setBoard(parent->getBoard());




    return nullptr;
}

ChessTree::ChessTree() {
}
