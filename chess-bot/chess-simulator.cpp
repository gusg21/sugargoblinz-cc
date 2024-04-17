#include "chess-simulator.h"
#include "chess.hpp"
#include <random>

using namespace ChessSimulator;

std::string ChessSimulator::Move(const std::string& fen) {
    // Create a board state from this FEN
    printf("[[ New Move ]]\n");
    chess::Board board(fen);
    std::srand(std::time(nullptr));

    g_ChessTree = new chess::ChessTree();
    g_ChessTree->createRoot(board);

    // TODO: do number of iterations
    for (uint32_t i = 0; i < 1000; i++) {
        chess::ChessTreeNode* bestNode = g_ChessTree->selectNode(g_ChessTree->getRoot());
        chess::ChessTreeNode* newNode = g_ChessTree->expandNode(bestNode);
        float mcEval = g_ChessTree->mcEvalNode(newNode);
        g_ChessTree->backpropagation(newNode, mcEval);
    }
    g_ChessTree->debugPrint();

    std::string bestMoveUCI = g_ChessTree->getBestMove();

    delete g_ChessTree;

    printf("Best Move: %s\n", bestMoveUCI.c_str());
    return bestMoveUCI;


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
