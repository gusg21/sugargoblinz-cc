#include "chess-simulator.h"
#include "chess-simulator.h"
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
    chess::Color rootColor = board.sideToMove();
    srand(time(nullptr));

    auto* g_ChessTree = new chess::ChessTree();
    g_ChessTree->createRoot(board);

    // TODO: do number of iterations
    for (uint32_t i = 0; i < 500; i++) {
        chess::ChessTreeNode* selected = g_ChessTree->selectNode(g_ChessTree->getRoot());
        if (chess::ChessTree::hasMoves(selected)) {
            chess::ChessTreeNode* newNode = g_ChessTree->expandNode(selected);
            float mcEval = g_ChessTree->mcEvalNode(newNode);
            g_ChessTree->backpropagation(newNode, mcEval, rootColor);
        }
        g_ChessTree->debugPrint();
    }
    

    std::string bestMoveUCI = g_ChessTree->getBestMove();

    delete g_ChessTree;

#ifdef INCLUDE_PRINTS
    printf("Best Move: %s\n", bestMoveUCI.c_str());
#endif
    return bestMoveUCI;
}

std::pair<float, std::string> ChessSimulator::GetEval(const std::string& _fen)
{
    chess::Board board(_fen);

    chess::ChessTree* g_ChessTree = new chess::ChessTree();
    g_ChessTree->createRoot(board);

    std::string fen {};
    float eval = g_ChessTree->mcEvalNode(g_ChessTree->getRoot(), fen);

    return {
        eval, fen
    };
}

float ChessSimulator::GetHeuristic(const chess::Board& board)
{
    return chess::ChessTree::getHeuristicValue(board);
}
