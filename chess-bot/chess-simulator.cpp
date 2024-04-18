#include "chess-simulator.h"
#include "chess.hpp"
#include <random>

using namespace ChessSimulator;

std::string ChessSimulator::Move(const std::string& fen) {
    // Create a board state from this FEN
#ifdef INCLUDE_PRINTS
    printf("[[ New Move ]]\n");
#endif
    chess::Board board(fen);
    std::srand(time(nullptr));

    chess::ChessTree* g_ChessTree = new chess::ChessTree();
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

#ifdef INCLUDE_PRINTS
    printf("Best Move: %s\n", bestMoveUCI.c_str());
#endif
    return bestMoveUCI;
}
