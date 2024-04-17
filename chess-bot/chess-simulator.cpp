#include "chess-simulator.h"
// disservin's lib. drop a star on his hard work!
// https://github.com/Disservin/chess-library
#include "chess.hpp"
#include <random>

using namespace ChessSimulator;

std::string ChessSimulator::Move(const std::string& fen) {
    // Create a board state from this FEN
    chess::Board board(fen);

    if (g_ChessTree == nullptr) {
        g_ChessTree = new chess::ChessTree();
    }
    if (g_ChessTree->getRoot() == nullptr) { g_ChessTree->createRoot(board); }
    else {
        printf("Wow!");
    }

    // TODO: do number of iterations
    for (uint32_t i = 0; i < 100; i++) {
        chess::ChessTreeNode* bestNode = g_ChessTree->selectNode(g_ChessTree->getRoot());
        chess::ChessTreeNode* newNode = g_ChessTree->expandNode(bestNode);
        float mcEval = g_ChessTree->mcEvalNode(newNode);
        if (mcEval == 1)
        {
            g_ChessTree->backpropagation(newNode, true);
        }
        else
        {
            g_ChessTree->backpropagation(newNode, false);
        }
    }


//  chess::Movelist moves;
//  chess::movegen::legalmoves(moves, board);
//  if(moves.size() == 0)
//    return "";
//
//  // get random move
//  std::vector<std::string> moveUcis;
//  for (int i = 0; i < moves.size(); i++) {
//    std::string uci = chess::uci::moveToUci(moves[i]);
//    std::cout << uci << std::endl;
//    moveUcis.push_back(uci);
//  }
//  auto uciCompare = [](const std::string& a, const std::string& b) { return a < b; };
//  std::sort(moveUcis.begin(), moveUcis.end(), uciCompare);
//
//  return moveUcis.front();
}
